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
  The common interface to sensors.
****************************************************************/

#include <tos.h>
#include <sensors.h>
#include <tsl2550.h>
#include <lm73.h>
#include <battery.h>
#include <pwrctrl.h>

/*******************************************************************************
  Description:  Inits id sensor.

  Parameters:   id - sensor descriptor.

  Returns:      FAIL - if there is hardware error or there is no requested sensor 
                SUCCESS - in other case.
*******************************************************************************/
result_t sensor_open(uint8_t id)
{
    switch( id ) 
    {
    case SENSOR_LIGHT:
      if( tsl2550__open() == SUCCESS )
      {
        hpl__peripherial__on( id ); 
        return SUCCESS; 
      }
    break;

    case SENSOR_TEMPERATURE:
      if( lm73__open() == SUCCESS )
      {
        hpl__peripherial__on( id );
        return SUCCESS;
      }
    break;

    case SENSOR_BATTERY:
      if( battery__open() == SUCCESS )
      {
        hpl__peripherial__on( id );
        return SUCCESS;
      }
    break;

    default:{ return FAIL; }
  }
return FAIL;
}

/*******************************************************************************
  Description:  Closes id sensor.

  Parameters:   id - sensor descriptor.

  Returns:      FAIL - if there is hardware error, there is no requested sensor, 
                       there is uncompleted getData request.
                SUCCESS  - in other case.
*******************************************************************************/
result_t sensor_close(uint8_t id)
{ 
    switch( id ) 
    {
    case SENSOR_LIGHT:
      if( tsl2550__close() == SUCCESS )
      {
        hpl__peripherial__off( id ); 
        return SUCCESS; 
      }
    break;

    case SENSOR_TEMPERATURE:
      if( lm73__close() == SUCCESS )
      {
        hpl__peripherial__off( id );
        return SUCCESS;
      }
    break;

    case SENSOR_BATTERY:
      if( battery__close() == SUCCESS )
      {
        hpl__peripherial__off( id );
        return SUCCESS;
      }
    break;

    default:{ return FAIL; }
    }
return FAIL;
}

/*******************************************************************************
  Description:  Gets data from id sensor.

  Parameters:   id - sensor descriptor.
                dataReady - pointer to the sensor data handler
                            result -  the result requested operation. 
                                      TRUE - there is no error,
                                      FALSE - there is error.
                            data - sensor data.
                
  Returns:      FAIL - if there is no such sensor or previous request has not been 
                        completed yet.
                SUCCESS - in other case.
*******************************************************************************/
result_t sensor_getData(uint8_t id, void (*dataReady)(bool result, float data) )
{
  switch (id)
  {
    case SENSOR_LIGHT:
      return tsl2550__getData(dataReady);

    case SENSOR_TEMPERATURE:
      return lm73__getData(dataReady);

    case SENSOR_BATTERY:
      return battery__getData(dataReady);

    default:{ return FAIL; }
  }
}
// eof sensors.c
