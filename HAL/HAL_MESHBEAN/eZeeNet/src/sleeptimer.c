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
  The implementation of the sleep timer.
****************************************************************/

#include <sleeptimer.h>
#include <sleeptimerclock.h>

typedef enum
{
  IDLE = 0,        // the timer channel is free
  ACTIVE,          // the inteval is count out
  STOPED,          // the timer channel was stopped
  QUEUED,          // the interval was added
  FIRED            // the interval was counted out
}SleepTimerControlStates_e;

struct SleepTimerControl_s
{
  int id;                          // timer descriptor
  void (*fired)();                 // callback 
  SleepTimerControlStates_e state; // the state of timer channel
  bool repeatFired;                // defines if the timer is reapit.
  TimerMode_t type;                // type of timer
  uint32_t Interval;    // contains count out interval in ticks.
  uint32_t curInterval; // contains a current interval
  uint32_t repeatInterval; // copy of interval
};

struct SleepTimerControl_s SleepTimerQueue[NUM_SLEEP_TIMERS]; // array of timers desriptors
int8_t FindMinInterval();
   
/****************************************************************
  Description:  Inits the sleep timer control module.

  Parameters:   none.

  Returns:      none.      
****************************************************************/
void sleepTimer_init()
{
  uint8_t i;
  static uint8_t first = 0;
  if(first) return;
    first = 1;
  for(i=0; i< NUM_SLEEP_TIMERS; i++)
  {
    SleepTimerQueue[i].id = 0;
    SleepTimerQueue[i].fired = 0;
    SleepTimerQueue[i].state = IDLE;
    SleepTimerQueue[i].repeatFired = FALSE;
  }
  HPLTimer0Clock__StdControl__init();
}  

/****************************************************************
  Description:  Search timers the interval of which was counted out.
                Calls callbacks.

  Parameters:   none.

  Returns:      none.      
****************************************************************/
void HandleFire() 
{
  uint8_t i;
  for(i=0; i<NUM_SLEEP_TIMERS; i++)
  {// Search for fired timers.
    if(SleepTimerQueue[ i ].state == FIRED)
    { 
      SleepTimerQueue[ i ].state = IDLE;
      if( SleepTimerQueue[ i ].fired )
        SleepTimerQueue[ i ].fired();
    }
    else
    {
      if(SleepTimerQueue[ i ].repeatFired == TRUE)
      {
        SleepTimerQueue[ i ].repeatFired = FALSE;
          if( SleepTimerQueue[ i ].fired )
             SleepTimerQueue[ i ].fired();
      }
    }
  }
}

/****************************************************************
  Description:  Takes account of interval which was counted out 
                in other timers channels. 

  Parameters:   none.

  Returns:      none.      
****************************************************************/
void AdjustInterval()
{
  uint8_t i;
  uint32_t l;
  l = HPLTimer0Clock__getInterval();
  for(i=0; i<NUM_SLEEP_TIMERS; i++)
  {// interval subtraction cycle
    if((SleepTimerQueue[ i ].state == STOPED) && (SleepTimerQueue[ i ].Interval > l))
    {
      SleepTimerQueue[ i ].Interval -= l;
    }
    else
    {
      if( (SleepTimerQueue[ i ].state == STOPED) || (SleepTimerQueue[ i ].state == FIRED))
      {
        if( SleepTimerQueue[ i ].type == TIMER_REPEAT_MODE)
        {
          SleepTimerQueue[ i ].state = STOPED;
          SleepTimerQueue[ i ].repeatFired = TRUE;
          SleepTimerQueue[ i ].Interval = SleepTimerQueue[ i ].repeatInterval;
          SleepTimerQueue[ i ].curInterval = 0;
        }
        else
        {  
          SleepTimerQueue[ i ].state = FIRED;
          SleepTimerQueue[ i ].Interval = 0;
        }
      }
      else // state = QUEUED?
      {
        if( SleepTimerQueue[ i ].state == QUEUED )
        {
          SleepTimerQueue[ i ].state = STOPED;
          if(SleepTimerQueue[ i ].Interval > (l - SleepTimerQueue[ i ].curInterval))
          {
            SleepTimerQueue[ i ].Interval -= (l - SleepTimerQueue[ i ].curInterval);
          }
          else
          {// ubnormal situation ?
            if( SleepTimerQueue[ i ].id )
            {
              SleepTimerQueue[ i ].state = FIRED;
              SleepTimerQueue[ i ].Interval = 0;
            }
          }
        }
        else{} // IDLE?
      }
    }
  }// end of interval subtraction cycle
}// end AdjustInterval

/****************************************************************
  Description:  Performs a search of  timer which was stopped and has 
                minimal interval.

  Parameters:   none.

  Returns:      descriptor of found timer,  -1 - otherwise.    
****************************************************************/
int8_t FindMinInterval()
{
  int8_t i,j;
  for(i=0,j=0; i<NUM_SLEEP_TIMERS; i++)
  {//cycle to find min interval
    if(SleepTimerQueue[ i ].state != STOPED) continue;
    if(SleepTimerQueue[ j ].state != STOPED) j = i;
    if(SleepTimerQueue[ i ].Interval <= SleepTimerQueue[ j ].Interval) // sign '=' for NUM_SLEEP_TIMERS=1 
    {
      j = i; 
    }
  }
  return  (SleepTimerQueue[ j ].state == STOPED) ? j:-1;
}// end FindMinInterval()


