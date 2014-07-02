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
  adc.c

  ADC interface implementation.
******************************************************************************/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <adc.h>

#define NUM_ADC_CHANNELS 4

enum
{  
  ADC_IDLE,
  ADC_BUSY,
  ADC_FREE
} ADC_state = ADC_IDLE;

uint8_t ui8ActiveChannel; //contains a number of active channel.

typedef void (*ADC_callback)(uint16_t);
ADC_callback ADC_callback_list[NUM_ADC_CHANNELS]; //callbacks array.

/*******************************************************************************
  Description: ADC initialization.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void adc_init(void)
{  
  uint8_t i;
  for (i = 0; i < NUM_ADC_CHANNELS; i++) ADC_callback_list[i] = NULL;
  // Disable Power Reduction ADC
  PRR0 &= ~ (1 << PRADC);
  ADMUX = HAL_ADC_REF_VOLTAGE_SOURCE;
  // ADC Enable, ADC Prescaler division factor = 32 (125KHz for CLK = 4MHz)
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);
  ADC_state = ADC_FREE;
}

/*******************************************************************************
  Description: Opens channel adcNumber, registers callback for data indication.

  Parameters: adcNumber - channel number.
              f - data indicaion handler.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise..
*******************************************************************************/
result_t adc_open(uint8_t adcNumber, void (*f)(uint16_t data))
{
  // ADC Number is out of range or 
  // CALLBACK is not defined
  if ((adcNumber >= NUM_ADC_CHANNELS) && (f == NULL))
    return FAIL;
  // ADC Channel has been already opened
  if (ADC_callback_list[adcNumber] != NULL)
    return FAIL;
  // ADC initialization
  if (ADC_state == ADC_IDLE) adc_init();
  // Digital input disable
  DIDR0 |= (1 << adcNumber);
  // Register callback
  ADC_callback_list[adcNumber] = f;
  return SUCCESS;
}

/*******************************************************************************
  Description: Gets sample from ADC channel adcNumber. The result is returned
               by callback registered by adc_open() function.

  Parameters: adcNumber - channel number.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise.
*******************************************************************************/
result_t adc_get(uint8_t adcNumber)
{
  // ADC Number is out of range
  if (adcNumber >= NUM_ADC_CHANNELS)
    return FAIL;
  // ADC Channel has not been opened yet
  if (ADC_callback_list[adcNumber] == NULL)
    return FAIL;
  // ADC is run
  if (ADC_state == ADC_BUSY)
    return BUSY;
  ui8ActiveChannel = adcNumber;
  ADC_state = ADC_BUSY;
  // Change ADC channel
  ADMUX = (ADMUX & 0xE0) | adcNumber;
  // Start one conversion
  ADCSRA |= (1 << ADIE);
  ADCSRA |= (1 << ADSC);
  return SUCCESS;
}

/*******************************************************************************
  Description: Closes adcNumber channel.

  Parameters: adcNumber - channel number.

  Returns: FAIL - if adcNumber is out of range or channel has been already opened.
           SUCCESS - otherwise.
*******************************************************************************/
result_t adc_close(uint8_t adcNumber)
{  
  // ADC Channel has not been opened yet
  if (ADC_callback_list[adcNumber] == NULL)
    return FAIL;
  // ADC Interrupt Disable
  ADCSRA &= ~(1 << ADIE);
  // Digital input enable
  DIDR0 &= ~(1 << adcNumber);
  ADC_callback_list[adcNumber] = NULL;
  uint8_t i;
  for (i = 0; i < NUM_ADC_CHANNELS; i++) 
    if(ADC_callback_list[i] != NULL) break;
  // ADC Disable (if all ADC channels are closed)
  if (i == NUM_ADC_CHANNELS) 
  {
    ADCSRA &= ~(1 << ADEN);
    // Enable Power Reduction ADC
    PRR0 = (1 << PRADC);
    ADC_state = ADC_IDLE;
  }
  return SUCCESS;
}

/*******************************************************************************
  Description: ADC conversion complete interrupt handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
SIGNAL(SIG_ADC)
{
  uint16_t data;
  // Disable ADC Interrupt
  ADCSRA &= ~(1 << ADIE);
  // Read ADC conversion result
  data = (uint16_t) ADCL;
  data |= (uint16_t) ADCH << 8;
  // Run callback
  ADC_callback_list[ui8ActiveChannel](data);
  ADC_state = ADC_FREE;
}
// eof adc.c
