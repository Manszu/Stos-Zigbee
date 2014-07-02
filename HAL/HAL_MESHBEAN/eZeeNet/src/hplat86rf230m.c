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
hplat86fr230m.c

The implementation of access to at86rf230.
*******************************************************************************/

#include <tos.h>
#include <hplat86rf230m.h>
#include <hardware.h>
#include <atomic.h>
#include <at86rf230.h>
#include <hplspi.h>

uint8_t HPLAT86RF230C__descriptor = 0; // descriptor
bool HPLAT86RF230C__init = FALSE;
void (*HPLAT86RF230Cirq)();            //callback pointer

/*******************************************************************************
  Description:  reset module

  Parameters: none.
 
  Returns:    none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__reset()
{
uint8_t i;
    TOSH_CLR_TRSLEEP_PIN();
    TOSH_MAKE_TRSLEEP_OUTPUT();

    HPLAT86RF230C__HPLAT86RF230__clrRST();
    TOSH_MAKE_RSTIN_OUTPUT();
    for(i = 0; i < 8 ; i++)
      __delay_us(250);
    HPLAT86RF230C__HPLAT86RF230__setRST();

    i = 0;
    (*((RegTrxState *)(&i))).trx_cmd = AT86RF230_TRX_CMD_TRX_OFF;
    HPLSPIM__HPLSPI__init();
    HPLSPIM__HPLSPI__clrCS();
      HPLSPIM__HPLSPI__write( (AT86RF230_TRX_STATE_REG & AT86RF230_RADDRM) | AT86RF230_CMD_RW);
      HPLSPIM__HPLSPI__write( i );
    HPLSPIM__HPLSPI__setCS();
}

/*******************************************************************************
  Description: Makes RST and SLP_TR pins as outputs. Setups irq parameters.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__init()
{
  if( HPLAT86RF230C__init == FALSE)
  {
    HPLAT86RF230C__init = TRUE;    
    TOSH_MAKE_RFIRQ_INPUT();
    
    EICRB |= (1 << ISC51) | (1 << ISC50); // rising edge
    EIFR = 1 << INTF5; // clear interrupt flag
  }
  
  #ifdef ZDM_A1281_PN
    // ZigBit with amplifier.
    DDRC  |= 1<<PIN1;
    PORTC |= 1<<PIN1;
  #endif
}

/*******************************************************************************
  Description:  Sets irq handler.

  Parameters:   irq - callback
 
  Returns:      FAIL - there is previous open call,  SUCCESS - otherwise.
*******************************************************************************/
result_t HPLAT86RF230C__HPLAT86RF230__open(void (*irq)())
{
    if(HPLAT86RF230C__descriptor || !irq)
    { 
      return FAIL;
    }
    HPLAT86RF230C__HPLAT86RF230__reset();
    HPLAT86RF230C__HPLAT86RF230__init();
    HPLAT86RF230C__descriptor = 1;
    HPLAT86RF230Cirq = irq;
  return SUCCESS;
}

/*******************************************************************************
  Description:  Cancels irq handler.

  Parameters: none.
 
  Returns:      FAIL - there is no previous open call, SUCCESS - otherwise.
*******************************************************************************/
result_t HPLAT86RF230C__HPLAT86RF230__close()
{
  if(HPLAT86RF230C__descriptor)
  {
    ATOMIC_SECTION_ENTER
    {
      HPLAT86RF230C__HPLAT86RF230__irqDisable();
      HPLAT86RF230C__descriptor = 0;
    }
    ATOMIC_SECTION_LEAVE
    return SUCCESS; 
  }
  return FAIL;  
}

/*******************************************************************************
  Description:  Enables the irq.

  Parameters:   none.
 
  Returns:      FAIL - there is no previous open call, SUCCESS - otherwise.
*******************************************************************************/
result_t HPLAT86RF230C__HPLAT86RF230__irqEnable()
{
  if( HPLAT86RF230C__descriptor )
  { 
    EIMSK |= 1 << INT5;
   return SUCCESS;
  }
  return FAIL;
}

/*******************************************************************************
  Description: Disables the irq.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__irqDisable()
{
  EIMSK &= ~(1 << INT5);
}

/*******************************************************************************
  Description: Sets RST pin to 1.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__setRST()
{
  TOSH_SET_RSTIN_PIN();
}

/*******************************************************************************
  Description: Sets RST pin to 0.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__clrRST()
{
  TOSH_CLR_RSTIN_PIN();
}

/*******************************************************************************
  Description: Sets SLP_TR pin to 1.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__setSLPTR()
{
  TOSH_SET_TRSLEEP_PIN();
}

/*******************************************************************************
  Description: Sets SLP_TR pin to 0.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void HPLAT86RF230C__HPLAT86RF230__clrSLPTR()
{
  TOSH_CLR_TRSLEEP_PIN();
}

/*******************************************************************************
  Description: Interrupt handler.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
SIGNAL(SIG_INTERRUPT5)
{
  if(HPLAT86RF230Cirq && HPLAT86RF230C__descriptor)
  {
    #ifdef DEBUG_PROFILE_RF230_ISR
      DDRB |= 1<<PIN7;    //red led
      PORTB |= 1<<PIN7;
    #endif
    HPLAT86RF230Cirq();
    #ifdef  DEBUG_PROFILE_RF230_ISR
      PORTB &= ~(1<<PIN7);
    #endif
  }
}

//endof hplat86rf230m.c
