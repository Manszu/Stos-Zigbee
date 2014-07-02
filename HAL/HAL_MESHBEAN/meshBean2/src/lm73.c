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

/********************************************************************************
  This module provides access to lm73 the sensor.
  Continues conversion released only.
********************************************************************************/

#include <sensors.h>
#include <lm73.h>
#include <i2cpacket.h>
#include <atomic.h>
#include <i2c.h>

#define LM73_DEVICE_ADDRESS  (0x92 >> 1)


#define LM73_DATA_REGISTER 0x00
#define LM73_CONFIGURATION_REGISTER 0x01
#define LM73_UPPER_LIMIT_REGISTER 0x02
#define LM73_LOWER_LIMIT_REGISTER 0x03
#define LM73_CONTROL_STATUS_REGISTER 0x04
#define LM73_IDENTIFICATION_REGISTER 0x07

// states 
typedef enum
{
  IDLE,      // idle
  DATA       // performs request
}lm73States_e;

lm73States_e lm73__state; // Monitors current state
uint32_t lm73__data;      // Contains the result of sampling
bool lm73__result;        // Result of operation. TRUE - there are no errors, FALSE - in other case.

void (*lm73__dataReady)(bool error, float data); // callback 
static I2CMode_t i2cMode = {.clockrate = I2C_CLOCK_RATE_62 }; // the work mode of i2c interface
void lm73__i2cPacket_writeDone(bool result);

/*******************************************************************************
  Description:  Inits lm73 component.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void lm73__init() 
{       
  static bool flag = FALSE;
  if( flag == TRUE ) return;
  flag = TRUE;
  lm73__state = IDLE;
}

/*******************************************************************************
  Description:  Opens the component to use.

  Parameters:   none.

  Returns:      SUCCESS - the component is ready to been use.
                FAIL - otherwise.
*******************************************************************************/
result_t lm73__open()
{
  lm73__init();
   if( lm73__state == IDLE ) return SUCCESS;
  return FAIL;
}

/*******************************************************************************
  Description:  Performs the test if the component have completed request.

  Parameters:   none.

  Returns:      FAIL - the previous request is not completed.
                SUCCES - otherwise.
*******************************************************************************/
result_t lm73__close()
{
   if( lm73__state == IDLE ) return SUCCESS;
  return FAIL;
}  

/*******************************************************************************
  Description:  Task about the read request has been completed.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void lm73__post_readDone()
{
  int16_t i;
  i2cpacket_close(); // free
  if(lm73__result == FALSE)
  { // there were some errors on the i2c interface
    ATOMIC_SECTION_ENTER
      lm73__state = IDLE; 
    ATOMIC_SECTION_LEAVE
    lm73__dataReady(FALSE, 0);
    return;
  }

  switch( lm73__state )
  {
    case DATA:
      ATOMIC_SECTION_ENTER
        lm73__state = IDLE; 
      ATOMIC_SECTION_LEAVE
      i = (uint8_t)lm73__data; i <<= 8;
      i |= ((lm73__data >> 8) & 0x00FF);
      lm73__dataReady( TRUE, (float)(i) * (0.03125/4.0) );
    break;
    default:{;}
  }
}

/*******************************************************************************
  Description:  The notice on that the packet has been read.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void lm73__i2cPacket_readDone(bool result)
{
  lm73__result = result; // stores the result of operation
  TOS_post( lm73__post_readDone ); // frees interrupt handler
}

/*******************************************************************************
  Description:  Task about the write request has been completed.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void lm73__post_writeDone()
{	
  result_t res;
  const uint8_t NumBytesForRead[]={ 2, // size of TEMPERATURE DATA REGISTER
                                    1, // size of CONFIGURATION REGISTER
                                    2, // size of UPPER-LIMIT REGISTER
                                    2, // size of LOWER-LIMIT REGISTER
                                    1, // size of CONTROL/STATUS REGISTER
                                    0,
                                    0,
                                    2  // size of IDENTIFICATION REGISTER
                                  };     
  if(lm73__result == FALSE)
  { // there were some errors on the i2c interface
    i2cpacket_close(); // free
      ATOMIC_SECTION_ENTER
        lm73__state = IDLE; 
      ATOMIC_SECTION_LEAVE
    lm73__dataReady(FALSE, 0);
    return;
  }
      
  switch( lm73__state )
  {
    case DATA:
         res = i2cpacket_read( LM73_DEVICE_ADDRESS,  // uint8_t id, 
                               2,                    // uint8_t in_length, 
                               (uint8_t*)(&lm73__data), // uint8_t *data, 
                               lm73__i2cPacket_readDone // void (*f)(bool result)
	                     );
    break;
    default:{;}
  }

  if( res == FALSE )
  {
    lm73__result = FALSE; 
    TOS_post( lm73__post_readDone );
  }
}

/*******************************************************************************
  Description:  The notice on that the packet has been written down.

  Parameters:   none.

  Returns:      none.
*******************************************************************************/
void lm73__i2cPacket_writeDone(bool result)
{	
  lm73__result = result; 
  TOS_post( lm73__post_writeDone ); // frees interrupt handler
}

/*******************************************************************************
  Description:  Gets data from lm73 sensor.

  Parameters:   f - callback.

  Returns:      FAIL -  the previous request has not been completed yet,
                        the address of callback is 0, i2c interface is busy,
	                    there is error on i2c interface.
                SUCCESS - other case.
*******************************************************************************/
result_t lm73__getData( void (*f)(bool result, float data) )
{   
  if( lm73__state != IDLE ) 
  {
      return FAIL;
  }
  if( !f ) return FAIL;
  if(i2cpacket_open( &i2cMode ) == FAIL) return FAIL;
  ATOMIC_SECTION_ENTER
    lm73__state = DATA;  
  ATOMIC_SECTION_LEAVE
  lm73__dataReady = f;
  lm73__data = LM73_DATA_REGISTER;          // selects temperature register
  if( i2cpacket_write( LM73_DEVICE_ADDRESS, // address 
                       1,                   // uint8_t in_length, 
                       (uint8_t*)(&lm73__data),  // uint8_t* in_data, 
                       lm73__i2cPacket_writeDone // void (*f)(bool result) 
                     ) == FAIL 
    )
  {
    ATOMIC_SECTION_ENTER
    lm73__state = IDLE;
      i2cpacket_close();
    ATOMIC_SECTION_LEAVE
    return FAIL;
  }
return SUCCESS;
}
// endof lm73.c
