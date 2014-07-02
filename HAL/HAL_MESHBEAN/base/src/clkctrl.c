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

/*===============================================================
The main clock control.
=================================================================*/

#include <clkctrl.h>
#include <avrhardware.h>
#include <hardware.h>
#include <tos.h>
#include <calibration.h>
#include <at86rf230.h>
#include <hplspi.h>

#ifdef HAL_RC_OSCILLATOR
void calibrateMainClock()
{
  calibrateInternalRc();
}
#else
void calibrateMainClock()
{
  ;
}
#endif

/*******************************************************************************
  Description: Inits system clock.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
inline void hpl__init__freq()
{
#ifdef HAL_RC_OSCILLATOR
  CLKPR = 1 << CLKPCE;
  CLKPR = 1 << CLKPS0;
  ASSR = (1 << AS2);
#else
uint8_t buffer[1];
    buffer[0] = 0;
    (*((RegTrxCtrl0 *)buffer)).pad_io = AT86RF230_PAD_IO_2MA;
    (*((RegTrxCtrl0 *)buffer)).pad_io_clkm = AT86RF230_PAD_IO_CLK_4MA;
    (*((RegTrxCtrl0 *)buffer)).clkm_sha_sel = AT86RF230_CLKM_SHA_SEL_DISABLE;
    (*((RegTrxCtrl0 *)buffer)).clkm_ctrl = AT86RF230_CLK_CTRL_4MHZ;
    HPLSPIM__HPLSPI__clrCS();    
      HPLSPIM__HPLSPI__write( (AT86RF230_TRX_CTRL_0_REG & AT86RF230_RADDRM) | AT86RF230_CMD_RW );
      HPLSPIM__HPLSPI__write( buffer[0] );
    HPLSPIM__HPLSPI__setCS();
#endif
}

/*******************************************************************************
  Description: System clock.

  Parameters: nothing.

  Returns: system clock in Hz.
*******************************************************************************/
inline uint32_t hpl__get__freq()
{
  return F_CPU;
}

// eof clkctrl.c
