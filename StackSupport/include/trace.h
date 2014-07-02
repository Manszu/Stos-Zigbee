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
  Trace functions declaration.
********************************************************************************/

#ifndef _TRACE_H
#define _TRACE_H

#include "tos.h"
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

/********************************************************************************
  result_t trace__init()

  Initiates BSC channel and trace module.
  Returns:
          - FAIL if there is BSC init error.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__init();

/********************************************************************************
  result_t trace__start()

  Starts message formation.
  Returns:
          - FAIL in non-init state.
          - BUSY if storage buffer is full.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__start();

/********************************************************************************
  result_t trace__put(uint8_t value)

  Fills storage buffer with user message.
  Returns
          - FAIL if trace is not initialized.
          - BUSY if in storage buffer instead user message loss message is forming.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__put(uint8_t value);

/********************************************************************************
  result_t trace__send()

  Sends user message.
  Returns
          - FAIL if trace is not initialized.
          - SUCCESS otherwise.
********************************************************************************/
result_t trace__send();

/********************************************************************************
  Returns TRUE, if all messages have been transmitted and FALSE, otherwise.
********************************************************************************/
bool trace__isEmpty();

#endif

// eof trace.h
