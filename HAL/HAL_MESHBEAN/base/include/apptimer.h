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
 Application timer header file.
*******************************************************************************/

#ifndef APPTIMER_H_
#define APPTIMER_H_

#include <tos.h>
#include <appclock.h>

#ifndef APP_NUM_TIMERS 
#define APP_NUM_TIMERS 0 // the number of user timers
#endif

#define NUM_TIMERS (APP_NUM_TIMERS + 12) // total number of the application timers

typedef enum {
    TIMER_REPEAT_MODE = 0,
    TIMER_ONE_SHOT_MODE = 1,
}TimerMode_t;

/*******************************************************************************
  Description: Sets the interval.

  Parameters: interval - the value which has been counted out.

  Returns: nothing.
*******************************************************************************/
void appTimer_setInterval( uint32_t interval);

/*******************************************************************************
  Description: System time.

  Parameters: nothing.

  Returns: the system time in milliseconds.
*******************************************************************************/
uint32_t getSystemTime();

/*******************************************************************************
  Description: Registers the handler for the application timer event.

  Parameters: fired -  pointer to a fired event handler.

  Returns: positive descriptor if theregistration is successful and negative
           value in other cases.
*******************************************************************************/
int appTimer_open(void (*fired)());

/*******************************************************************************
  Description: Cancels the handler that was associated with the id descriptor.

  Parameters: id - the timer descriptor.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_close(int id);

/*******************************************************************************
  Description: Starts the timer.

  Parameters: id - descriptor.
              mode - application timer mode.
              delay - delay in milliseconds.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_start(int id, TimerMode_t mode, uint32_t interval);

/*******************************************************************************
  Description: Stops the application timer.

  Parameters: id -  descriptor.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_stop(int id);

#endif // APPTIMER_H_
