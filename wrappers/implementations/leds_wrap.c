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
  leds_wrap.c
  
  LEDs wrapper implementation for meashbean2.
  
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "leds_cntrl_fxns.h"
#include "avrhardware.h"
#include "leds.h"

#define RED_LED_MASK    0x20
#define YELLOW_LED_MASK 0x40
#define GREEN_LED_MASK  0x80

#define RED_LED_ON       PORTB = PORTB | RED_LED_MASK;
#define RED_LED_OFF      PORTB = PORTB & ~RED_LED_MASK;
#define RED_LED_TOGGLE   PORTB = PORTB ^ RED_LED_MASK;

#define YELLOW_LED_ON       PORTB = PORTB | YELLOW_LED_MASK;
#define YELLOW_LED_OFF      PORTB = PORTB & ~YELLOW_LED_MASK;
#define YELLOW_LED_TOGGLE   PORTB = PORTB ^ YELLOW_LED_MASK;

#define GREEN_LED_ON       PORTB = PORTB | GREEN_LED_MASK;
#define GREEN_LED_OFF      PORTB = PORTB & ~GREEN_LED_MASK;
#define GREEN_LED_TOGGLE   PORTB = PORTB ^ GREEN_LED_MASK;

/*******************************************************************************
  Description: turn LED on.
  	
  Parameters: pLeds - LED object reference, n - LED number.
  	
  Returns: nothing.
*******************************************************************************/
static void on(struct Leds* pLeds, Led n)
{
   switch (n)
   {
      case RED_LED:
           RED_LED_ON;
           break;
      
      case YELLOW_LED:
           YELLOW_LED_ON;
           break;
           
      case GREEN_LED:
           GREEN_LED_ON;
           break;
   }   
}

/*******************************************************************************
  Description: turn LED off.
  	
  Parameters: pLeds - LEDs object reference, n - LED number.
  	
  Returns: nothing.
*******************************************************************************/
static void off(struct Leds* pLeds, Led n)
{
   switch (n)
   {
      case RED_LED:
           RED_LED_OFF;
           break;
      
      case YELLOW_LED:
           YELLOW_LED_OFF;
           break;
           
      case GREEN_LED:
           GREEN_LED_OFF;
           break;
   }
}

/*******************************************************************************
  Description: toggle LED off.
  	
  Parameters: pLeds - LEDs object reference, n - LED number.
  	
  Returns: nothing.
*******************************************************************************/
static void toggle(struct Leds* pLeds, Led n)
{
   switch (n)
   {
      case RED_LED:
           RED_LED_TOGGLE;
           break;
      
      case YELLOW_LED:
           YELLOW_LED_TOGGLE;
           break;
           
      case GREEN_LED:
           GREEN_LED_TOGGLE;
           break;
   }
}

/******************************************************************************
  Description: opens LEDs.

  Parameters: pLeds - LEDs object reference.
  	
  Returns: nothing.
*******************************************************************************/
static void open(struct Leds* pLeds)
{
  leds_open();
}

/******************************************************************************
  Description: closes LEDs.

  Parameters: pLeds - LEDs object reference.
  	
  Returns: nothing.
*******************************************************************************/
static void close(struct Leds* pLeds)
{
  leds_close();
}

/*******************************************************************************
  Description: provides reference to LEDs interface implementation.
  	
  Parameters: pLedsFxns - LEDs interface implementation reference.
  	
  Returns: nothing.
*******************************************************************************/
void leds_getFxns(LedsFxns* pLedsFxns)
{
   pLedsFxns->on = on;
   pLedsFxns->off = off;
   pLedsFxns->toggle = toggle;   
   pLedsFxns->open   = open;
   pLedsFxns->close  = close;
}

/*******************************************************************************
  Description: inits the LEDs.
  	
  Parameters: pLeds - LEDs object reference.
  	
  Returns: nothing.
*******************************************************************************/
void leds_Init(struct Leds* pLeds)
{
   leds_open();
   DDRB |= (RED_LED_MASK | YELLOW_LED_MASK | GREEN_LED_MASK);
}

// eof leds_wrap.c
