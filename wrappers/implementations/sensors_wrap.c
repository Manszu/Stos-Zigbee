/**
 * Copyright (c) 2005-2006 LuxLabs Ltd., All Rights Reserved.
 *
 * Redistribution, sublicensing and use in source and binary forms
 * are permitted provided that the following conditions are met:
 *
 * -Redistributions, sublicensing of source code is allowed for authorised companies
 *  only and must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 * -Redistribution, sublicensing in binary form is allowed for authorised companies
 *  only and must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the distribution.
 *
 * No changes (alteration, merge, modification, reverse engineer) can be applied
 * to the source or binary code without specific prior written permission.
 *
 * Neither the name of LuxLabs Ltd./MeshNetics or the names of contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. LUXLABS LTD. AND ITS LICENSORS SHALL NOT
 * BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL LUXLABS OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN
 * IF LUXLABS Ltd. HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or intended for
 * use in the design, construction, operation or maintenance of any nuclear
 * facility, medical or other mission-critical system.
 **/

/*******************************************************************************
  sensors_wrap.c
  
  Buttons wrapper implementation.
  
  Written by J.Polyakova.
*******************************************************************************/

#include "sensors_cntrl_fxns.h"
#include <sensors.h>
#include "com_assert.h"           
#include <tos.h>

static struct Sensors* sensors;

/*******************************************************************************
  Description: inits sensors
  	
  Parameters: pSensors - sensors object reference.
  	
  Returns: nothing.
*******************************************************************************/
void sensors_init(struct Sensors* pSensors)
{
  pSensors->state[0] = SENSOR_FREE_STATE;
  pSensors->state[1] = SENSOR_FREE_STATE;
  pSensors->data[0] = 0;
  pSensors->data[1] = 0;
  sensors=pSensors;
} 
 
/*******************************************************************************
  Description: open sensor.
  	
  Parameters: pSensors - sensors object reference,
              id - sensor id.
  	
  Returns: nothing.
*******************************************************************************/
static void open(struct Sensors* pSensors, Uint8 id)
{
  pSensors->state[id-2] = SENSOR_FREE_STATE;
  
  sensor_open(id);
}

/*******************************************************************************
  Description: close sensor.
  	
  Parameters: pSensors - sensors object reference,
              id - sensor id.
  	
  Returns: nothing.
*******************************************************************************/
static void close(struct Sensors* pSensors, Uint8 id)
{
  pSensors->state[id-2] = SENSOR_FREE_STATE;

  sensor_close(id);
}

/*******************************************************************************
  Description: event from temperature sensor.

  Parameters: result - result of sensor operation,
              data - sensor data
  
  Returns: nothing
*******************************************************************************/
static void tempDataReady(bool result, float data)
{
  result_t res;
  if (result == true)
  {
    sensors->state[0] = SENSOR_FREE_STATE;
    sensors->data[0] = data;  
  }
  else
  {
    res = sensor_getData(SENSOR_TEMPERATURE, tempDataReady);        
    if (res == FAIL) ASSERT(false);
  }
}

/*******************************************************************************
  Description: event from temperature sensor.

  Parameters: result - result of sensor operation,
              data - sensor data
  
  Returns: nothing
*******************************************************************************/
static void batDataReady(bool result, float data)
{
  result_t res;
  if (result == true)
  {
    sensors->state[1] = SENSOR_FREE_STATE;
    sensors->data[1] = data;
  }
  else
  {
    res=sensor_getData(SENSOR_BATTERY, batDataReady);       
    if (res == FAIL) ASSERT(false);   
  }
}

/*******************************************************************************
  Description: start sensor measurement.
  	
  Parameters: pSensors - sensors object reference,
              id - sensor id.
  	
  Returns: false if sensor busy, true otherwise.
*******************************************************************************/
static Bool start(struct Sensors* pSensors, Uint8 id)
{
  result_t res;
  if (pSensors->state[id-2] == SENSOR_FREE_STATE)
  {
    pSensors->state[id-2] = SENSOR_BUSY_STATE;
    if (id == SENSOR_TEMPERATURE)
    {
      res=sensor_getData(id, tempDataReady);
      if (res == FAIL) ASSERT(false);   
    }
    else
      if (id == SENSOR_BATTERY)
      {
        res = sensor_getData(id, batDataReady);       
        if (res == FAIL) ASSERT(false);   
      }
    return true;
  }
  else return false;
}

/*******************************************************************************
  Description: get sensor's data.
  	
  Parameters: pSensors - sensors object reference,
              id - sensor id
              data - pointer to variable contains sensor data.
  	
  Returns: true if data available, false otherwise.
*******************************************************************************/
static Bool get(struct Sensors* pSensors, Uint8 id, SensorsData* data)
{
  if (pSensors->state[id-2] == SENSOR_FREE_STATE)
  {
    *data = pSensors->data[id-2];
    return true;
  }
  else return false;
}

/*******************************************************************************
  Description: provides reference to Sensors interface implementation.
  	
  Parameters: pSensorsFxns - sensors interface implementation reference.
  	
  Returns: nothing.
*******************************************************************************/
void sensors_getFxns(SensorsFxns* pSensorsFxns)
{
  pSensorsFxns->open  = open;
  pSensorsFxns->close = close;
  pSensorsFxns->start = start;
  pSensorsFxns->get   = get;
}

// eof sensors_wrap.c
