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

/****************************************************************
  The implementation of SPI module.
****************************************************************/

#include <hplspi.h>
#include <atomic.h>
#include <avr/interrupt.h>
#include <avrhardware.h>
#include <hardware.h>
#include <tos.h>

/*******************************************************************************
  Description: Performs initialization of SPI interface.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__init()
{
  TOSH_MAKE_SPI_MISO_INPUT();
  TOSH_MAKE_SPI_SCLK_OUTPUT();
  TOSH_MAKE_SPI_MOSI_OUTPUT();
  TOSH_MAKE_SPI_CSN_OUTPUT();
  SPCR = ((1 << SPE) | (1 << MSTR)); // SPI enable, master mode.
  SPSR = (1 << SPI2X); // rate = fosc/2
}

/*******************************************************************************
  Description: Deselects a slave device.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__setCS()
{
  TOSH_SET_SPI_CSN_PIN();
}

/*******************************************************************************
  Description: Selects a slave device.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__clrCS()
{
  TOSH_CLR_SPI_CSN_PIN();
}

/*******************************************************************************
  Description: Writes a byte to SPI. Reads a byte from a slave.

  Parameters: none.
 
  Returns: a delivered byte.
*******************************************************************************/
uint8_t HPLSPIM__HPLSPI__write(uint8_t value)
{
  SPDR = value; // Write data.
  while( !(SPSR&(1 << SPIF)) );
  return SPDR;
}

/*******************************************************************************
  Description:  Writes a block of bytes to SPI.
 
  Parameters:   value - pointer to the data to write,
                value[0] - block's length.
                
  Returns:      none.
*******************************************************************************/
void HPLSPIM__HPLSPI__writeBlock(uint8_t* value)
{
  uint8_t i;

  for(i = 0; i < value[0] - 1; i++)
  {// Without 2 bytes of CRC but with one byte of the length field.
    SPDR = value[i]; // Write data.
    while( !(SPSR&(1 << SPIF)) );
  }
}

/*******************************************************************************
  Description:  Reads out a block of bytes from SPI. 

  Parameters:   value       - pointer to the incomminf data's buffer.
                frameLength - number of bytes to read.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__readBlock(uint8_t* value, uint8_t frameLength)
{
  uint8_t i;

  for(i = 0; i < frameLength; i++)
  {
    SPDR = 0x55; // Just something.
    while( !(SPSR&(1 << SPIF)) );
    value[i] = SPDR; // Write data.
  }
}

// eof hplspi.c
