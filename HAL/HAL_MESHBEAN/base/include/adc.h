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

/******************************************************************************
  adc.h

  ADC interface header file
******************************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include <tos.h>

#define ADC_BAT 0       //ADC channel 0
#define ADC_INPUT_1 1   //ADC channel 1
#define ADC_INPUT_2 2   //ADC channel 2
#define ADC_INPUT_3 3   //ADC channel 3

// Voltage Reference = AREF, Right adjusted result
#define HAL_ADC_REF_VOLTAGE_SOURCE (0 << REFS0 | 0 << REFS1)

/*******************************************************************************
  Description: ADC initialization.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void adc_init(void);

/*******************************************************************************
  Description: Opens channel adcNumber, registers callback for data indication.

  Parameters: adcNumber - channel number.
              f - data indicaion handler.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise..
*******************************************************************************/
result_t adc_open(uint8_t adcNumber, void (*f)(uint16_t data));

/*******************************************************************************
  Description: Gets sample from ADC channel adcNumber. The result is returned
               by callback registered by adc_open() function.

  Parameters: adcNumber - channel number.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise.
*******************************************************************************/
result_t adc_get(uint8_t adcNumber);

/*******************************************************************************
  Description: Closes adcNumber channel.

  Parameters: adcNumber - channel number.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise.
*******************************************************************************/
result_t adc_close(uint8_t adcNumber);

#endif // ADC_H_
// eof adc.h
