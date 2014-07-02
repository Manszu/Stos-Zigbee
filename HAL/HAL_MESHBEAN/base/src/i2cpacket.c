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

/*
 *
 * Authors:		Joe Polastre, Rob Szewczyk
 * Date last modified:  7/18/02
 *
 */


/*===============================================================
Provides the functionality for the writing and the reading 
of packets through TWI.
=================================================================*/

#include <tos.h>
#include <avrhardware.h>
#include <i2cpacket.h>
#include <i2c.h>

/* state of the i2c request */
typedef enum 
{
  IDLE = 99,
  I2C_START_COMMAND = 1,
  I2C_STOP_COMMAND = 2,
  I2C_STOP_COMMAND_SENT = 3,
  I2C_WRITE_ADDRESS = 10,
  I2C_WRITE_DATA = 11,
  I2C_READ_ADDRESS = 20,
  I2C_READ_DATA = 21,
  I2C_READ_DONE = 22
} i2cpacket_states_t;

uint8_t* i2c_data;  // bytes to write to the i2c bus 
uint8_t i2c_length; // length in bytes of the request 
uint8_t i2c_index;  // current index of read/write byte 
i2cpacket_states_t i2c_state;  // current state of the i2c request 
uint8_t i2c_addr;   // destination address 
  
void (*i2cPacket_done)(bool result); // callback
bool i2c_handler = TRUE;             // i2cpacket control

/*******************************************************************************
  Description: Opens resource.

  Parameters: i2cMode - mode.

  Returns: FAIL - resource has been opened or unsupported mode.
           SUCCESS - other case.
*******************************************************************************/
result_t i2cpacket_open(const I2CMode_t *i2cMode)
{
  if (i2c_handler == TRUE)
  {
    if (i2c_init(i2cMode) == FAIL)
    {
      return FAIL;
    }
    i2c_state = IDLE;
    i2c_index = 0;
    i2c_handler = FALSE;
    return SUCCESS;
  }
  return FAIL;
}

/*******************************************************************************
  Description: Closes resource. 

  Parameters: nothing.

  Returns: FAIL - resource has not been opened.
           SUCCESS - other case.
*******************************************************************************/
result_t i2cpacket_close()
{
  if (i2c_handler == FALSE)
  {
    i2c_handler = TRUE;
    i2c_state = IDLE;
    return SUCCESS;
  }
  return FAIL;
}

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
                         void (*f)(bool result)) 
{
  if ((i2c_state == IDLE) && (i2c_handler == FALSE) && f)
  {
    /*  reset variables  */
    i2c_addr = id;
    i2c_data = (uint8_t*)in_data;
    i2c_index = 0;
    i2c_length = in_length;
    i2cPacket_done = f;
  }
  else 
  {
    return FAIL;
  }

  i2c_state = I2C_WRITE_ADDRESS;
  if (i2c_sendStart())
  {
    return SUCCESS;
  }
  else
  {
    i2c_state = IDLE;
    return FAIL;
  }
}
 
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
                        void (*f)(bool result))  
{
  if ((i2c_state == IDLE) && (i2c_handler == FALSE) && f)
  {
    i2c_addr = id;
    i2c_index = 0;
    i2c_length = in_length;
    i2c_data = data;
    i2cPacket_done = f;
  }
  else 
  {
    return FAIL;
  }

  i2c_state = I2C_READ_ADDRESS;
  if (i2c_sendStart())
  {
    return SUCCESS;
  }
  else
  {
    i2c_state = IDLE;
    return FAIL;
  }
}

/*******************************************************************************
  Description: Notification that the start condition was sent.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendStartDone() 
{
  I2CMode_t i2cMode = {.clockrate = I2C_CLOCK_RATE_62};

  if (i2c_state == I2C_WRITE_ADDRESS)
  {
    i2c_state = I2C_WRITE_DATA;
    i2c_write(((i2c_addr << 1) + 0));
  }
  else if (i2c_state == I2C_READ_ADDRESS)
  {
    i2c_state = I2C_READ_DATA;
    i2c_write(((i2c_addr << 1) + 1));
    i2c_index++;
  }
  else
  { // abnormal
    i2c_state = IDLE;
    i2c_init(&i2cMode);
    if (i2cPacket_done)
      i2cPacket_done(FAIL);
  }
  return SUCCESS;
}

/*******************************************************************************
  Description: Notification that the stop condition was sent.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendEndDone() 
{
  i2c_state = IDLE;
  if (i2cPacket_done)
    i2cPacket_done(TRUE);
  return SUCCESS;
}

/*******************************************************************************
  Description: Notification of a byte sucessfully written to the bus.

  Parameters: result - contains result of previous operation.

  Returns: FAIL - there is the state error.
  SUCCESS - other case.
*******************************************************************************/
result_t i2c_writeDone(bool result) 
{
  I2CMode_t i2cMode = {.clockrate = I2C_CLOCK_RATE_62};
  if (i2c_state == IDLE)
  {
    i2c_init(&i2cMode);
    return FAIL;
  }
  if (result == FAIL) 
  {
    i2c_state = IDLE;
    if (i2cPacket_done)
      i2cPacket_done(FAIL);
    return FAIL;
  }
  if ((i2c_state == I2C_WRITE_DATA) && (i2c_index < i2c_length))
  {
    i2c_index++;
    if (i2c_index == i2c_length)
      i2c_state = I2C_STOP_COMMAND;
    return i2c_write(i2c_data[i2c_index - 1]);
  }
  else if (i2c_state == I2C_STOP_COMMAND)
  {
    i2c_state = I2C_STOP_COMMAND_SENT;
    return i2c_sendEnd();
  }
  else if (i2c_state == I2C_READ_DATA)
  {
    return i2c_read(TRUE); // send ack
  }
  else
  { // abnormal
    i2c_state = IDLE;
    i2c_init(&i2cMode);
    if (i2cPacket_done)
      i2cPacket_done(FAIL);
  }
  return SUCCESS;
}

/*******************************************************************************
  Description: Reads a byte off the TWI and adds it to the packet.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_readDone(char in_data)
{
  i2c_data[i2c_index - 1] = in_data;
  i2c_index++;
  if (i2c_index <= i2c_length)
  {
    i2c_read(TRUE); // send ACK
  }
  else
  {
    i2c_state = I2C_READ_DONE;
    i2c_sendEnd();
  }
  return SUCCESS;
}

// eof i2cpacket.c
