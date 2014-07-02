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
    This module provides access to tsl2550 the sensor.
****************************************************************/

#include <sensors.h>
#include <tsl2550.h>
#include <i2cpacket.h>
#include <i2c.h>
#include <atomic.h>
#include <apptimer.h>

#define TSL_DEVICE_ADDRESS 0x39
#define TSL_READ_CHANNEL_0_COMMAND 0x43
#define TSL_READ_CHANNEL_1_COMMAND 0x83
#define TSL_POWERDOWN_COMMAND      0x00
#define TSL_POWERUP_COMMAND        0x03
#define TSL_EXTENDED_RANGE_COMMAND 0x1D
#define TSL_STANDARD_RANGE_COMMAND 0x18

// states of module
typedef enum
{
  IDLE,             // idle
  LIGHTSENSOR,      // performs request at the visible sensor
  IRLIGHTSENSOR     // performs request at the ifrared sensor
}lightSensorStates_e;

lightSensorStates_e tsl2550__state; // state of module
bool tsl2550__result; // result of last operation on i2c interface

uint8_t tsl2550_mode = TSL2550_STANDART_RANGE_MODE; // work mode of the tsl2550
uint16_t tsl2550_IrData = 0; // infrared  data
uint16_t tsl2550_Data = 0;   // visible and infrared data
 
int appTimer__ID; // appTimer descriptor
static I2CMode_t i2cMode = {.clockrate = I2C_CLOCK_RATE_62 }; // parameter of i2c interface

void (*dataReady)(bool error, float data); // data callback pointer
void i2cPacket_writeDone(bool result); 
 
/*******************************************************************************
  Description:  Inits component.

  Parameters:   none. 

  Returns:      none.
*******************************************************************************/
result_t tsl2550_init() 
{
  static bool flag = FALSE;
  if( flag == TRUE ) return;
  flag = TRUE;
  appTimer_init();    
  tsl2550__state = IDLE;
  return SUCCESS;
}

/*******************************************************************************
  Description:  Opens the component to use.

  Parameters:   none. 

  Returns:      SUCCESS - the component is ready to been use.
                FAIL - otherwise.
*******************************************************************************/
result_t tsl2550__open()
{
  tsl2550_init();
  if( tsl2550__state == IDLE) return SUCCESS;
  return FAIL;
}

/*******************************************************************************
  Description:  Performs the test if the component have completed request.

  Parameters:   none.

  Returns:      FAIL - the previous request is not completed.
                SUCCES - otherwise.
*******************************************************************************/
result_t tsl2550__close()
{
  if( tsl2550__state == IDLE) return SUCCESS;
  return FAIL;
}

/*******************************************************************************
  Description:  Task about the read request has been completed.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void post_dataReady()
{
  ATOMIC_SECTION_ENTER
  tsl2550__state = IDLE;
  appTimer_close( appTimer__ID );
  i2cpacket_close();
  ATOMIC_SECTION_LEAVE
  if( tsl2550__result == TRUE )
  {
    dataReady(TRUE, (float)tsl2550_Data * 0.46 * exp( (-3.13) * ((float)tsl2550_IrData/(float)tsl2550_Data) ));
  }
  else
  {
    dataReady(FALSE, 0);
  }
}

/*******************************************************************************
  Description:  The notice on that the packet has been read.

  Parameters:   result - TRUE if successfully read.

  Returns:      none.
*******************************************************************************/
void i2cPacket_readDone(bool result)
{
  const uint16_t ChordValue[] = {0, 16, 49, 115, 247, 511, 1039, 2095};
  result_t res;
  uint16_t i;
  if(result == TRUE)
  { 
    switch( tsl2550__state ) 
    {
      case IRLIGHTSENSOR:
        if(!(tsl2550_IrData & 0x80))
        {// VALID bit is false
          tsl2550__result = FALSE;
          TOS_post(post_dataReady);
          return;
        }				
        i = (tsl2550_IrData & 0x70) >> 4;
        tsl2550_IrData = ChordValue[i] + (uint16_t)(1u << i) * (uint16_t)(tsl2550_IrData & 0x0F);

        tsl2550__state = LIGHTSENSOR;
	tsl2550_Data = TSL_READ_CHANNEL_0_COMMAND;      // get visible light
    	if( i2cpacket_write( TSL_DEVICE_ADDRESS,        // address 
                             1,                         // uint8_t in_length, 
                             (uint8_t*)(&tsl2550_Data), // uint8_t *in_data, 
                             i2cPacket_writeDone        // void (*f)(bool result) 
                           ) == FAIL
          )
        {
           tsl2550__result = FALSE;
           TOS_post(post_dataReady);
        }
	break;
        case LIGHTSENSOR:
          if(!(tsl2550_Data & 0x80))
          {//VALID bit is false
            tsl2550__result = FALSE;
            TOS_post(post_dataReady);
            return;
          }
          i = (tsl2550_Data & 0x70) >> 4;
          tsl2550_Data = ChordValue[i] + (uint16_t)(1u << i) * (uint16_t)(tsl2550_Data & 0x0F);
          tsl2550__result = TRUE;
          TOS_post(post_dataReady);
	break;

	default:{}
      }//end switch
    }
    else
    {// 
      tsl2550__result = FALSE;
      TOS_post(post_dataReady);
    }
}

