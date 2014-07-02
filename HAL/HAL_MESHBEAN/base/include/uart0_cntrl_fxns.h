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
  uart0_cntrl_fxns.h
  
  UART0 control interface declaration for ATMega1281.
  
  Notes on using module:
  - To specify oscillator frequency define FOSC_4MHZ or FOSC_8MHZ. If nor FOSC_4MHZ 
    nor FOSC_8MHZ is defined, frequency is assumed to be 4MHz.
  - UART0 doesn't have CTS and RTS pinouts, so enabling any flow control feature will 
    lead to ASSERT.
  - Not all the baudrates are available on different oscillator frequences. Some actual rates
    would differ too much from desired (more than 3%), so these baudrates are disabled.
    Specifying a disabled baudrate in *open* function will lead to ASSERT.  
  - Default buffer size for incoming and outgoing data is 128 bytes. If it must be changed, 
    define UART0_RX_BUFF_SIZE and UART0_TX_BUFF_SIZE prior to including this file.
 
    Written by M.Balakhno
***********************************************************************/

#ifndef UART0_CNTRL_FXNS_H_
#define UART0_CNTRL_FXNS_H_

#include "uart_fxns.h"
#include "uart_avr1281.h"


#ifndef UART0_RX_BUFF_SIZE
#define UART0_RX_BUFF_SIZE  128
#endif

#ifndef UART0_TX_BUFF_SIZE
#define UART0_TX_BUFF_SIZE  128
#endif

typedef struct
{
  UartBase base;

  Uint8 rxBuff[UART0_RX_BUFF_SIZE];  // RX circular buffer
  Uint8 txBuff[UART0_TX_BUFF_SIZE];  // TX circular buffer
}Uart0;

/*******************************************************************************
  Description: provides reference to UART0 interfaces implementation.

  Parameters: pUartFxns - UART interface implemetation reference.

  Returns: nothing.
*******************************************************************************/
void uart0_getFxns(UartFxns* pUartFxns);

/*******************************************************************************
  Description: inits UART0.

  Parameters: pUart0     - UART0 object reference.

  Returns: nothing.
*******************************************************************************/
void uart0_init(Uart0* pUart0);

/*******************************************************************************
  Description: process UART0 inner tasks

  Parameters: pUart0     - UART0 object reference.

  Returns: nothing.
*******************************************************************************/
void uart0_process(Uart0* pUart0);

#endif

// eof uart0_cntrl_fxns.h
