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
  uart0spi.c
  
  UART0 SPI implementation
  
  Notes on using module:
  - To specify oscillator frequency define UART_4MHZ or UART_8MHZ. If nor UART_4MHZ 
    nor UART_8MHZ is defined, frequency is assumed to be 4MHz.
  - Not all the baudrates are available on different oscillator frequences. 
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

#include "spi_master_fxns.h"
#include "uart0spi_cntrl_fxns.h"

// Setup values for 8MHz oscillator (it shoud be decreased by 1 to get value for UBRR register, )
#ifdef FOSC_8MHZ 

#define UART_SPI_PRESC_INVALID  0                   
#define UART_SPI_PRESC_125      32                  
#define UART_SPI_PRESC_250      16                  
#define UART_SPI_PRESC_500      8                   
#define UART_SPI_PRESC_1000     4
#define UART_SPI_PRESC_2000     2
#define UART_SPI_PRESC_4000     1

// Setup values for 4MHz oscillator
#else

#define UART_SPI_PRESC_INVALID  0                   
#define UART_SPI_PRESC_125      16                  
#define UART_SPI_PRESC_250      8                  
#define UART_SPI_PRESC_500      4                   
#define UART_SPI_PRESC_1000     2
#define UART_SPI_PRESC_2000     1
#define UART_SPI_PRESC_4000     0

#endif


static Uart0Spi* pUartSpiObject0;  // UART SPI global object pointer


static const Uint8 uartSpiPrescalers[] PROGMEM =   // prescale values for selected FOSC
{
  UART_SPI_PRESC_INVALID,
  UART_SPI_PRESC_125,
  UART_SPI_PRESC_250,
  UART_SPI_PRESC_500,
  UART_SPI_PRESC_1000,
  UART_SPI_PRESC_2000,
  UART_SPI_PRESC_4000
}; 




/*******************************************************************************
  Description:  Open UART SPI hardware initializing all the registers

  Parameters:   pUartSpi - UART SPI object
                mode - SPI mode
  
  Returns:      none.
*******************************************************************************/
static void open(Uart0Spi* pUartSpi, const SpiMasterMode *mode)
{
  Uint8 prescaler_config;
  
  ASSERT(0 != pUartSpi);
  ASSERT(0 != mode);
  ASSERT(UART0_SPI_STATE_CLOSED == pUartSpi->state);
  ASSERT( (SPI_CLOCK_RATE_INVALID < mode->clockRate) && (SPI_CLOCK_RATE_4000 >= mode->clockRate) );
  ASSERT( (SPI_CLOCK_MODE_INVALID < mode->clockMode) && (SPI_CLOCK_MODE_3 >= mode->clockMode));
  ASSERT( (SPI_DATA_ORDER_LSB == mode->dataOrder) || (SPI_DATA_ORDER_MSB == mode->dataOrder) );

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
  UBRR0 = 0;

  // Read pre-compiled prescaler value from program memory
  memcpy_P(&prescaler_config, &uartSpiPrescalers[ mode->clockRate ], sizeof(Uint8));    

  ASSERT(prescaler_config);
  
  // Init UART0 in SPI master mode
  DDRE |= (1<<2);  // XCK0 bit in E port
  UCSR0C = (1<<UMSEL01)|(1<<UMSEL00);
  
  if (SPI_DATA_ORDER_LSB == mode->dataOrder)
  {
    UCSR0C |= (1<<UCSZ01);  
  }
  
  // leadin edge - sample (rising), trailing edge - setup (falling). Phase = 0, polarity = 0
  if (SPI_CLOCK_MODE_0 == mode->clockMode)
  {
  }
  // leadin edge - setup (rising), trailing edge - sample (falling). Phase = 1, polarity = 0
  else if (SPI_CLOCK_MODE_1 == mode->clockMode)
  {
    UCSR0C |= (1<<UCSZ00);
  }
  // leadin edge - sample (falling), trailing edge - setup (rising). Phase = 0, polarity = 1
  else if (SPI_CLOCK_MODE_2 == mode->clockMode)
  {
    UCSR0C |= (1<<UCPOL0);
  }
  // leadin edge - setup (falling), trailing edge - sample (rising). Phase = 1, polarity = 1
  else if (SPI_CLOCK_MODE_3 == mode->clockMode)
  {
    UCSR0C |= (1<<UCSZ00)|(1<<UCPOL0);
  }

  // enable receiver and transmitter
  UCSR0B = (1<<RXEN0)|(1<<TXEN0);
  
  // set baudrate
  UBRR0 = prescaler_config-1;
}


/*******************************************************************************
  Description:  Close UART SPI hardware

  Parameters:   pUartSpi - UART SPI object
  
  Returns:      none.
*******************************************************************************/
static void close(Uart0Spi* pUartSpi)
{
  ASSERT(0 != pUartSpi);
  ASSERT(UART0_SPI_STATE_OPENED == pUartSpi->state);

  UCSR0A = 0;
  UCSR0B = 0;
  UCSR0C = 0;
}


/*******************************************************************************
  Description:  Do data exchange over UART SPI.
                *length* bytes is send ove SPI and *length* bytes is received.
                Received bytes are placed into *data* buffer.

  Parameters:   pUartSpi - UART SPI object
                data - data buffer provided by user
                length - data length
  
  Returns:      Actual bytes read.
*******************************************************************************/
static Uint16 exchange(Uart0Spi* pUartSpi, Uint8* data, Uint16 length)
{
  Uint16 res = 0;
  
  while (res < length)
  {
    // wait for empty transmit buffer
    while ( !( UCSR0A & (1<<UDRE0)) );
  
    UDR0 = *data;
    
    // Wait for byte to receive
    while ( !(UCSR0A & (1<<RXC0)) );
    
    *(data++) = UDR0;

	res++;
  }

  return res;
}



/*******************************************************************************
  Description: inits UART0 SPI.

  Parameters: pUartSpi - UART SPI object

  Returns: nothing.
*******************************************************************************/
void uart0spi_init(Uart0Spi* pUartSpi)
{
  pUartSpiObject0 = pUartSpi;

  pUartSpiObject0->state = UART0_SPI_STATE_CLOSED;
}

/*******************************************************************************
  Description: provides reference to UART0 interfaces implementation.

  Parameters: pUartFxns - UART interface implemetation reference.

  Returns: nothing.
*******************************************************************************/
void uart0spi_getFxns(SpiMasterFxns* pSpiMasterFxns)
{
  pSpiMasterFxns->open = open;
  pSpiMasterFxns->close = close;
  pSpiMasterFxns->exchange = exchange;
}




// eof circ_buf.h