/****************************************************************
  Description:  Registers handler for sleep timer fired event.
  
  Parameters:   fired  - pointer to fired event handler.

  Returns:      positive descriptor if the registration is successful,
                -1 - in other case.
****************************************************************/
int sleepTimer_open(void (*fired)())
{
  uint8_t i;
  sleepTimer_init();
  if(fired)
  {
    for(i=0; i < NUM_SLEEP_TIMERS; i++)
    {
      if( !SleepTimerQueue[ i ].id )
      {
        SleepTimerQueue[ i ].id = (i+1);
        SleepTimerQueue[ i ].fired = fired;
        return SleepTimerQueue[ i ].id; // descriptor
      }
    }
    return -1; // there are not free channels
  }
  else
  {
    return -1;
  }
}

/****************************************************************
  Description:  Cancels handler was concerned with id descriptor.

  Parameters:   id - timer descriptor.

  Returns:      FAIL - there is no such descriptor, 
                SUCCESS - other case.
****************************************************************/
result_t sleepTimer_close(int id)
{
  uint8_t i;
  for(i=0; i < NUM_SLEEP_TIMERS; i++)
  {
    if( SleepTimerQueue[ i ].id == id)
    {
      SleepTimerQueue[ i ].state = IDLE;
      SleepTimerQueue[ i ].id = 0;
      SleepTimerQueue[ i ].fired = 0;
      return SUCCESS;
    }
  }
  return FAIL; // bad descriptor
}

/****************************************************************
  Description:  Starts sleep timer.

  Parameters:   id - timer descriptor,
                mode - sleep timer mode,
                interval - interval in ms.

  Returns:      FAIL - bad descriptor or the type of timer.
                SUCCESS - otherwise.        
****************************************************************/
result_t sleepTimer_start(int id, TimerMode_t type, uint32_t interval) 
{
  uint8_t i,j; // for stuff
  uint32_t l;
  for(i=0; i < NUM_SLEEP_TIMERS; i++)
  {
    if(SleepTimerQueue[ i ].id == id) break;
  }
  if (i >= NUM_SLEEP_TIMERS) return FAIL; // bad descriptor
  if (type > 1) return FAIL;
  id = i;
  if (SleepTimerQueue[ id ].state != IDLE) return FAIL;
  SleepTimerQueue[ id ].Interval = (HPLTimer0Clock__getFrequency() * interval)/1000ul; // interval in ticks
  if(!SleepTimerQueue[ id ].Interval) return FAIL;// Can't count out interval
  SleepTimerQueue[ id ].type = type;        
  SleepTimerQueue[ id ].curInterval = 0;
  SleepTimerQueue[ id ].repeatInterval = SleepTimerQueue[ id ].Interval;

  for(i=0; i<NUM_SLEEP_TIMERS; i++)
  {// Search for active timer.
    if(SleepTimerQueue[ i ].state == ACTIVE) break;
  }
  if(i < NUM_SLEEP_TIMERS)
  {/// It is really there is active timer.
    HPLTimer0Clock__intDisable();
    l = HPLTimer0Clock__getInterval();
    if(SleepTimerQueue[ i ].Interval < l)
    {
      l = SleepTimerQueue[ i ].Interval;
    }
    if(SleepTimerQueue[ id ].Interval < (SleepTimerQueue[ i ].Interval - l))
    {/* Hmm, remainder is bigger req interval.
        We should stop active timer and subtract counted out result
      */
      SleepTimerQueue[ i ].state = STOPED; // stop active timer
      AdjustInterval();
      SleepTimerQueue[ id ].state = STOPED;// welcome 
        if( (int8_t)(j = FindMinInterval()) != (int8_t)(-1))
        {//The minimal interval is really found
          HPLTimer0Clock__setInterval( SleepTimerQueue[ j ].Interval );
          SleepTimerQueue[ j ].state = ACTIVE;
          HPLTimer0Clock__intEnable();
        }
        else{}//Abnormal ???
          TOS_post(HandleFire); 
    }//if remainder is bigger req interval
    else
    {// remainder is smaller req interval
       SleepTimerQueue[ id ].state = QUEUED;
       SleepTimerQueue[ id ].curInterval = l;
    }
    HPLTimer0Clock__intEnable();
  }// if there is active timer
  else
  {// there is no active timer
    HPLTimer0Clock__setInterval( SleepTimerQueue[ id ].Interval );
    SleepTimerQueue[ id ].state = ACTIVE;
    HPLTimer0Clock__intEnable();
  }
  return SUCCESS;
}// end sleepTimer_start

/****************************************************************
  Description:  Stops timer.

  Parameters:   none.

  Returns:      FAIL - bad descriptor.
                SUCCESS - otherwise.        
****************************************************************/
result_t sleepTimer__stop(int id)
{
  uint8_t i;
  for(i=0; i<NUM_SLEEP_TIMERS; i++)
  {
    if(SleepTimerQueue[ i ].id == id) break;
  }
  if(i >= NUM_SLEEP_TIMERS) return FAIL;
  SleepTimerQueue[ i ].state = IDLE;
  return SUCCESS;
}

/****************************************************************
  Description:  Notifies on that the set interval was counted out

  Parameters:   none.

  Returns:      none.        
****************************************************************/
void HPLTimer0Clock__fire()
{
  uint8_t i;
  for(i=0; i<NUM_SLEEP_TIMERS; i++)
  {// Search for active timer.
    if(SleepTimerQueue[ i ].state == ACTIVE) break;
  }
  if( i<NUM_SLEEP_TIMERS )
  {// Active timer is really found
    SleepTimerQueue[ i ].state = FIRED;
    SleepTimerQueue[ i ].Interval = 0;
  }
  AdjustInterval();          
  if( (int8_t)(i = FindMinInterval()) != (int8_t)(-1))
  {//The minimal interval is really found
    SleepTimerQueue[ i ].state = ACTIVE;  
    HPLTimer0Clock__setInterval( SleepTimerQueue[ i ].Interval );
  }
  TOS_post(HandleFire); 
} 

//eof sleeptimer.c
