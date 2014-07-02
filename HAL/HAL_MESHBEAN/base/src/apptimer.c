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

/*===============================================================
Implementation of application timer.
=================================================================*/
#include <apptimer.h>
#include <atomic.h>
#include <appclock.h>


uint32_t Timer__mState;         // Contains a bit mask of active timers
uint32_t Timer__mInterval;      // Contains a count out interval of the hardware timer.
int8_t Timer__queue_head;       // The queue index
int8_t Timer__queue_tail;       // The queue index
uint8_t Timer__queue_size;
uint8_t Timer__queue[NUM_TIMERS];
struct Timer__timer_s Timer__mTimerList[ NUM_TIMERS ]; // Array of the application timer descriptors.
bool Timer__initFlag = FALSE;

uint32_t systemTime = 0; // It is used  for to store the system time.

result_t Timer__Timer__fired(uint8_t);

/*******************************************************************************
  Description: Sets the interval.

  Parameters: interval - the value which has been counted out.

  Returns: nothing.
*******************************************************************************/
void appTimer_setInterval( uint32_t interval)
{
  Timer__mInterval = interval;
}

/*******************************************************************************
  Description: System time.

  Parameters: nothing.

  Returns: the system time in milliseconds.
*******************************************************************************/
uint32_t getSystemTime()
{
  return systemTime;
}

/*******************************************************************************
  Description: Removes the timer from queue.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
uint8_t Timer__dequeue(void)
{
  if (Timer__queue_size == 0) 
  {
    return NUM_TIMERS;
  }
  if (Timer__queue_head == NUM_TIMERS - 1) 
  {
    Timer__queue_head = -1;
  }
  Timer__queue_head++;
  Timer__queue_size--;
  return Timer__queue[(uint8_t )Timer__queue_head];
}

/*******************************************************************************
  Description: The search of the expired timer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
inline void Timer__signalOneTimer(void)
{
  uint8_t itimer = Timer__dequeue();

  if (itimer < NUM_TIMERS) 
  {
    Timer__Timer__fired(itimer);
  }
}

/*******************************************************************************
  Description: Adds the timer to the queue.

  Parameters: value - timer number.

  Returns: nothing.
*******************************************************************************/
static inline void Timer__enqueue(uint8_t value)
{
  if (Timer__queue_tail == NUM_TIMERS - 1) 
  {
    Timer__queue_tail = -1;
  }
  Timer__queue_tail++;
  Timer__queue_size++;
  Timer__queue[(uint8_t )Timer__queue_tail] = value;
}

/*******************************************************************************
  Description: Task processing timers after appclock interrupt.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClockHandleFire(void)
{
  uint8_t i;
  uint8_t val = 10;
  systemTime += Timer__mInterval;
  if(Timer__mState) 
  {
    for(i = 0; i < NUM_TIMERS; i++) 
    {
      if(Timer__mState & (0x1ul << i)) 
      {// the active timer
        if(Timer__mTimerList[i].ticksLeft > Timer__mInterval)
        {
          Timer__mTimerList[i].ticksLeft -= Timer__mInterval;
        }
        else
        {
          if(Timer__mTimerList[i].type == TIMER_REPEAT_MODE) 
          {
            Timer__mTimerList[i].ticksLeft = Timer__mTimerList[i].ticks;
          }
          else 
          {
            Timer__mState &= ~(0x1ul << i);
          }
          Timer__enqueue(i);
          TOS_post(Timer__signalOneTimer);
        }
        if( Timer__mState ) 
        {// There is the active logic timers.
          if( (Timer__mState & (0x1ul << i)) && (Timer__mTimerList[i].ticksLeft < val) ) 
          {// There is a need to count out other interval.
             val = Timer__mTimerList[i].ticksLeft;
          }
        }
      }// end if active timer
    }
  }
  appClock__Clock__setInterval( Timer__mInterval = val );
}

/*******************************************************************************
  Description: The event of the appclock interrupt handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appClock__Clock__HandleFire(void)
{
  TOS_post(appClockHandleFire);
}

/*******************************************************************************
  Description: Sends the event about expired the timer.

  Parameters: id - timer ID.

  Returns: nothing.
*******************************************************************************/
result_t Timer__Timer__fired(uint8_t id)
{
  unsigned int result;
  Timer__mTimerList[id].fired();
  return result;
}


/*******************************************************************************
  Description: Initialize apptimer control module.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void appTimer_init()
{
  if(!Timer__initFlag) 
  {
    Timer__initFlag = TRUE;
    Timer__mState = 0;
    Timer__queue_head = Timer__queue_tail = -1;
    Timer__queue_size = 0;
    Timer__mInterval = 10;
    appClock__Clock__setRate(Timer__mInterval);
  }
}

/*******************************************************************************
  Description: Registers the handler for the application timer event.

  Parameters: fired -  pointer to a fired event handler.

  Returns: positive descriptor if theregistration is successful and negative
           value in other cases.
*******************************************************************************/
int appTimer_open(void (*fired)())
{
  int i = 0;
  appTimer_init();
  for(i = 0; i < NUM_TIMERS; i++)
  {
    if(Timer__mTimerList[i].fired == NULL)
    {
      Timer__mTimerList[i].fired = fired;
      return i;
    }
  }	
  return -1;
}

/*******************************************************************************
  Description: Cancels the handler that was associated with the id descriptor.

  Parameters: id - the timer descriptor.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_close(int id)
{
  if ((uint16_t)id >= NUM_TIMERS)
  {
    return FAIL;
  }
  if(Timer__mTimerList[id].fired != NULL)
  {
    Timer__mTimerList[id].fired = NULL;
    return SUCCESS;
  }
  else
    return FAIL;
}

/*******************************************************************************
  Description: Starts the timer.

  Parameters: id - descriptor.
              mode - application timer mode.
              delay - delay in milliseconds.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_start(int id, TimerMode_t mode, uint32_t delay)
{
  uint8_t diff;
  
  if((uint16_t)id >= NUM_TIMERS)
  {
    return FAIL;
  }
  if(mode > 1) 
  {
    return FAIL;
  }
  Timer__mTimerList[id].ticks = delay;
  Timer__mTimerList[id].type = mode;
    //ATOMIC_SECTION_ENTER
    {
      uint8_t temp = TIMSK4;
      TIMSK4 = 0;

      diff = appClock__Clock__readCounter();
      delay += diff;
      Timer__mTimerList[id].ticksLeft = delay;
      Timer__mState |= 0x1ul << id;
      if (delay < Timer__mInterval)
      {
        Timer__mInterval = delay;
	appClock__Clock__setInterval(Timer__mInterval);
      }
      TIMSK4 = temp;
    }
    //ATOMIC_SECTION_LEAVE
  return SUCCESS;
}

/*******************************************************************************
  Description: Stops the application timer.

  Parameters: id -  descriptor.

  Returns: FAIL - there is no such descriptor.
           SUCCESS - in other cases.
*******************************************************************************/
result_t appTimer_stop(int id)
{
  if((uint16_t)id >= NUM_TIMERS) 
  {
    return FAIL;
  }
  if(Timer__mState & (0x1ul << id)) 
  {
    uint8_t temp = TIMSK4;
    TIMSK4 = 0;
    //ATOMIC_SECTION_ENTER

      Timer__mState &= ~(0x1ul << id);
    //ATOMIC_SECTION_LEAVE
    TIMSK4 = temp;

    return SUCCESS;
  }
  return FAIL;
}

// eof apptimer.c
