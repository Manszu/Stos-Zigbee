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
  spi_master_fxns.h
  
  SPI master interface declaration.
  
  Written by M.Balakhno.
***********************************************************************/

#ifndef SPI_MASTER_FXNS_H_
#define SPI_MASTER_FXNS_H_

#include "com_types.h"

typedef enum
{
  SPI_CLOCK_RATE_INVALID =  0, // invalid clock rate
  SPI_CLOCK_RATE_125,          // 125 Kb/s
  SPI_CLOCK_RATE_250,          // 250 Kb/s
  SPI_CLOCK_RATE_500,          // 500 Kb/s
  SPI_CLOCK_RATE_1000,          // 1 Mb/s
  SPI_CLOCK_RATE_2000,         // 2 Mb/s
  SPI_CLOCK_RATE_4000          // 4 Mb/s
} SpiMasterClockRate;

typedef enum
{
  SPI_CLOCK_MODE_INVALID = 0,  // 
  SPI_CLOCK_MODE_0,            // leadin edge - sample RX bit (rising), trailing edge - setup TX bit (falling)
  SPI_CLOCK_MODE_1,            // leadin edge - setup TX bit (rising), trailing edge - sample RX bit (falling)
  SPI_CLOCK_MODE_2,            // leadin edge - sample RX bit (falling), trailing edge - setup TX bit (rising)
  SPI_CLOCK_MODE_3,            // leadin edge - setup TX bit (falling), trailing edge - sample RX bit (rising)
} SpiMasterClockMode;

typedef enum
{
  SPI_DATA_ORDER_INVALID = 0,
  SPI_DATA_ORDER_LSB,          // LSB first
  SPI_DATA_ORDER_MSB,          // MSB first
} SpiMasterDataOrder;

typedef struct
{
  SpiMasterClockRate clockRate;      // SPI clock rate
  SpiMasterClockMode clockMode;      // UART data length
  SpiMasterDataOrder dataOrder;      // UART parity mode. 
} SpiMasterMode;

struct SpiMaster;

typedef struct
{
  struct SpiMaster* pSpiMaster;

  void (*open)(struct SpiMaster* pSpiMaster, const SpiMasterMode *mode);
  void (*close)(struct SpiMaster* pSpiMaster);
  Uint16 (*exchange)(struct SpiMaster* pSpiMaster, Uint8* data, Uint16 length);
} SpiMasterFxns;

#endif

// eof spi_master_fxns.h
