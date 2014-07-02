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
  uart_avr1281.h
  
  Base UART functions for AVR 1281.

  This file contains commonly used functions for uart0, uart1, usrt0, usrt1 
  implementations and must not be used directly. Use one of listed above modules
  for USART functionality.
  
  Written by M.Balakhno
***********************************************************************/

#ifndef UART_AVR1281_H
#define UART_AVR1281_H

#include "com_types.h"
#include "uart_fxns.h"

typedef enum
{
  UART_STATE_INVALID = 0,
  UART_STATE_CLOSED,
  UART_STATE_OPENED
}UartBaseState;

typedef struct
{
  Uint8 *rxBuff;                     // pointer to RX circular buffer
  Uint16 rxHead, rxTail, rxLength;
  
  Uint8 *txBuff;                     // pointer to TX circular buffer
  Uint16 txHead, txTail, txLength;

  Uint8 flags;                       // various flags
  UartBaseState state;                   // UART hardware state
  Uint8 *baseReg;                    // pointer to UART registers group start
  Uint8 *rtsPinReg;                  // pointer to PIN register containing RTS pin
  Uint8 *ctsPortReg;                 // pointer to PORT register containing CTS pin
  Uint8 rtsPinMask;                  // RTS pin mask on its PIN register
  Uint8 ctsPortMask;                  //CTS pin mask on its PORT register
}UartBase;


void uartAvr1281_preOpen(UartBase* pUart, const UartMode *mode);
void uartAvr1281_close(UartBase* pUart);
void uartAvr1281_resumeTransmit(UartBase* pUart);
Uint16 uartAvr1281_put(UartBase* pUart, const Uint8* data, Uint16 length);
void uartAvr1281_manageCts(UartBase* pUart);
Uint16 uartAvr1281_get(UartBase* pUart, Uint8* data, Uint16 length);
void uartAvr1281_receive(UartBase* pUart);
Bool uartAvr1281_isTxEmpty(UartBase* pUart);
void uartAvr1281_process(UartBase* pUart);
void uartAvr1281_rxControl(UartBase* pUart, Bool enableRx);

#endif
