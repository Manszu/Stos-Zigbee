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
hplclock.c

The module counts out requested interval of timerM.
*******************************************************************************/

#include <hplclock.h>
#include <atomic.h>

//uint8_t HPLClock__set_flag;
uint8_t HPLClock__mscale;
//uint8_t HPLClock__nextScale;
uint8_t HPLClock__minterval; // Contains minimal interval
uint8_t HPLClock__counter;   // Stores the value which will be counted out
uint8_t HPLClock__cnt;       // Contains a counted out interval

// event hanndler
void TimerM__Clock__fire();

/*******************************************************************************
  Description: Sets a interval.

  Parameters:  value - contains the interval.
 
  Returns: none.
*******************************************************************************/
void HPLClock__Clock__setInterval(uint8_t value)
{
  TIMSK5 &= (1 << OCIE5A); // Disable TC5 interrupt
  HPLClock__counter = value;
  HPLClock__cnt = 0;
  TIMSK5 |= (1 << OCIE5A); // Enable TC5 interrupt
}

/*******************************************************************************
  Description: Returns a current value of the interval

  Parameters:  none.
 
  Returns:     interval.
*******************************************************************************/
uint8_t HPLClock__Clock__readCounter(void)
{
  return HPLClock__cnt;
}

/*******************************************************************************
  Description:  Sets a count out interval. Performs init settings.

  Parameters:   interval - contains the interval.
 
  Returns:      SUCCESS - always.
*******************************************************************************/
result_t HPLClock__Clock__setRate(char interval, char scale)
{
  /*scale &= 0x7;*/
  /*scale |= 0x8;*/
  ATOMIC_SECTION_ENTER
    TCCR5B = 0; // stop the timer
    HPLClock__counter = interval;
    HPLClock__cnt = 0;
    TIMSK5 &= ~((1 << OCIE5A) | (1 <<  TOIE5)); // Disable TC5 interrupt
    OCR5A = 500; // 1 millisecond timer interrupt interval.
    TCNT5 = 0;
    TCCR5B = (1 << WGM12) | (1 << CS11); // CTC, main clk / 8
    TIMSK5 |= (1 << OCIE5A); // Enable TC5 interrupt
  ATOMIC_SECTION_LEAVE
return SUCCESS;
}

/*******************************************************************************
  Description: Initialization

  Parameters: none.
 
  Returns: SUCCESS - always. 
*******************************************************************************/
result_t HPLClock__StdControl__init(void)
{
  ATOMIC_SECTION_ENTER
    HPLClock__mscale = 0x02;
    HPLClock__minterval = 64;
  ATOMIC_SECTION_LEAVE
return SUCCESS;
}

/*******************************************************************************
  Description: Starts hardware timer.

  Parameters: none.
 
  Returns: SUCCESS - always. 
*******************************************************************************/
result_t HPLClock__StdControl__start(void)
{
  HPLClock__Clock__setRate(HPLClock__minterval,  HPLClock__mscale);
  return SUCCESS;
}

/*******************************************************************************
  Description: Sets default interval.

  Parameters: none.
 
  Returns: SUCCESS - always. 
*******************************************************************************/
result_t HPLClock__StdControl__stop(void)
{
  uint8_t mi;

  ATOMIC_SECTION_ENTER
    mi = HPLClock__minterval;
  ATOMIC_SECTION_LEAVE
  HPLClock__Clock__setRate(mi, 0);
  return SUCCESS;
}

/*******************************************************************************
  Description: Interrupt handler.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
SIGNAL(SIG_OUTPUT_COMPARE5A)
{
/*	ATOMIC_SECTION_ENTER
    	if (HPLClock__set_flag)
        {
        	HPLClock__mscale = HPLClock__nextScale;
         	HPLClock__nextScale |= 0x8;
         	HPLClock__set_flag = 0;
        }
	ATOMIC_SECTION_LEAVE*/
  
  if (++HPLClock__cnt >= HPLClock__counter)
  {
  #ifdef DEBUG_PROFILE_HPLCLOCK_ISR
    DDRB |= 1<<PIN6;    //yellow led
    PORTB |= 1<<PIN6;
  #endif
    HPLClock__cnt = 0;
    TimerM__Clock__fire();
  #ifdef DEBUG_PROFILE_HPLCLOCK_ISR
    PORTB &= ~(1<<PIN6);
  #endif
  }

}
// eof hplclock.c
