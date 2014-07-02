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
  Module for count out requested interval.
=================================================================*/

#include <avrhardware.h>
#include <appclock.h>
#include <sleeptimerclock.h>

struct
{
  uint32_t Interval;     // Contains number of tics before will send event.
  uint32_t workInterval; // Contains number of ticks had passed since moment of call setInterval()
  uint32_t factor;       // Contains scale factor.
} HPLTimerControl;

bool HPLSleepTimer__enableSleep = FALSE; // Defines capability of the component makes shutdown.

/****************************************************************
  Description:  Stops the application timer. Sets the sleep control flag.

  Parameters:   enableSleep - TRUE system will be shutdown after 
                the wake up if the interval is still not counted.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__sleepControl(bool enableSleep)   
{
  appClock__Clock__stop();
  HPLSleepTimer__enableSleep = enableSleep;
}

/****************************************************************
  Description:  Initializes the component.

  Parameters:   none.
  
  Returns:      SUCCESS.
****************************************************************/
result_t HPLTimer0Clock__StdControl__init()
{
  ASSR |= (1 << AS2);
  TCCR2B = 0x00;  // Stop the timer
  TCCR2A = 0x00;
  TCNT2 = 0x00;
  OCR2A = 0x00;
  HPLTimerControl.workInterval = 0;
  while ( ASSR & 0x1B);
  return SUCCESS;
 }

/****************************************************************
  Description:  Starts the component and its subcomponents.

  Parameters:   none.
  
  Returns:      SUCCESS.
****************************************************************/
result_t HPLTimer0Clock__StdControl__start()
{
  return SUCCESS;
}

/****************************************************************
  Description:  Stops the component. 

  Parameters:   none.
  
  Returns:      SUCCESS.
****************************************************************/
result_t HPLTimer0Clock__StdConrtol__stop()
{
  TCCR2B &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);  // Stops the timer
  return SUCCESS;
}

/****************************************************************
  Description:  Sets clock interval. 

  Parameters:   value - contains number of ticks which the timer must count out.
  
  Returns:     none. 
****************************************************************/
void HPLTimer0Clock__setInterval( uint32_t value )
{
  TCCR2B &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);  // Stop the timer  
  HPLTimerControl.Interval = value;
  HPLTimerControl.workInterval = 0;
  HPLTimerControl.factor = value / 256ul;
  while ( ASSR & 0x1B);
  GTCCR |= 1<<PSRASY; // Prescaler reset Timer/Counter2
  TIFR2 |= (1 << OCF2A); // Clears Timer/Counter2 compare flag
  TCNT2 = 0x00;
  if(!HPLTimerControl.factor)
  {
    OCR2A = HPLTimerControl.Interval; 
  }
  else
  {
    OCR2A = 0x00;
  }
  TCCR2B |= SLEEPTIMER_PRESCALER;// starts timer 
  while ( ASSR & 0x1B);
}

/****************************************************************
  Description:  Returns the current interval in ticks.

  Parameters:   none.
  
  Returns:      current interval in ticks
****************************************************************/
uint32_t HPLTimer0Clock__getInterval()
{
  return (HPLTimerControl.workInterval + TCNT2);
}

/****************************************************************
  Description:  Returns the timer frequency in Hz.

  Parameters:   none.
  
  Returns:      timer frequency in Hz.
****************************************************************/
uint32_t HPLTimer0Clock__getFrequency()
{
  const uint16_t ScaleFactors[]={
                                  0,   // Timer/Counter stopped
                                  1,   // No prescaling
                                  8,   // clk/8
                                  32,  // clk/32
                                  64,  // clk/64
                                  128, // clk/128
                                  256, // clk/256
                                  1024 // clk/1024
                                 };
  return (SLEEPTIMER_CLOCK / (uint32_t)ScaleFactors[ SLEEPTIMER_PRESCALER ]);
}

/****************************************************************
  Description:  Set HW clock counter to a specified value.

  Parameters:   n - value to set.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__setCounter(uint8_t n)
{
  uint8_t i;
  while ( ASSR & 0x1B);
  i = TCCR2B;
  TCCR2B = 0x00;  // Stops the timer
  TCNT2 = n;
  TCCR2B = i;
  while ( ASSR & 0x1B);
  return;
}

/****************************************************************
  Description:  Disables compare interrupt.

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__intDisable()
{
  TIMSK2 &= ~(1 << OCIE2A); // Disables 8-bit Timer/Counter2 compare interrupt
  return;
}

/****************************************************************
  Description:  Enables compare interrupt.

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__intEnable()
{
  TIMSK2 |= (1 << OCIE2A); // Sets 8-bit Timer/Counter0 compare interrupt enable
  return;
}

/****************************************************************
  Description:  Interrupt handler.

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
SIGNAL(SIG_OUTPUT_COMPARE2A)
{
  uint8_t i;
  #ifdef DEBUG_PROFILE_SLEEPTIMERCLOCK_ISR
    DDRB |= 1<<PIN7;    //red led
    PORTB |= 1<<PIN7;
  #endif
  if( HPLTimerControl.factor )
  {
    // Sets the maximum interval of the uninterrupted sleep.
    HPLTimerControl.factor--;
    HPLTimerControl.workInterval += 256ul;
    if(!HPLTimerControl.factor)
    {
      i = HPLTimerControl.Interval - HPLTimerControl.workInterval; // The number of periods to sleep
      TCCR2B &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);  // Stops the timer
      TCNT2 = 0x00;
      if(i)
      {            
        OCR2A = i;
        while ( ASSR & 0x1B);  
          TCCR2B |= SLEEPTIMER_PRESCALER;// start timer  
        while ( ASSR & 0x1B);
        if( HPLSleepTimer__enableSleep == TRUE)            
          sleep();      
      }
      else
      {
        while ( ASSR & 0x1B);
        if( HPLSleepTimer__enableSleep == TRUE )
        {
          HPLSleepTimer__enableSleep = FALSE;
          PowerOn();
            appTimer_setInterval( (HPLTimer0Clock__getInterval() * 1000) / HPLTimer0Clock__getFrequency() );
            appClock__Clock__HandleFire();
            appClock__Clock__start();
        }
        HPLTimer0Clock__fire();
      }
    }
    else
    {
      if( HPLSleepTimer__enableSleep == TRUE )
      sleep();
    }
  }
  else
  {
    // Remainder from 256.
    TCCR2B &= ~(1 << CS20 | 1 << CS21 | 1 << CS22);  // Stops the timer
    TCNT2 = 0;
    HPLTimerControl.workInterval = HPLTimerControl.Interval;          
    while ( ASSR & 0x1B);
    if( HPLSleepTimer__enableSleep == TRUE )
    {
      HPLSleepTimer__enableSleep = FALSE;
      PowerOn();
      appTimer_setInterval( (HPLTimer0Clock__getInterval() * 1000) / HPLTimer0Clock__getFrequency() );
      appClock__Clock__HandleFire();
      appClock__Clock__start();
    }
    HPLTimer0Clock__fire();
  }      
  #ifdef  DEBUG_PROFILE_SLEEPTIMERCLOCK_ISR
    PORTB &= ~(1<<PIN7);
  #endif
}
//eof sleeptimerclock.c

