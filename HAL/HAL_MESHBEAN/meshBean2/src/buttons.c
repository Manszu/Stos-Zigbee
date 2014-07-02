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
  Access to the buttons.
****************************************************************/

#include <tos.h>
#include <buttons.h>
#include <apptimer.h>

#ifndef KEY0
#define KEY0 GPIO_IRQ_6
#endif
#ifndef KEY1
#define KEY1 GPIO_IRQ_7
#endif

BUTTEventFunk vBUTTPressHandl;   // callback 
BUTTEventFunk vBUTTReleaseHandl; // callback

uint8_t button_state;                   // Current state of buttons.
int iButtonsPeriodicalTimer;     // timer descriptor 
   
void vbuttons__timer__fire(void); // callback of timer when one expired.

/*******************************************************************************
  Description:  Initializes buttons module.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void buttons_init()
{
uint8_t value, i;
static bool flag = FALSE;
  if( flag == TRUE ) return;
  flag = TRUE;
    button_state = 0;
    for (i = 0; i < MAX_BUTTONS_COUNT; i++)
    {
      gpio_setConfig(KEY0 + i, GPIO_INPUT_PULLUP_ON);
      gpio_getState( KEY0 + i, &value);
      button_state |= ( value & 0x01) << i;
    }
	appTimer_init();
	iButtonsPeriodicalTimer = appTimer_open(vbuttons__timer__fire);
	appTimer_start(iButtonsPeriodicalTimer, TIMER_REPEAT_MODE, SAMPLE_PERIOD);
  return ; 
}

/*******************************************************************************
  Description:  Register handlers for button events, that will be called in
                the non­interrupt context. 

  Parameters:   pressed - the handler to process pressing the button
                released ­ the handler to process releasing the button
                bn - button number.

  Returns:      SUCCESS - always.   
*******************************************************************************/
result_t buttons_open(void (*pressed)(uint8_t bn), void (*released)( uint8_t bn))
{
  buttons_init();
  vBUTTPressHandl = pressed;
  vBUTTReleaseHandl = released;
  return SUCCESS;
};

/*******************************************************************************
  Description:  Cancel button handlers.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
result_t buttons_close()
{
	appTimer_close(iButtonsPeriodicalTimer);
	vBUTTPressHandl = NULL;
	vBUTTReleaseHandl = NULL;
	return SUCCESS;
};

/*******************************************************************************
  Description: Reads state of buttoms.

  Parameters:  none.

  Returns:    current buttons state in a binary way.
              Bit 0 defines state of the button 1, bit 1 defines state of
              the button 2.  
*******************************************************************************/
uint8_t buttons_readState()
{
	return (~button_state) & (~((~(sizeof(button_state) - 1)) << MAX_BUTTONS_COUNT));
};

/*******************************************************************************
  Description:  Event about buttons poll interval expired

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void vbuttons__timer__fire(void)
{
  uint8_t i, j, value; 
    j = 0;
    for (i = 0; i < MAX_BUTTONS_COUNT; i++)
    {
        gpio_getState( KEY0 + i, &value);
        j |= (value & 0x01) << i;
        if( (button_state & (1 << i)) ^ (j & (1 << i)) )
        {// "i" key has been toggled
          button_state &= ~(1 << i);
          button_state |= j & (1 << i);
          if( !(j & (1 << i) ) )
          {// key has been pressed            
            if( vBUTTPressHandl )
              vBUTTPressHandl(i + 1);
          }
          else
          {// key has been released
            if( vBUTTReleaseHandl )
              vBUTTReleaseHandl(i + 1);
          }
        }
    }// end for
};
// eof buttons.c
