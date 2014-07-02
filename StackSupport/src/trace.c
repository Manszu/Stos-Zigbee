/**
 * Copyright (c) 2005-2007 LuxLabs Ltd. dba MeshNetics, All Rights Reserved.
 *
 * No changes (alteration, merge, modification, reverse engineer) can be applied
 * to the source or binary code without specific prior written permission.   
 *
 * Redistribution, sublicensing and use in source and binary forms 
 * are permitted provided that the following conditions are met:
 *
 * - Redistribution, sublicensing of this source code and its derivatives is 
 *  allowed only with expressed written permission of MeshNetics and must 
 *  retain the above copyright notice, this list of conditions and 
 *  the following disclaimer.
 *
 * - Redistribution, sublicensing of this software or its derivatives in binary
 *  form is allowed only with expressed written permission of MeshNetics and 
 *  must reproduce the above copyright notice, this list of conditions and 
 *  the following disclaimer in the documentation and/or other materials 
 *  provided with the distribution.
 *
 * Neither the name of LuxLabs Ltd., MeshNetics, or the names of contributors 
 * may be used to endorse or promote products derived from this software 
 * without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty or support of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING 
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MESHNETICS AND ITS LICENSORS SHALL 
 * NOT BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS 
 * DERIVATIVES. IN NO EVENT WILL MESHNETICS OR ITS LICENSORS BE LIABLE 
 * FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, 
 * CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS 
 * OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE 
 * SOFTWARE, EVEN IF MESHNETICS HAS BEEN ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGES.
 *
 * You acknowledge that this software is not designed, licensed or intended for 
 * use in the design, construction, operation or maintenance of any nuclear 
 * facility, medical or other mission-critical system.
 **/

/********************************************************************************
  Trace functions implementation.
********************************************************************************/

#include "trace.h"
#include "bsc.h"
#include "sbuf.h"
#include "atomic.h"

#define   TRACE_SERIAL_CHANNEL    1 // Trace serial channel number.

// Trace buffer size, in bytes.
#ifndef TRACE_BUFFER_SIZE
#define TRACE_BUFFER_SIZE 100 
#endif

// Trace message max size, in bytes.
#ifndef TRACE_MESSAGE_MAX_SIZE
#define TRACE_MESSAGE_MAX_SIZE 16 
#endif

#define           TRACE_MODULE 0xFF // Module ID for loss message.
#define    LOST_MESSAGE_LENGTH 0x03 // Size of loss message.
#define      LOST_MESSAGE_CODE 0x00 // Code for loss message.

/********************************************************************************
  Any message consists of:
                                   - length     1 byte
                                   - code       1 byte
                                   - parameters N byte
  Storage buffer has always to provide capacity for loss message.
  The loss message informs upper level about user's messages loss.
  The loss message consists of:
                                   - length                   1 byte
                                   - code (LOST_MESSAGE_CODE) 1 byte
                                   - counter of lost messages 1 byte
  Full flag in storage buffer means that last message in buffer is 
  a loss message. At the same time startLast means start of the 
  loss message.
********************************************************************************/

static struct // Storage buffer.
{
  uint8_t  data[TRACE_BUFFER_SIZE]; // Round robin buffer for trace messages.
  uint16_t                   start; // First busy byte pointer.
  uint16_t                     end; // First free byte pointer.
  uint16_t             freeCounter; // Free bytes counter.
  bool                        full; // Full flag.
  bool                       empty; // Empty flag.
  uint16_t               startLast; // Start of the last message.
  bool                 lossMessage; // Loss message flag.
  uint8_t              lossCounter; // Counter in loss message.
} stBuffer;

static struct // Buffer for transmitting message through BSC.
{
  uint8_t data[TRACE_MESSAGE_MAX_SIZE]; // Message buffer.
  Sbuf_t                            sb; // Message buffer descriptor.
} trBuffer;

static bool  init = FALSE; // Init flag.

static inline void endCorrect();                           // End pointer correction in storage buffer.
static void transmit();                                    // Transmits a message.
static result_t sendDone(Sbuf_t* req, BSCStatus_t status); // Transmission completion handler.

/********************************************************************************
  void endCorrect()
  
  End pointer correction in storage buffer.
********************************************************************************/
void endCorrect()
{
  stBuffer.end++;
  if (TRACE_BUFFER_SIZE <= stBuffer.end) 
    stBuffer.end = 0; 
}

