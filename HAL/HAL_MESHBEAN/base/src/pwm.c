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
  pwm.c
  
  Simple PWM module implementation for ATMega1281
  Used Timer1
  
  Written by P.Sobol.
*******************************************************************************/

#include <tos.h>
#include "com_assert.h"
#include <avrhardware.h>
#include <clkctrl.h>
#include "pwm_cntrl_fxns.h"

#define PWM_PRESCALE    1

static const Uint16 topCount[] = 
{ 
  F_CPU / (2 *   5000ul * PWM_PRESCALE), // PWM_BASE_FREQ_5000
  F_CPU / (2 *  10000ul * PWM_PRESCALE), // PWM_BASE_FREQ_10000
  F_CPU / (2 *  20000ul * PWM_PRESCALE), // PWM_BASE_FREQ_20000
  F_CPU / (2 *  50000ul * PWM_PRESCALE), // PWM_BASE_FREQ_50000
  F_CPU / (2 * 100000ul * PWM_PRESCALE), // PWM_BASE_FREQ_100000
};

// Sets base output frequency for Pwm object.
static PwmStatus setFrequency(struct Pwm*, Uint8, PwmBaseFreq);

/*******************************************************************************
  Description: inits the PWM .  
  	
  Parameters: pPwm - PWM object reference.
  	
  Returns: nothing.

  Notes:  Pwm channel number determines the hardware PWM channel the Pwm object is associated with.
          Each PWM channel is hardwired to the particular output pin (see table).
          
          PWM channel   output pin
              0        GPIO0
              1        GPIO1
              2        GPIO2
          
          On the MeshBean2 board, GPIO0...GPIO2 pins connected to LEDs .
*******************************************************************************/
void pwm_init(struct Pwm* pPwm)
{
  ASSERT(pPwm);
  
  pPwm->channels[0].isOpened = false;  
  pPwm->channels[1].isOpened = false;
  pPwm->channels[2].isOpened = false;
  pPwm->state = PWN_GEN_PASSIVE;
}

