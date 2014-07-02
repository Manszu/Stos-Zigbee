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
  uart0.c
  
  UART0 implementation
  
  Several hardware considerations:
  - UART0 doesn't have CTS and RTS pinouts, so enabling any flow control feature will 
    lead to ASSERT.
  - Not all the baudrates are available on different oscillator frequences. Some actual rates
    would differ too much from desired (more than 3%), so these baudrates are disabled.
    Specifying a disabled baudrate in *open* function will lead to ASSERT.
  
  Written by M.Balakhno
***********************************************************************/

#ifndef __AVR_ATmega1281__
#define __AVR_ATmega1281__
#endif

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <atomic.h>
#include "com_assert.h"

#include "uart_fxns.h"
#include "uart0_cntrl_fxns.h"


static Uart0* pUartObject0;  // UART global object pointer

/*******************************************************************************
  Description:  Open UART hardware initializing all the registers

  Parameters:   pUart - UART object
                mode - UART mode
  
  Returns:      none.
*******************************************************************************/
static void open(Uart0* pUart, const UartMode *mode)
{
  Uint16 prescaler_config;

  // UART0 doesn't have CTS and RTS pinouts, so this features should not be used
  ASSERT( (!mode->flowCntrl.rtsCntrl) && (!mode->flowCntrl.ctsCntrl) );

  uartAvr1281_preOpen(pUart, mode);

  // Start transeiver and enable receive interrupt
  UCSR0B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}

/*******************************************************************************
  Description:  Interrupt on UDRE0 (USART0 data register empty) flag handler

  Parameters:   none. 
  
  Returns:      none.
*******************************************************************************/
SIGNAL(SIG_USART0_DATA)
{
  uartAvr1281_resumeTransmit(pUartObject0);
}

/*******************************************************************************
  Description:  Interrupt on RXC0 (USART0 receive complete) flag handler

  Parameters:   none. 
  
  Returns:      none.
*******************************************************************************/
SIGNAL(SIG_USART0_RECV)
{
  uartAvr1281_receive(pUartObject0);
}

/*******************************************************************************
  Description: inits UART0.

  Parameters: pUart0     - UART0 object reference.

  Returns: nothing.
*******************************************************************************/
void uart0_init(Uart0* pUart)
{
  pUartObject0 = pUart;

  // Init circular buffers
  pUartObject0->base.rxBuff = pUartObject0->rxBuff;
  pUartObject0->base.txBuff = pUartObject0->txBuff;
  pUartObject0->base.txLength = UART0_RX_BUFF_SIZE;
  pUartObject0->base.rxLength = UART0_TX_BUFF_SIZE;
  
  // Init hardware information 
  pUartObject0->base.baseReg = &UCSR0A;
  pUartObject0->base.rtsPinReg = 0;       // Flow control is not supported on UART0
  pUartObject0->base.ctsPortReg = 0;      //
  pUartObject0->base.rtsPinMask = 0;      //
  pUartObject0->base.ctsPortMask = 0;     //
    
  pUartObject0->base.state = UART_STATE_CLOSED;
}

/*******************************************************************************
  Description: provides reference to UART0 interfaces implementation.

  Parameters: pUartFxns - UART interface implemetation reference.

  Returns: nothing.
*******************************************************************************/
void uart0_getFxns(UartFxns* pUartFxns)
{
  pUartFxns->open = open;
  pUartFxns->close = uartAvr1281_close;
  pUartFxns->get = uartAvr1281_get;
  pUartFxns->put = uartAvr1281_put;
  pUartFxns->isTxEmpty = uartAvr1281_isTxEmpty;
}


/*******************************************************************************
  Description: process UART inner tasks

  Parameters: pUart - UART base object reference.

  Returns: nothing.
*******************************************************************************/
void uart0_process(Uart0* pUart)
{
  uartAvr1281_process(pUart);
}



// eof circ_buf.h
