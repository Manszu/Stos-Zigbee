/*									tab:4
 *
 *
 * "Copyright (c) 2000-2002 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 */
/*									tab:4
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.  By
 *  downloading, copying, installing or using the software you agree to
 *  this license.  If you do not agree to this license, do not download,
 *  install, copy or use the software.
 *
 *  Intel Open Source License 
 *
 *  Copyright (c) 2002 Intel Corporation 
 *  All rights reserved. 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 * 
 *	Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *	Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *      Neither the name of the Intel Corporation nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INTEL OR ITS
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 */
/*
 *
 * Authors:		Phil Buonadonna
 * Date last modified:  12/19/02
 *
 * HPLI2CM: Hardware based I2C for the ATmega128 series microcontroller.
 * Note: Hardware HPLI2C USES DIFFERENT PINS than the software based
 * I2CM used on earlier microcontrollers. Edit I2CC.nc to achieve the desired
 * configuration connection to hardware or software support.
 *
 */

/*===============================================================
Provides the functionality of TWI.
=================================================================*/

#include <hardware.h>
#include <i2c.h>

// define TWI device status codes. 
enum 
{
  TWS_BUSERROR     = 0x00,
  TWS_START        = 0x08,
  TWS_RSTART       = 0x10,
  TWS_MT_SLA_ACK   = 0x18,
  TWS_MT_SLA_NACK  = 0x20,
  TWS_MT_DATA_ACK  = 0x28,
  TWS_MT_DATA_NACK = 0x30,
  TWS_M_ARB_LOST   = 0x38,
  TWS_MR_SLA_ACK   = 0x40,
  TWS_MR_SLA_NACK  = 0x48,
  TWS_MR_DATA_ACK  = 0x50,
  TWS_MR_DATA_NACK = 0x58
};

/*******************************************************************************
  Description: Silly task to signal when a stop condition is completed.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void I2C_task() 
{
  loop_until_bit_is_clear(TWCR,TWSTO);
  i2c_sendEndDone();
}

/*******************************************************************************
  Description: Inits TWI module. Setup teh speed of TWI.

  Parameters: i2cMode - the speed of TWI.

  Returns: FAIL - unsupported speed.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_init( const I2CMode_t *i2cMode ) 
{
  TWCR = 0x00;
  // Set bit rate
  switch(i2cMode->clockrate)
  {
    case I2C_CLOCK_RATE_250: 
    case I2C_CLOCK_RATE_125:
    case I2C_CLOCK_RATE_62:
      TWBR = i2cMode->clockrate;
    break;
    default:
    {
      return FAIL;
    }
  }
  // Enable TWI interface.
  TWCR = ((1 << TWINT) | (1 << TWEN) | (1 << TWIE));
  return SUCCESS;
}

/*******************************************************************************
  Description: Directs TWI to send start condition.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendStart() 
{

  TWCR |= (1 << TWSTA);
  TWCR |= (1 <<TWINT);
  return SUCCESS;
}

/*******************************************************************************
  Description: Directs TWI to send stop condition.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendEnd() 
{

  TWCR |= (1 << TWSTO);
  TWCR |= (1 << TWINT);

  TOS_post( I2C_task );
  return SUCCESS;
}

/*******************************************************************************
  Description: Begins to read an byte from TWI.

  Parameters: ack - defines the need to send ACK after an byte was recieved.

  Returns: FAIL - the TWI is busy or the TWI improperly initialized.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_read(bool ack) 
{

  if (bit_is_clear(TWCR,TWINT))
   return FAIL;
  
  if (ack) 
    TWCR |= (1 << TWEA);
  else
    TWCR &= ~(1 << TWEA);

  TWCR |= (1 << TWINT);  // Trigger the TWI
  return SUCCESS;
}

/*******************************************************************************
  Description: Begins the writing an byte to TWI.

  Parameters: data - an byte for the sending.

  Returns: FAIL -  the TWI is busy or the TWI improperly initialized.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_write(char data) 
{
  if(bit_is_clear(TWCR,TWINT))
    return FAIL;
  
  TWDR =  data;
  TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWIE);
  return SUCCESS;
}

/*******************************************************************************
  Description: Interrupt handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
SIGNAL(SIG_2WIRE_SERIAL) 
{

  switch (TWSR & 0xF8) 
  {
    case TWS_BUSERROR:
      TWCR = ((1 << TWSTO) | (1 << TWINT));  // Reset TWI
      break;

    case TWS_START:
    case TWS_RSTART:
      i2c_sendStartDone();
      break;
      
    case TWS_MT_SLA_ACK:
    case TWS_MT_DATA_ACK:
      i2c_writeDone(TRUE);
      break;

    case TWS_MT_SLA_NACK:
    case TWS_MT_DATA_NACK:
      i2c_writeDone(FALSE);
      break;
      
    case TWS_MR_SLA_ACK:
      i2c_writeDone(TRUE);
      break;

    case TWS_MR_SLA_NACK:
      i2c_writeDone(FALSE);
      break;

    case TWS_MR_DATA_ACK:
    case TWS_MR_DATA_NACK:
      i2c_readDone( TWDR );
      break;

    default:
      break;
  }
}

// eof i2c.c
