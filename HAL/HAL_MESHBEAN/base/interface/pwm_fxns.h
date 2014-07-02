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
  pwm_fxns.h
  
  Declares PWM interface.

  Written by P.Sobol.
***********************************************************************/

#ifndef PWM_FXNS_H_
#define PWM_FXNS_H_

#include "com_types.h"

typedef enum
{
  PWM_BASE_FREQ_5000    = 0,    // base PWM frequency = 5 kHz
  PWM_BASE_FREQ_10000   = 1,    // 10 kHz  
  PWM_BASE_FREQ_20000   = 2,    // 20 kHz         
  PWM_BASE_FREQ_50000   = 3,    // 50 kHz
  PWM_BASE_FREQ_100000  = 4,    // 100 kHz
} PwmBaseFreq;

typedef Uint8 PwmDutyCycle;

#define PWM_DUTY_CYCLE_MAX 100

typedef struct 
{
  /*
   *  PWM_POLARITY_NON_INVERTED - PWM output is low when duty cycle = 0% and 
   *                              PWM output is high when duty cycle = 100%.
   *  PWM_POLARITY_INVERTED     - PWM output is high when duty cycle = 0% and
   *                              PWM output is low when duty cycle = 100%.
   */
  enum
  {
    PWM_POLARITY_NON_INVERTED  = 0,
    PWM_POLARITY_INVERTED      = 1
  } polarity;
} PwmChannelParam;

typedef enum
{
  PWM_STATUS_INVALID = 0,
  PWM_STATUS_SUCCESS,
  PWM_STATUS_ERROR,
} PwmStatus;

struct Pwm;

typedef struct
{
  struct Pwm* pPwm;

  /*****************************************************************************
    Description: opens Pwm object.

    Parameters: pPwm - reference to Pwm object.
                channel - channel number.
                param - reference to structure containing parameters for Pwm object.
    Returns: PWM_STATUS_SUCCESS if channel has been opened and PWM_STATUS_ERROR,
                                                  otherwise. 
  *****************************************************************************/
  PwmStatus (*open)(struct Pwm* pPwm, Uint8 channel, const PwmChannelParam* param);

  /*****************************************************************************
    Description: closes Pwm object.

    Parameters: pPwm - reference to Pwm object.
                channel - channel number.

    Returns: PWM_STATUS_SUCCESS if channel has been close and PWM_STATUS_ERROR,
                                                  otherwise.
  *****************************************************************************/
  PwmStatus (*close)(struct Pwm* pPwm, Uint8 channel);

  /*****************************************************************************
    Description: set base output frequency for Pwm object.

    Parameters: pPwm - reference to Pwm object.
                channel - channel number.
                frequency - frequency code.

    Returns: PWM_STATUS_SUCCESS if frequency has been set and PWM_STATUS_ERROR,
                                                  otherwise.
  *****************************************************************************/
  PwmStatus (*setFrequency)(struct Pwm* pPwm, Uint8 channel, PwmBaseFreq frequency);

  /*****************************************************************************
    Description: set duty cycle for Pwm object.

    Parameters: pPwm - reference to Pwm object.
                channel - channel number.
                value - duty cycle (integer value in %, range from 0 to 100)

    Returns: PWM_STATUS_SUCCESS if duty cycle has been set and PWM_STATUS_ERROR,
                                                  otherwise.
  *****************************************************************************/
  PwmStatus (*setDutyCycle)(struct Pwm* pPwm, Uint8 channel, PwmDutyCycle value);

  /*****************************************************************************
    Description: retrieve base output frequency for Pwm object.

    Parameters: pPwm - reference to Pwm object.
                channel - channel number.
                pFrequency - reference to the variable which receives result
                             (frequency code).

    Returns: PWM_STATUS_SUCCESS if frequency has been got and PWM_STATUS_ERROR,
                                                  otherwise.
  *****************************************************************************/
  PwmStatus (*getFrequency)(struct Pwm* pPwm, Uint8 channel, PwmBaseFreq* pFrequency);

  /*****************************************************************************
    Description: retrieve duty cycle for Pwm object.

    Parameters: pPwm - reference to Pwm object,
                channel - channel number. 
                pValue - reference to the variable which receives result
                         (integer value in %, range from 0 to 100)

    Returns: PWM_STATUS_SUCCESS if duty cycle has been got and PWM_STATUS_ERROR,
                                                  otherwise.
  *****************************************************************************/
  PwmStatus (*getDutyCycle)(struct Pwm* pPwm, Uint8 channel, PwmDutyCycle* pValue);

} PwmFxns;

#endif

// eof pwm_fxns.h
