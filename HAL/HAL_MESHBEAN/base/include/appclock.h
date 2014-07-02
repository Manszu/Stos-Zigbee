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

#ifndef APPTIMERCLOCK_H
#define APPTIMERCLOCK_H
#include <tos.h>

struct Timer__timer_s 
{
  uint8_t type;        // type of timer
  uint32_t ticks;      // interval
  uint32_t ticksLeft;  // work interval
  void (*fired)(void); // callback
};

/*******************************************************************************
  Description: Stops the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__stop();

/*******************************************************************************
  Description: Starts the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__start();

/*******************************************************************************
  Description: The event of the appclock interrupt handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__HandleFire();

/*******************************************************************************
  Description: Stops the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__stopTimer();

/*******************************************************************************
  Description: Starts the hardware application timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__startTimer();

/*******************************************************************************
  Description: Sets a counted out interval.

  Parameters: value - contains the interval.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__setInterval(uint8_t value);

/*******************************************************************************
  Description: Returns a current value of the interval.

  Parameters: nothing.

  Returns: current value of the interval.
*******************************************************************************/
uint8_t appClock__Clock__readCounter(void);

/*******************************************************************************
  Description: Sets a count out interval. Performs init settings.

  Parameters: interval - contains the interval.

  Returns: SUCCESS.
*******************************************************************************/
result_t appClock__Clock__setRate(char interval);
#endif

// eof appclock.h