/********************************************************************************
  result_t trace__init()

  Initiates BSC channel and trace module.
  Returns:
          - FAIL if there is BSC init error.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__init()
{
  BSCInitParams_t bscParams; // BSC parameters.
  
  if (!init)
  {
    // First initializing.
    init = TRUE;
    // Init storage buffer.
    stBuffer.start = 0;
    stBuffer.end = 0;
    stBuffer.full = FALSE;
    stBuffer.empty = TRUE;
    stBuffer.freeCounter = TRACE_BUFFER_SIZE;
    // Init transmitting buffer descriptor.
    trBuffer.sb.busy = FALSE;
    sbuf__buf__init(&trBuffer.sb, trBuffer.data, TRACE_MESSAGE_MAX_SIZE);
    // Init BSC.
    bsc__getAffixInfo(&bscParams.rxHeader, &bscParams.rxFooter);
    bsc__init(TRACE_SERIAL_CHANNEL, &bscParams);
    return bsc__open(TRACE_SERIAL_CHANNEL, 0);
  }
  // Initializing has already been done.
  return SUCCESS;
}

/********************************************************************************
  result_t trace__start()

  Starts message formation.
  Returns:
          - FAIL in non-init state.
          - BUSY if storage buffer is full.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__start()
{
  if (!init)
    // Non-init state. 
    return FAIL;
  // Start message formation.
  cli();//ATOMIC_SECTION_ENTER;
  if (stBuffer.full)
    // Service message is being generate.
    return BUSY;
  stBuffer.lossMessage = FALSE; // It is not a service message yet.
  // Fill length of message.
  stBuffer.startLast = stBuffer.end;
  stBuffer.data[stBuffer.startLast] = 0;
  stBuffer.freeCounter--;
  stBuffer.empty = FALSE;
  endCorrect();   // Correct end pointer.
  return SUCCESS;
}

/********************************************************************************
  result_t trace__put(uint8_t value)

  Fills storage buffer with user message.
  Returns
          - FAIL if trace is not initialized.
          - BUSY if in storage buffer instead user message loss message is forming.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__put(uint8_t value)
{
  if (!init)
    // Non-init state. 
    return FAIL;
  if (stBuffer.lossMessage)
    // Service message in buffer.
    return BUSY;
  stBuffer.freeCounter--;
  if (stBuffer.freeCounter < LOST_MESSAGE_LENGTH + 1)
  {
    // There is no room for message. We have to form
    // a service message.
    // Recover free counter and end pointer.
    stBuffer.freeCounter += (stBuffer.data[stBuffer.startLast] + 2); 
    stBuffer.end = stBuffer.startLast;
    // Put a service message.
    stBuffer.data[stBuffer.end] = LOST_MESSAGE_LENGTH;
    endCorrect();
    stBuffer.freeCounter--;
    stBuffer.data[stBuffer.end] = TRACE_MODULE;
    endCorrect();
    stBuffer.freeCounter--;
    stBuffer.data[stBuffer.end] = LOST_MESSAGE_CODE;
    endCorrect();
    stBuffer.freeCounter--;
    stBuffer.data[stBuffer.end] = 0;
    stBuffer.lossCounter = stBuffer.end;
    endCorrect();
    stBuffer.freeCounter--;
    stBuffer.lossMessage = TRUE;
    // Check buffer fullness.
    if (stBuffer.freeCounter < LOST_MESSAGE_LENGTH + 1)
      stBuffer.full = TRUE;
    return BUSY;
  }
  // Add byte to user message.
  stBuffer.data[stBuffer.end] = value;
  stBuffer.data[stBuffer.startLast]++;
  endCorrect();   // Correct end pointer.
  return SUCCESS;
}

/********************************************************************************
  result_t trace__send()

  Sends user message.
  Returns
          - FAIL if trace is not initialized.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__send()
{
  if (!init)
    // Non-init state. 
    return FAIL;
  if (stBuffer.lossMessage)
    // There is a loss message in buffer. Increase loss message counter.
    stBuffer.data[stBuffer.lossCounter]++;
  if (!trBuffer.sb.busy)
    // Transmission is possible.
    transmit();
  // Finish message formation.
  sei();//ATOMIC_SECTION_LEAVE;
  return SUCCESS;
}

/********************************************************************************
  void transmit()

  Transmits a message.
********************************************************************************/
void transmit()
{
  uint16_t start;  // Start of current message.
  uint16_t length; // Length of current message.
  uint16_t tail;   // Distance to the end of storage buffer.
    
  length = stBuffer.data[stBuffer.start];
  start = stBuffer.start + 1;
  tail = TRACE_BUFFER_SIZE - start;
  if (length <= tail)
    sbuf__data__put(&trBuffer.sb, trBuffer.sb.data, &stBuffer.data[start], length);
  else
  {
    sbuf__data__put(&trBuffer.sb, trBuffer.sb.data, &stBuffer.data[start], tail);
    sbuf__data__put(&trBuffer.sb, trBuffer.sb.data + tail, &stBuffer.data[0], length - tail);
  }
  stBuffer.start += (length + 1);
  if (stBuffer.start >= TRACE_BUFFER_SIZE)
    stBuffer.start -= TRACE_BUFFER_SIZE;
  sbuf__data__setLength(&trBuffer.sb, length); 
  trBuffer.sb.busy = TRUE;                      
  stBuffer.freeCounter += (length + 1);         
  // Check buffer emptiness.
  if (stBuffer.start == stBuffer.end)
    stBuffer.empty = TRUE;
  // Buffer has already been full.
  stBuffer.full = FALSE;
  bsc__request(TRACE_SERIAL_CHANNEL, &trBuffer.sb, sendDone); // Transmit the message. 
}

/********************************************************************************
  Transmission completion handler.
********************************************************************************/
result_t sendDone(Sbuf_t* req, BSCStatus_t status)
{
  // Clear transmittion buffer description.
  trBuffer.sb.busy = FALSE;
  sbuf__buf__setRoom(&trBuffer.sb, 0, 0);
  // Check storage buffer for the next transmittion.
  if (!stBuffer.empty)
    transmit();
  return SUCCESS;
}

/********************************************************************************
  Returns TRUE, if all messages have been transmitted and FALSE, otherwise.
********************************************************************************/
bool trace__isEmpty()
{
  return stBuffer.empty && !trBuffer.sb.busy; 
}

// eof trace.c
