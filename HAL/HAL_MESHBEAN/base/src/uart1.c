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
  uart1.c
  
  UART1 implementation
  
  Several hardware considerations:
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
#include "uart1_cntrl_fxns.h"

#define UART1_CTS_PORT   PORTD
#define UART1_CTS_DDR    DDRD
#define UART1_CTS_PIN    PIND
#define UART1_CTS_MASK   (1<<5)

#define UART1_RTS_PORT   PORTD
#define UART1_RTS_DDR    DDRD
#define UART1_RTS_PIN    PIND
#define UART1_RTS_MASK   (1<<4)

static Uart1* pUartObject1;  // UART global object pointer

/*******************************************************************************
  Description:  Open UART hardware initializing all the registers

  Parameters:   pUart - UART object
                mode - UART mode
  
  Returns:      none.
*******************************************************************************/
static void open(Uart1* pUart, const UartMode *mode)
{
  Uint16 prescaler_config;

  uartAvr1281_preOpen(pUart, mode);

  // Configure flow control pins
  if (mode->flowCntrl.cts)
  {
    UART1_CTS_DDR |= UART1_CTS_MASK;
	UART1_CTS_PORT &= ~UART1_CTS_MASK;  // set CTS low (active) initially
  }
  if (mode->flowCntrl.rts)
  {
    UART1_RTS_DDR &= ~UART1_RTS_MASK;
	UART1_RTS_PORT |= UART1_RTS_MASK;
  }

  // Start transeiver and enable receive interrupt
  UCSR1B |= (1<<RXEN0)|(1<<TXEN0)|(1<<RXCIE0);
}

/*******************************************************************************
  Description:  Interrupt on UDRE0 (USART1 data register empty) flag handler

  Parameters:   none. 
  
  Returns:      none.
*******************************************************************************/
SIGNAL(SIG_USART1_DATA)
{
  uartAvr1281_resumeTransmit(pUartObject1);
}

/*******************************************************************************
  Description:  Interrupt on RXC0 (USART1 receive complete) flag handler

  Parameters:   none. 
  
  Returns:      none.
*******************************************************************************/
SIGNAL(SIG_USART1_RECV)
{
  uartAvr1281_receive(pUartObject1);
}

/*******************************************************************************
  Description: inits UART1.

  Parameters: pUart1     - UART1 object reference.

  Returns: nothing.
*******************************************************************************/
void uart1_init(Uart1* pUart, Uint8* pTxBuf, Uint16 txBufSize, 
                              Uint8* pRxBuf, Uint16 rxBufSize)
{
  pUartObject1 = pUart;

  // Init circular buffers
  pUartObject1->base.txBuff = pTxBuf;
  pUartObject1->base.rxBuff = pRxBuf;
  pUartObject1->base.txLength = txBufSize;
  pUartObject1->base.rxLength = rxBufSize;
  
  // Init hardware information 
  pUartObject1->base.baseReg = &UCSR1A;
  pUartObject1->base.rtsPinReg = &UART1_RTS_PIN;    // Flow control pins
  pUartObject1->base.ctsPortReg = &UART1_CTS_PORT;    //
  pUartObject1->base.rtsPinMask = UART1_RTS_MASK;   //
  pUartObject1->base.ctsPortMask = UART1_CTS_MASK;  //
    
  pUartObject1->base.state = UART_STATE_CLOSED;
}

/*******************************************************************************
  Description: provides reference to UART1 interfaces implementation.

  Parameters: pUartFxns - UART interface implemetation reference.

  Returns: nothing.
*******************************************************************************/
void uart1_getFxns(UartFxns* pUartFxns)
{
  pUartFxns->open = open;
  pUartFxns->close = uartAvr1281_close;
  pUartFxns->get = uartAvr1281_get;
  pUartFxns->put = uartAvr1281_put;
  pUartFxns->isTxEmpty = uartAvr1281_isTxEmpty;
  pUartFxns->rxControl = uartAvr1281_rxControl; //TODO: function reference
}


/*******************************************************************************
  Description: process UART inner tasks

  Parameters: pUart - UART base object reference.

  Returns: nothing.
*******************************************************************************/
void uart1_process(Uart1* pUart)
{
  uartAvr1281_process((UartBase*)pUart);
}


/*******************************************************************************
  Description: explicitly control cts signal for UART1

  Parameters: pUart1     - UART1 object reference
              enableRx   - if true, cts will be contolled by uart according to its buffer state,
                           if false, cts will unconditionally set to disable rx.

  Returns: nothing.
*******************************************************************************/
void uart1_ctsControl(Uart1* pUart1, Bool enableRx);


// eof circ_buf.h
