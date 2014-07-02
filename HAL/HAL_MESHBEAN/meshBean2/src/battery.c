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
  The measurement of battery voltage.
*****************************************************************/

#include <sensors.h>
#include <atomic.h>
#include <adc.h>
#include <battery.h>

// states 
typedef enum
{
  IDLE,           // idle
  DATA,           // performs request
}batteryStates_e;

batteryStates_e battery__state; // the state of component
uint16_t battery__data;         // contains sampled data

void (*battery__dataReady)(bool error, float data); // callback

/*******************************************************************************
  Description:  Inits the use of the battery sensor.

  Parameters:   none.

  Returns:      SUCCESS - always
*******************************************************************************/
result_t battery__init() 
{       
  static bool flag = FALSE;
  if( flag == TRUE ) return SUCCESS;
  flag = TRUE;
  battery__state = IDLE;
  return SUCCESS;
}

/*******************************************************************************
  Description:  Opens the component to use.

  Parameters:   none.

  Returns:      SUCCESS - the component is ready to been use.
                FAIL - otherwise.
*******************************************************************************/
result_t battery__open()
{
  battery__init();
  if( battery__state == IDLE) return SUCCESS;
  return FAIL;
}

/*******************************************************************************
  Description:  Performs the test if the component have completed request.

  Parameters:   none.

  Returns:      FAIL - the previous request is not completed.
                SUCCES - otherwise.
*******************************************************************************/
result_t battery__close()
{
  if( battery__state == IDLE) return SUCCESS;
  return FAIL;
} 

/*******************************************************************************
  Description:  The task about the data request has been done.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void battery__postsamplingDone()
{
  ATOMIC_SECTION_ENTER
    battery__state = IDLE;  
    adc_close( ADC_BAT );
  ATOMIC_SECTION_LEAVE
  battery__dataReady(TRUE, ((float)battery__data * 1.25 * (1 + 2))/1024.0 );
}

/*******************************************************************************
  Description: Notification about the request to the battery sensor 
               has been done.

  Parameters:  data - result from the sensor

  Returns:     none.
*******************************************************************************/
void battery__samplingDone(uint16_t data)
{
  battery__data = data;
  TOS_post( battery__postsamplingDone );
}

/*******************************************************************************
  Description: Gets data from battery sensor.

  Parameters:  f - callback.

  Returns:     FAIL - if previous request has not been completed yet 
               SUCCESS -  other case.
*******************************************************************************/
result_t battery__getData( void (*f)(bool result, float data) )
{
  if( battery__state != IDLE ) 
  {
    return FAIL;
  }
  if( adc_open(ADC_BAT, battery__samplingDone) == FAIL)
  {
    return FAIL;
  }
  ATOMIC_SECTION_ENTER
    battery__state = DATA;  
  ATOMIC_SECTION_LEAVE
  battery__dataReady = f;
  if( adc_get( ADC_BAT ) == FAIL )
  {
    battery__state = IDLE;  
    return FAIL;
  }
  return SUCCESS;
}

// endof battery.c
