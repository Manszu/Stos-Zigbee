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
  uart0spi_cntrl_fxns.h
  
  UART0 in SPI mode control interface declaration for ATMega1281.
  
  Notes on using module:
  - To specify oscillator frequency define UART_4MHZ or UART_8MHZ. If nor UART_4MHZ 
    nor UART_8MHZ is defined, frequency is assumed to be 4MHz.
  - Not all the baudrates are available on different oscillator frequences. 
    Specifying a disabled baudrate in *open* function will lead to ASSERT.  
 
    Written by M.Balakhno
***********************************************************************/

#ifndef UART0SPI_CNTRL_FXNS_H_
#define UART0SPI_CNTRL_FXNS_H_

#include "spi_master_fxns.h"

typedef enum
{
  UART0_SPI_STATE_INVALID = 0, 
  UART0_SPI_STATE_OPENED,
  UART0_SPI_STATE_CLOSED,
}Uart0SpiState;

typedef struct
{
  Uart0SpiState state;

}Uart0Spi;

/*******************************************************************************
  Description: provides reference to UART0 SPI interfaces implementation.

  Parameters: pSpiMasterFxns - SPI Master interface implemetation reference.

  Returns: nothing.
*******************************************************************************/
void uart0spi_getFxns(SpiMasterFxns* pSpiMasterFxns);

/*******************************************************************************
  Description: inits UART0 SPI. 

  Parameters: pUart0Spi     - Uart0Spi object pointer.

  Returns: nothing.
*******************************************************************************/
void uart0spi_init(Uart0Spi* pUart0Spi);

#endif

// eof uart0spi_cntrl_fxns.h
