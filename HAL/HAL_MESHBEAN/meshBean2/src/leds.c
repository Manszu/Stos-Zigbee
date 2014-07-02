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

/****************************************************************
  The module for access to leds.
****************************************************************/

#include <leds.h>
#include <gpio.h>
#include <sensors.h>

#ifndef LED_ON 
#define LED_ON  1
#endif
#ifndef LED_OFF 
#define LED_OFF 0
#endif

/*******************************************************************************
  Description:  Inits LEDs control module

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void leds_init() 
{       
  static bool flag = FALSE;
  if( flag == TRUE ) return;
  flag = TRUE;
  gpio_setConfig(LED_RED, GPIO_OUTPUT);
  gpio_setConfig(LED_GREEN, GPIO_OUTPUT);
  gpio_setConfig(LED_YELLOW, GPIO_OUTPUT);
}

/*******************************************************************************
  Description:  Opens leds module to use.

  Parameters:   none.

  Returns:      SUCCESS - always.
*******************************************************************************/
result_t leds_open(void)
{
  leds_init();
  hpl__peripherial__on( SENSOR_LED ); 
  return SUCCESS;
}

/*******************************************************************************
  Description: Closes leds module. 

  Parameters:  none.

  Returns:     SUCCESS - always.
*******************************************************************************/
result_t leds_close(void)
{
  hpl__peripherial__off( SENSOR_LED ); 
  return SUCCESS;
}

/*******************************************************************************
  Description:  Turns on the LED.

  Parameters:   id - number of led

  Returns:      none.
*******************************************************************************/
void leds_on(uint8_t id)
{
  switch( id )
  {
    case LED_RED:
    case LED_GREEN:
    case LED_YELLOW:
        gpio_setState( id, LED_ON );
    break;
  }
}

/*******************************************************************************
  Description:  Turns off the LED.

  Parameters:   id - number of led

  Returns:      none.
*******************************************************************************/
void leds_off(uint8_t id)
{
  switch( id )
  {
    case LED_RED:
    case LED_GREEN:
    case LED_YELLOW:
        gpio_setState( id, LED_OFF );
    break;
  }
}

/*******************************************************************************
  Description:  Changes the LED state to opposite.

  Parameters:   id - number of led

  Returns:      none.
*******************************************************************************/
void leds_toggle(uint8_t id)
{
uint8_t value;
  switch( id )
  {
    case LED_RED:
    case LED_GREEN:
    case LED_YELLOW:
      gpio_getState( id, &value);
      if(value == LED_ON)
        leds_off( id );
      else
        leds_on( id );
    break;
  }
}
// eof leds.c
