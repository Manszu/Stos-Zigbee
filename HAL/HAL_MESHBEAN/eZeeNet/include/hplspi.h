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

/*******************************************************************************
  hplspi.h

  SPI for AT86RF230 radio - header file.
*******************************************************************************/

#ifndef HPLSPI_H_
#define HPLSPI_H_

#include "avrhardware.h"
#include "hardware.h"
#include "tos.h"

TOSH_ASSIGN_PIN(SPI_CSN, B, 0);        // CSN output SPI interface.
TOSH_ASSIGN_PIN(SPI_SCLK, B, 1);
TOSH_ASSIGN_PIN(SPI_MOSI, B, 2);
TOSH_ASSIGN_PIN(SPI_MISO, B, 3);

/*******************************************************************************
  Description: Performs initialization of SPI interface.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__init();

/*******************************************************************************
  Description: Deselects a slave device.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__setCS();

/*******************************************************************************
  Description: Selects a slave device.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__clrCS();

/*******************************************************************************
  Description: Writes a byte to SPI. Reads a byte from a slave.

  Parameters: none.
 
  Returns: a delivered byte.
*******************************************************************************/
uint8_t HPLSPIM__HPLSPI__write(uint8_t value);

/*******************************************************************************
  Description:  Reads out a block of bytes from SPI. 

  Parameters:   value       - pointer to the incomminf data's buffer.
                frameLength - number of bytes to read.
 
  Returns: none.
*******************************************************************************/
void HPLSPIM__HPLSPI__readBlock(uint8_t* value, uint8_t frameLength);

/*******************************************************************************
  Description:  Writes a block of bytes to SPI.
 
  Parameters:   value - pointer to the data to write,
                value[0] - block's length.
                
  Returns:      none.
*******************************************************************************/
void HPLSPIM__HPLSPI__writeBlock(uint8_t* value);


typedef struct        // Parameters of registers operation. 
{
  uint8_t addr;       // Base address of registers.
  uint8_t *data;     // Values of registers.
} SPIRegistersParams;

typedef struct    // Parameters of farme buffer operation. 
{
  uint8_t *data;  // Buffer of bytes.
  uint8_t length; // Number of bytes.
} SPIFrameParams;

typedef struct    // Parameters of SRAM operation. 
{
  uint8_t addr;  // Base address.
  uint8_t *data;  // Buffer of bytes.
  uint8_t length; // Number of bytes.
} SPISRAMParams;

enum // Constants. 
{
  AT86RF230_CMD_RW = ((1<<7) | (1<<6)),
  AT86RF230_CMD_RR = ((1<<7) | (0<<6)),
  AT86RF230_CMD_FW = ((0<<7) | (1<<6) | (1<<5)),
  AT86RF230_CMD_FR = ((0<<7) | (0<<6) | (1<<5)),
  AT86RF230_CMD_SW = ((0<<7) | (1<<6) | (0<<5)),
  AT86RF230_CMD_SR = ((0<<7) | (0<<6) | (0<<5)),
  
  AT86RF230_RADDRM = ((0<<7) | (0<<6) | (1<<5) | (1<<4) | (1<<3) | (1<<2) |(1<<1) | (1<<0)),
};

#endif /* HPLSPI_H_ */
                         
