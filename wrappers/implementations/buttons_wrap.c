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
  buttons_wrap.c
  
  Buttons wrapper implementation.
  
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "buttons_cntrl_fxns.h"
#include <gpio.h>
#include <irq.h>

static void key1Handler(void);
static void key2Handler(void);

static struct Buttons* buttons;

/*******************************************************************************
  Description: read events of buttons interface.
  	
  Parameters: pButtons - buttons instance reference.
  	
  Returns: state of buttons.
*******************************************************************************/
static ButtonState getState(struct Buttons* pButtons)
{
   return 0;
}

/*******************************************************************************
  Description: read events of buttons interface.
  	
  Parameters: pButtons - buttons instance reference,
              pButtonEvent - pointer to ButtonEvent struct.
  	
  Returns: true if event available, false otherwise.
*******************************************************************************/
static Bool getEvent(struct Buttons* pButtons, ButtonEvent* pButtonEvent)
{
  if (pButtons->isEvent)
  {
    pButtons->isEvent = false;
    *pButtonEvent = pButtons->event;
    return true;
  }
  else
    return false;
}

/*******************************************************************************
  Description: clear events of buttons.
  	
  Parameters: pButtons - buttons object reference.
  	
  Returns: nothing.
*******************************************************************************/
static void clear(struct Buttons* pButtons)
{
   pButtons->isEvent = false;
}

/*******************************************************************************
  Description: provides reference to Buttons interface implementation.
  	
  Parameters: pButtonsFxns - buttons interface implementation reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_getFxns(ButtonsFxns* pButtonsFxns)
{
   pButtonsFxns->getState = getState;
   pButtonsFxns->getEvent = getEvent;
   pButtonsFxns->clear = clear;
}

/*******************************************************************************
  Description: inits the buttons.
  	
  Parameters: pButtons - buttons object reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_init(struct Buttons* pButtons)
{
   pButtons->isEvent = false;

   gpio_setConfig (GPIO_IRQ_6, GPIO_INPUT_PULLUP_ON);
   gpio_setConfig (GPIO_IRQ_7, GPIO_INPUT_PULLUP_ON);
   irq_register(IRQ_6, IRQ_FALLING_EDGE, key1Handler);
   irq_register(IRQ_7, IRQ_FALLING_EDGE, key2Handler);
   irq_enable(IRQ_6);
   irq_enable(IRQ_7);
   
   buttons = pButtons;
}

/*******************************************************************************
  Description: work loop for buttons.
  	
  Parameters: pButtons - buttons object reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_process(struct Buttons* pButtons)
{
   
}

/*******************************************************************************
  Description: handler of button 1.
  	
  Parameters: nothing.
  	
  Returns: noting.
*******************************************************************************/
static void key1Handler(void)
{
   buttons->isEvent = true;
   buttons->event = BUTTON_0_PRESS_EVENT;
}

/*******************************************************************************
  Description: handler of button 2.
  	
  Parameters: nothing.
  	
  Returns: noting.
*******************************************************************************/
static void key2Handler(void)
{
   buttons->isEvent = true;
   buttons->event = BUTTON_1_PRESS_EVENT;
}

// eof buttons_wrap.c
