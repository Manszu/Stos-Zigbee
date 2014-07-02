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
  The module to control the power on periphery
****************************************************************/

#include <tos.h>
#include <pwrctrl.h>
#include <hplat86rf230m.h>

uint8_t power_control = 0;

/*******************************************************************************
  Description:  Powers on periphery.

  Parameters:   id.

  Returns:      none.
*******************************************************************************/
void hpl__peripherial__on( uint8_t id )
{
  if( !power_control )
  { 
#ifndef WDM_A1281_P1
	 TOSH_MAKE_SHDN_OUTPUT();
	 TOSH_CLR_SHDN_PIN();
#endif
   TOSH_MAKE_BTM_OUTPUT();
   
   #ifdef WDB_A1281_PN
     TOSH_CLR_BTM_PIN();
   #else
     TOSH_SET_BTM_PIN();
   #endif
   
   TOSH_MAKE_VTE_OUTPUT();
   TOSH_SET_VTE_PIN();
  }
  power_control |= (1 << id);
}

/*******************************************************************************
  Description:  Powers off periphery.

  Parameters:   id.

  Returns:      none.
*******************************************************************************/
void hpl__peripherial__off( uint8_t id )
{
  power_control &= ~(1 << id);
  if( power_control ) return;
#ifndef WDM_A1281_P1
	TOSH_MAKE_SHDN_OUTPUT();
	TOSH_SET_SHDN_PIN();
#endif
  TOSH_MAKE_BTM_OUTPUT();
  TOSH_CLR_BTM_PIN();
  TOSH_MAKE_VTE_OUTPUT();
  TOSH_CLR_VTE_PIN();
}
// eof hplpower.c
