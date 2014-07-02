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
The module counts out requested interval of application timer.
=================================================================*/

#include <appclock.h>
#include <avrhardware.h>
#include <atomic.h>
#include <clkctrl.h>

uint8_t appClock__counter; // Stores the value which will be counted out
uint8_t appClock__cnt;     // Contains a counted out interval

/*******************************************************************************
  Description: Stops the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__stop()
{
  TCCR4B = 0; // stop the timer
}

/*******************************************************************************
  Description: Starts the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__start()
{
  TCCR4B = (1 << WGM12) | (1 << CS11); // CTC, main clk / 8
}

/*******************************************************************************
  Description: Sets a counted out interval.

  Parameters: value - contains the interval.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__setInterval(uint8_t value)
{
  TIMSK4 &= ~(1 << OCIE4A); // Disable TC4 interrupt
  appClock__counter = value;
  appClock__cnt = 0;
  TIMSK4 |= (1 << OCIE4A); // Enable TC4 interrupt
}

/*******************************************************************************
  Description: Returns a current value of the interval.

  Parameters: nothing.

  Returns: current value of the interval.
*******************************************************************************/
uint8_t appClock__Clock__readCounter(void)
{
  return appClock__cnt;
}

/*******************************************************************************
  Description: Sets a count out interval. Performs init settings.

  Parameters: interval - contains the interval.

  Returns: SUCCESS.
*******************************************************************************/
result_t appClock__Clock__setRate(char interval)
{
  ATOMIC_SECTION_ENTER
    TCCR4B = 0; // stop the timer
    appClock__counter = interval;
    appClock__cnt = 0;
    TIMSK4 &= ~((1 <<  OCIE4A) | (1 << TOIE4)); // Disable TC4 interrupt
    OCR4A = ( F_CPU/1000ul ) / 8ul; // 1 millisecond timer interrupt interval.
    TCNT4 = 0;
    TCCR4B = (1 << WGM12) | (1 << CS11); // CTC, main clk / 8
    TIMSK4 |= (1 << OCIE4A); // Enable TC4 interrupt
  ATOMIC_SECTION_LEAVE
  return SUCCESS;
}

/*******************************************************************************
  Description: Interrupt handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
SIGNAL(SIG_OUTPUT_COMPARE4A)
{
  if(++appClock__cnt >= appClock__counter)
  {
    #ifdef DEBUG_PROFILE_APPCLOCK_ISR
      DDRB |= 1<<PIN5;    //red led
      PORTB |= 1<<PIN5;
    #endif
    appClock__cnt = 0;
    appClock__Clock__HandleFire();
    #ifdef DEBUG_PROFILE_APPCLOCK_ISR
      PORTB &= ~(1<<PIN5);
    #endif
  }
}

// eof appclock.c
