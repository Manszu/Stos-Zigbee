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
The module of interrupts control.
=================================================================*/

#include <inttypes.h>
#include <tos.h>
#include <avrhardware.h>

/*******************************************************************************
  Description: Enables global interrupt.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
inline result_t HPLInterrupt__Interrupt__enable()
{
   __asm volatile ("sei");
  return SUCCESS;
}

/*******************************************************************************
  Description: Desables global interrupt.

  Parameters: nothing.

  Returns: the actual value of the global interrupt bit.
*******************************************************************************/
inline bool HPLInterrupt__Interrupt__disable()
{
  bool result = (SREG & 0x80) != 0;
  cli();
  return result;
}

/*******************************************************************************
  Description: The wrapper for HPLInterrupt__Interrupt__enable().

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
bool TOSH_interrupt_enable()
{
  HPLInterrupt__Interrupt__enable();
  #ifdef DEBUG_SUPERVISE_ATOMIC
  {
    uint8_t low, high;
    uint16_t time;
    
    TCCR1B=0;
    low = TCNT1L;
    high = TCNT1H;
    time = low + (high << 8);
    if (time > MAX_ATOMIC_DURATION)
    {
      // assert!!!
      DDRB |= 1<<PIN6;
      PORTB |= 1<<PIN6;
      DDRB |= 1<<PIN7;
      PORTB |= 1<<PIN7;
      for(;;);
    }
  }
  #endif
  #ifdef DEBUG_PROFILE_TOSH_INT
    DDRB |= 1<<PIN5; //red led
    PORTB &= ~(1<<PIN5);
  #endif
}

/*******************************************************************************
  Description: The wrapper for HPLInterrupt__Interrupt__disable().

  Parameters: nothing.

  Returns: the actual value of the global interrupt bit.
*******************************************************************************/
bool TOSH_interrupt_disable(void)
{
  #ifdef DEBUG_SUPERVISE_ATOMIC
    //Hardware Timer1 used to measure atomic section duration
    TCNT1H=0;
    TCNT1L=0;
    TCCR1B=1;
  #endif
  #ifdef DEBUG_PROFILE_TOSH_INT
    DDRB |= 1<<PIN5; //red led
    PORTB |= 1<<PIN5;
  #endif
  return HPLInterrupt__Interrupt__disable();
}

// eof hplinterrupt.c