/*******************************************************************************
  Description:  The notice on that the set interval is counted out.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void appTimer_fired()
{
  result_t res;
  switch( tsl2550__state ) 
  {
    case IRLIGHTSENSOR:
      res = i2cpacket_read(TSL_DEVICE_ADDRESS,           // uint8_t id, 
                           1,                            // uint8_t in_length, 
                           (uint8_t*)(&tsl2550_IrData),  // uint8_t *data, 
                           i2cPacket_readDone            // void (*f)(bool result)
                          );
        break;

    case LIGHTSENSOR:
      res = i2cpacket_read(TSL_DEVICE_ADDRESS, // uint8_t id, 
                           1,               // uint8_t in_length, 
                           (uint8_t*)(&tsl2550_Data),// uint8_t *data, 
                           i2cPacket_readDone        // void (*f)(bool result)
                          );
    break;
    default:{;}
  }
  if( res == FAIL )
  {
    tsl2550__result = FALSE;
    TOS_post(post_dataReady);
  }
}

/*******************************************************************************
  Description:  The notice on that the packet has been written down.

  Parameters:   result - contains result of operation. 

  Returns:      none.
*******************************************************************************/
void i2cPacket_writeDone(bool result)
{
  result_t res;
  if(result == TRUE)
  { 
    switch( tsl2550__state ) 
    {
      case IRLIGHTSENSOR:
      case LIGHTSENSOR:
        if(tsl2550_mode == TSL2550_STANDART_RANGE_MODE)
          res = appTimer_start(appTimer__ID, TIMER_ONE_SHOT_MODE, 400);
        else
          res = appTimer_start(appTimer__ID, TIMER_ONE_SHOT_MODE, 80);
        if( res == FAIL )
        {
          tsl2550__result = FALSE;
          TOS_post(post_dataReady);
        } 
      break;
      default:{}
      }// end switch		
  } // if result == true
  else
  {            
    tsl2550__result = FALSE;
    TOS_post(post_dataReady);
  } 
}

/*******************************************************************************
  Description:  Gets data from tsl2550 sensor.

  Parameters:   f - callback.

  Returns:      FAIL - previous request has not been completed yet.
                SUCCESS - in other case.
*******************************************************************************/
result_t tsl2550__getData( void (*f)(bool result, float data) )
{
  uint8_t i;
  if( tsl2550__state != IDLE ) 
  {
    return FAIL;
  }
  if(i2cpacket_open( &i2cMode ) == FAIL) return FAIL;
  if( (appTimer__ID = appTimer_open( appTimer_fired )) < 0)
  {
    return FAIL;
  }
  ATOMIC_SECTION_ENTER
    tsl2550__state = IRLIGHTSENSOR;
  ATOMIC_SECTION_LEAVE
  dataReady = f;
  tsl2550_IrData = TSL_READ_CHANNEL_1_COMMAND;    // get infrared light
  if( i2cpacket_write( TSL_DEVICE_ADDRESS,        // address 
                       1,                         // uint8_t in_length, 
                       (uint8_t*)(&tsl2550_IrData), // uint8_t* in_data, 
                       i2cPacket_writeDone        // void (*f)(bool result) 
                     ) == FAIL
    )
  {
    ATOMIC_SECTION_ENTER
    tsl2550__state = IDLE;
    appTimer_close( appTimer__ID );
    i2cpacket_close();
    ATOMIC_SECTION_LEAVE
    return FAIL;
  }
return SUCCESS;
}

// endof tsl2550.c