/*******************************************************************************
*******************************************************************************/
#define SETUP_PWM_CHANNEL(chan, pin) \
      OCR1##chan##H = 0; \
      OCR1##chan##L = 0; \
      TCCR1A = ((TCCR1A | (1 << COM1##chan##1)) & ~(1 << COM1##chan##0)) | (polarity ? (1 << COM1##chan##0) : 0); \
      DDRB |= (1 << DD##pin); 

/*******************************************************************************/
#define CLOSE_PWM_CHANNEL(chan, pin) \
      DDRB &= ~(1 << DD##pin); \
      TCCR1A &= ~((1 << COM1##chan##1) || (1 << COM1##chan##0));

/**********************************************************************************
  Description: opens Pwm object.
    
  Parameters: pPwm - reference to Pwm object,
              channel - channel number.
              pParams - reference to structure containing parameters for Pwm object.
  Returns: PWM_STATUS_ERROR if:
                               - channel number is out of range,
                               - channel has been already opened
           and PWM_STATUS_SUCCESS, otherwise.
  
  Notes: When PWM channel is opened, corresponding output pin is configured as 
         output and then controlled by PWM channel. Duty cycle for the channel is set to 0. 
         Base frequency for the channel is set to default (5kHz) if were no opened channels, 
         otherwise the last frequency set for any channel is active for the particular channel.
*******************************************************************************/
static PwmStatus open(struct Pwm* pPwm, Uint8 channel, const PwmChannelParam* pParams)
{
  PwmStatus status = PWM_STATUS_ERROR;  
    
  if ((channel < PWM_MAX_CHANNEL) && !pPwm->channels[channel].isOpened)
  {
    Uint8 polarity = (pParams->polarity == PWM_POLARITY_INVERTED);
    
    if (PWN_GEN_PASSIVE == pPwm->state)
    {
      // Setup common PWM generator.
      // Setting Phase and Frequency correct PWM mode.
      TCCR1B = (1 << WGM13) | (0 << WGM12); // WGM1[3:2] =10, CS[2:0]=000. Timer clock stopped.
      TCCR1A = TCCR1A & ~((1 << WGM11) | (1 << WGM10));   // WGM1[1:0] =00, don't touch COM1x bits. 
      TCNT1H = 0; // clear counter
      TCNT1L = 0;
      ICR1H = 0;  // clear top count register
      ICR1L = 0;
      pPwm->state = PWM_GEN_ACTIVE;
      setFrequency(pPwm, 0, PWM_DEFAULT_FREQUENCY);
    }
    
    // Setup particular PWM channel
    switch (channel)
    {
      case 0: // PWM channel A
           SETUP_PWM_CHANNEL(A, 5);
           break;
      
      case 1: // PWM channel B
           SETUP_PWM_CHANNEL(B, 6);
           break;
      
      case 2: // PWM channel C
           SETUP_PWM_CHANNEL(C, 7);
           break;

      default:
           ASSERT(false);
    }
    pPwm->channels[channel].isOpened = true;
    pPwm->channels[channel].dutyCycle = 0;
    status = PWM_STATUS_SUCCESS;
  }
  return status;
}

/**********************************************************************************
  Description: closes Pwm object.
    
  Parameters: pPwm - reference to Pwm object.
              channel - channel number.
  
  Returns: PWM_STATUS_ERROR if:
                               - channel number is out of range,
                               - channel has not been opened yet
           and PWM_STATUS_SUCCESS, otherwise.

  Notes: When PWM channel is disabled by setting reg  to 0 or 2, corresponding output 
         pin is once configured as tri-state and then fully controlled as GPIO.
*******************************************************************************/
static PwmStatus close(struct Pwm* pPwm, Uint8 channel)
{
  PwmStatus status = PWM_STATUS_ERROR;  
    
  if ((channel < PWM_MAX_CHANNEL) && pPwm->channels[channel].isOpened)
  {  
    // stop particular PWM channel
    switch (channel)
    { 
      case 0: // PWM channel A
           CLOSE_PWM_CHANNEL(A, 5);
           break;
      
      case 1: // PWM channel B
           CLOSE_PWM_CHANNEL(B, 6);
           break;
      
      case 2: // PWM channel C
           CLOSE_PWM_CHANNEL(C, 7);
           break;

      default:
           ASSERT(false);
    }
      
    if (!pPwm->channels[0].isOpened &&
        !pPwm->channels[1].isOpened &&
        !pPwm->channels[2].isOpened)
    {
      // no more opened PWM channels - close PwmGen
      TCCR1B = 0; // disable PWM, stop timer
      TCCR1A = 0;
      pPwm->state = PWN_GEN_PASSIVE;
    }
    status = PWM_STATUS_SUCCESS;
  }
  
  return status;
}

/**********************************************************************************
  Description: set base output frequency for Pwm object.
    
  Parameters: pPwm - reference to Pwm object.
              channel - channel number.
              frequency - frequency code.
  
  Returns: PWM_STATUS_ERROR if:
                               - invalid PWM object reference.
                               - channel number is out of range,
                               - channel has not been opened yet
                               - wrong frequency.
           and PWM_STATUS_SUCCESS, otherwise.

  Notes: Actually base frequency selection for any PWM channel affects all channels 
         (frequency is common for all channels).
         If the selected frequency is different from the previously set frequency 
         then duty cycle for all PWM channels will be reset to 0.
*******************************************************************************/
static PwmStatus setFrequency(struct Pwm* pPwm, Uint8 channel, PwmBaseFreq frequency)
{
  PwmStatus status = PWM_STATUS_ERROR;  
  
  if (pPwm)
  {
    if ((channel < PWM_MAX_CHANNEL) &&
        pPwm->channels[channel].isOpened &&
        (frequency <= PWM_BASE_FREQ_100000))
    {
      if (frequency != pPwm->currentFrequency)
      {
        // need to set new frequency
        pPwm->currentFrequency = frequency;
        TCCR1B = TCCR1B & ~((1 << CS12) | (1 << CS11) | (1 << CS10)); // stop timer
        ICR1H = topCount[frequency] >> 8;        // set top register  
        ICR1L = topCount[frequency] & 0xFF;
        TCNT1H = 0;  // clear timer                                   
        TCNT1L = 0;
        OCR1AH = 0;  // clear PWM registers (all channels)
        OCR1AL = 0;
        OCR1BH = 0;
        OCR1BL = 0;
        OCR1CH = 0;
        OCR1CL = 0;
        pPwm->channels[0].dutyCycle = 0;
        pPwm->channels[1].dutyCycle = 0;
        pPwm->channels[2].dutyCycle = 0;
        TCCR1B = TCCR1B | (1 << CS10); // run timer, prescaler=1
      }
      status = PWM_STATUS_SUCCESS;
    }  
  }  
 
  return status;  
}

/**********************************************************************************
  Description: retrieve base output frequency for Pwm object.
    
  Parameters: pPwm - reference to Pwm object,
              channel - channel number.
              pFrequency - reference to the variable which receives result
                            (frequency code).
  
  Returns: PWM_STATUS_ERROR if:
                               - invalid PWM object reference.
                               - invalid frequency reference.
                               - channel number is out of range,
                               - channel has not been opened yet
           and PWM_STATUS_SUCCESS, otherwise.
*******************************************************************************/
static PwmStatus getFrequency(struct Pwm* pPwm, Uint8 channel, PwmBaseFreq* pFrequency)
{
  PwmStatus status = PWM_STATUS_ERROR;  
  
  if (pPwm && pFrequency)
  {
    if ((channel < PWM_MAX_CHANNEL) && pPwm->channels[channel].isOpened)
    {  
      *pFrequency = pPwm->currentFrequency;
      status = PWM_STATUS_SUCCESS;
    }
  }

  return status;
} 

/**********************************************************************************
  Description: set duty cycle for Pwm object.
    
  Parameters: pPwm - reference to Pwm object,
              channel - channel number.
              pValue - reference to the variable which receives result 
                        (integer value in %, range from 0 to 100)
  
  Returns: nothing.

  Notes: Actual duty cycle on tstatic void open(struct Pwm* pPwm, const PwmChannelParam* param)he output pin will be changed only at the end of 
         the current period of the base frequency. 
         Resolution of the duty cycle setting depends on the base frequency (see table).
 
         PWM base frequency  Duty cycle resolution
                   5 kHz           1%
                  10 kHz           1%
                  20 kHz           1%
                  50 kHz           2,5%
                 100 kHz           5%
**********************************************************************************/
static PwmStatus setDutyCycle(struct Pwm* pPwm, Uint8 channel, const PwmDutyCycle value)
{
  PwmStatus status = PWM_STATUS_ERROR;  
  
  if (pPwm)
  {
    if ((channel < PWM_MAX_CHANNEL) &&
        pPwm->channels[channel].isOpened &&
        value <= PWM_DUTY_CYCLE_MAX)
    {  
      Uint16 cmpValue;
  
      pPwm->channels[channel].dutyCycle = value;
      cmpValue = (topCount[pPwm->currentFrequency] * value) / 100; 

      switch (channel)
      {
        case 0: // PWM channel A
             OCR1AH = cmpValue >> 8;  
             OCR1AL = cmpValue & 0xFF;
             break;

        case 1: // PWM channel B
             OCR1BH = cmpValue >> 8;  
             OCR1BL = cmpValue & 0xFF;
             break;
      
        case 2: // PWM channel C
             OCR1CH = cmpValue >> 8;  
             OCR1CL = cmpValue & 0xFF;
             break;

        default:
             ASSERT(false);
      }
      status = PWM_STATUS_SUCCESS;
    }
  }

  return status;  
}

/**********************************************************************************
  Description: retrieve duty cycle for Pwm object.
    
  Parameters: pPwm - reference to Pwm object,
              channel - channel number.
              pValue - reference to the variable which receives result 
                        (integer value in %, range from 0 to 100)
  
  Returns: PWM_STATUS_ERROR if:
                               - channel number is out of range,
                               - channel has not been opened yet
           and PWM_STATUS_SUCCESS, otherwise.
*******************************************************************************/
static PwmStatus getDutyCycle(struct Pwm* pPwm, Uint8 channel, PwmDutyCycle* pValue)
{
  PwmStatus status = PWM_STATUS_ERROR;  
  
  ASSERT(pPwm && pValue);
    
  if ((channel < PWM_MAX_CHANNEL) && pPwm->channels[channel].isOpened)
  {  
    *pValue = pPwm->channels[channel].dutyCycle;
    status = PWM_STATUS_SUCCESS;
  }

  return status;
}

/*******************************************************************************
  Description: provides reference to PWM interface implementation.
  	
  Parameters: pPwmFxns - PWM interface implementation reference.
  	
  Returns: nothing.
*******************************************************************************/
void pwm_getFxns(PwmFxns* pPwmFxns)
{
  ASSERT(pPwmFxns);
  
  pPwmFxns->open = open;
  pPwmFxns->close = close;
  pPwmFxns->setFrequency = setFrequency;
  pPwmFxns->setDutyCycle = setDutyCycle;
  pPwmFxns->getFrequency = getFrequency;
  pPwmFxns->getDutyCycle = getDutyCycle;
}

// eof pwm.c
