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

/***********************************************************************
  uart_fxns.h
  
  UART interface declaration.
  
  Written by A.Kokhonovskiy.
***********************************************************************/

#ifndef UART_FXNS_H_
#define UART_FXNS_H_

#include "com_types.h"

typedef enum
{
  UART_BAUD_RATE_INVALID =  0, 
  UART_BAUD_RATE_1200,         // 1200 baud rate
  UART_BAUD_RATE_9600,         // 9600 baud rate
  UART_BAUD_RATE_38400,        // 38400 baud rate
  UART_BAUD_RATE_57600,        // 57600 baud rate
  UART_BAUD_RATE_115200        // 115200 baud rate
} UartBaudRate;

typedef enum
{
  UART_DATA_BITS_INVALID = 0,  
  UART_DATA_BITS_5,           // 5 bits data length
  UART_DATA_BITS_6,           // 6 bits data length
  UART_DATA_BITS_7,           // 7 bits data length
  UART_DATA_BITS_8            // 8 bits data length 
} UartDataBits;

typedef enum
{
  UART_PARITY_INVALID = 0,
  UART_PARITY_NONE,        // Non parity mode
  UART_PARITY_EVEN,        // Even parity mode
  UART_PARITY_ODD          // Odd parity mode
} UartParity;

typedef enum
{
  UART_STOP_BITS_INVALID = 0, 
  UART_STOP_BITS_1,           // 1 stop bits mode
  UART_STOP_BITS_2            // 2 stop bits mode
} UartStopBits;

typedef struct
{
  Bool cts;  // CTS control is enabled
  Bool rts;  // RTS control is enabled
} UartFlowCntrl;

typedef struct
{
  UartBaudRate baudRate;   // UART baud rate
  UartDataBits dataBits;   // UART data length
  UartParity   parity;     // UART parity mode. 
  UartStopBits stopBits;   // UART stop bits number
  UartFlowCntrl flowCntrl; // 
} UartMode;

struct Uart;

typedef struct
{
  struct Uart* pUart; // UART instance pointer: this value will be passed
                      // as a first parameter to all callbacks below.


  /*****************************************************************************
    Callback description: opens UART driver.

    Callback parameters: pUart - UART instance reference.
                         pMode  - UART mode reference.

    Callback returns: nothing.
  *****************************************************************************/
  void (*open)(struct Uart* pUart, const UartMode *pMode);

  /*****************************************************************************
    Callback description: closes UART driver.

    Callback parameters: pUart - UART instance reference.

    Callback returns: nothing.
  *****************************************************************************/
  void (*close)(struct Uart* pUart);

  /*****************************************************************************
    Callback description: puts bytes sequence to UART driver to send.

    Callback parameters: pUart - UART instance reference.
                         pData  - byte sequence buffer pointer.
                         length - byte sequence length.

    Callback returns: number of byte really accepted by UART driver.
  *****************************************************************************/
  Uint16 (*put)(struct Uart* pUart, const Uint8* pData, Uint16 length);

  /*****************************************************************************
    Callback description: gets received bytes sequence from UART driver.

    Callback parameters: pUart - UART instance reference.
                         pData  - buffer pointer for received sequence.
                         length - byte sequence length.

    Callback returns: number of byte really got from UART driver.
  *****************************************************************************/
  Uint16 (*get)(struct Uart* pUart, Uint8* pData, Uint16 length);

  /*****************************************************************************
    Callback description: checks whether all UART data has been sent.

    Callback parameters: pUart - UART instance reference.

    Callback returns: true if all UART data has been sent and false, otherwise.
  *****************************************************************************/
  Bool (*isTxEmpty)(struct Uart* pUart);

/*******************************************************************************
  Description: explicitly control cts signal for UART1

  Parameters: pUart1     - UART1 object reference
              enableRx   - if true, cts will be contolled by uart according to its buffer state,
                           if false, cts will unconditionally set to disable rx.

  Returns: nothing.
*******************************************************************************/
  void (*rxControl)(struct Uart* pUart, Bool enableRx);
  
} UartFxns;

#endif

// eof uart_fxns.h
