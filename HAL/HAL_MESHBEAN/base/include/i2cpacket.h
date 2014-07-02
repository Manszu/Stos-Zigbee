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
 
#ifndef I2CPACKETM_H
#define I2CPACKETM_H

typedef enum
{
  I2C_CLOCK_RATE_250 = 0, // 250 Kb/s clock rate
  I2C_CLOCK_RATE_125 = 8, // 125 Kb/s clock rate
  I2C_CLOCK_RATE_62  = 30 // 62.5 Kb/s clock rate
} I2CClockRate_t;

// TWI mode
typedef struct 
{
  I2CClockRate_t clockrate; // clock rate
}I2CMode_t;

/*******************************************************************************
  Description: Opens resource.

  Parameters: i2cMode - mode.

  Returns: FAIL - resource has been opened or unsupported mode.
           SUCCESS - other case.
*******************************************************************************/
result_t i2cpacket_open( const I2CMode_t *i2cMode );

/*******************************************************************************
  Description: Closes resource. 

  Parameters: nothing.

  Returns: FAIL - resource has not been opened.
           SUCCESS - other case.
*******************************************************************************/
result_t i2cpacket_close();

/*******************************************************************************
  Description: Writes a series of bytes out to the TWI bus.

  Parameters: in_length - number of bytes to be written to the bus.
              in_data - pointer to the data.
              f - callback.

  Returns: SUCCESS - the bus is free and the request is accepted.
           FAIL - other case.
*******************************************************************************/
result_t i2cpacket_write(uint8_t id, 
                         uint8_t in_length, 
                         const uint8_t* in_data, 
                         void (*f)(bool result));

/*******************************************************************************
  Description: Reads a series of bytes out to the TWI bus.

  Parameters: in_length - number of bytes to be read from the bus.
              in_data - the pointer to data buffer.

  Returns: SUCCESS - the bus is free and the request is accepted.
           FAIL - other case.
*******************************************************************************/
result_t i2cpacket_read(uint8_t id, 
                        uint8_t in_length, 
                        uint8_t *data, 
                        void (*f)(bool result));

#endif
// eof i2cpacket.h
