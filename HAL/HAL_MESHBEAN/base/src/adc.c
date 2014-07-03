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

Implementacja interfejsu ADC
******************************************************************************/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <adc.h>

#define NUM_ADC_CHANNELS 4

enum 
{  
  ADC_IDLE, //stan 0 
  ADC_BUSY, //stan 1
  ADC_FREE //stan 2
} ADC_state = ADC_IDLE; 

uint8_t ui8ActiveChannel; //contains a number of active channel.

typedef void (*ADC_callback)(uint16_t); //definicja typu void (*g) (uint16_t)
/* int foo(int i){ return i + 1;} analogiczne

typedef int (*g)(int);  // Declare typedef

g func = &foo;          // Define function-pointer variable, and initialise */

ADC_callback ADC_callback_list[NUM_ADC_CHANNELS]; //tablica wskaźników na funkcję

/*******************************************************************************
  Description: Inicjalizacja ADC

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void adc_init(void)
{  
  uint8_t i;
  for (i = 0; i < NUM_ADC_CHANNELS; i++) ADC_callback_list[i] = NULL;
  PRR0 &= ~ (1 << PRADC); // wylaczenie redukcji energii
  ADMUX = HAL_ADC_REF_VOLTAGE_SOURCE; //wpisz do rejestru ADMUX, ktory odpowiada za multiplexing kanalu ADC
  // a takze za wybor napiecia odniesienia i sposob zapisu w rejestrach ADCH ADCL
  // ADC Enable, ADC Prescaler division factor = 32 (125KHz for CLK = 4MHz)
  ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0);
  ADC_state = ADC_FREE; //stan ADC_wolny
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
  
  if ((adcNumber >= NUM_ADC_CHANNELS) && (f == NULL)) //numer kanału jest nie zdefiniowany i wskaznik na funkcje nieokreslony
    return FAIL;
  // kanał ADC jest juz otwarty
  if (ADC_callback_list[adcNumber] != NULL)  
    return FAIL;/* to samo co 
  void evaluate(bool (*pFunc[])(), int n) {
   for(int i = 0; i < n; i++)
      pFunc[i]();
} 
  bool (*pFunc[n])();
   pFunc[0] = funTrue;
   pFunc[1] = funFalse;
   evaluate(pFunc, n);  
    
    */
  // ADC initialization
  if (ADC_state == ADC_IDLE) adc_init(); //inicjalizacja ADC, teraz stan FREE
  // Digital input disable register
  DIDR0 |= (1 << adcNumber); 
  //DIDR0 is used to disable the digital input buffers on PC0 to PC5. When set, the corresponding PINC value will be set to 0.
  // Register callback
  ADC_callback_list[adcNumber] = f; // rejestracja callbacka - przypisanie funkcji f
/* Przyklad rejestracji callbacka
    #include <stdio.h>
    #include <stddef.h>
    #define CALLBACK_MAX 10
    typedef void (*callback_t)(void);
    static callback_t callbacks[CALLBACK_MAX];
    static size_t n = 0;
    void register_callback(callback_t callback)
    {
    if (n == CALLBACK_MAX)
    return;
    callbacks[n++] = callback;
    }
    void run_callbacks(void)
    {
    while (--n < (size_t)-1)
    callbacks[n](); //uruchomienie callbacka
    }
    void foo(void) { puts("foo!"); }
    void bar(void) { puts("bar!"); }
    void baz(void) { puts("baz!"); }
    int main(void)
    {
    register_callback(&foo);
    register_callback(&bar);
    register_callback(&baz);
    run_callbacks();
    return 0;
    } */
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
  ADCSRA |= (1 << ADIE); // register kontrolny konwersji AC
  ADCSRA |= (1 << ADSC); // początek konwersji
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
    ADCSRA &= ~(1 << ADEN); //instrukcja dla kontrolera, funkcje peryferyjne dla ADC wylaczam
    // Wlacz redukcje energii
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
