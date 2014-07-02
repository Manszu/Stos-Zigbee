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

/******************************************************************************
 One wire interface - header file
******************************************************************************/

#ifndef W1_H_
#define W1_H_

#include <tos.h>

#define W1_ANY_FAMILY 0x00
#defineD S2411 0x01

// w1 status
typedef enum
{
  W1_NO_DEVICE_STATUS, // There is no device on the bus 
  W1_SUCCESS_STATUS,   // At least one device is on the bus
  W1_INVALID_CRC       // Invalid CRC was read during the device search operation
} W1Status;

/*******************************************************************************
  Description: Resets all devices connected to the bus. Function asserts on 
               the bus reset pulse and detects presence pulse. The result is 
               contained in r24.

  Parameters: nothing.

  Returns: W1_SUCCESS_STATUS - If device(s) was(were) detected.
           W1_NO_DEVICE_STATUS - If device(s) was(were) not detected.
******************************************************************************/
W1Status w1_reset(void);

/*******************************************************************************
  Description: Reads byte from the bus.

  Parameters: nothing.

  Returns: byte read from the bus.
*******************************************************************************/
uint8_t w1_read (void);

/*******************************************************************************
  Description: Writes byte to the bus.

  Parameters: value - byte that should be written to the bus.

  Returns: nothing.
*******************************************************************************/
void w1_write (uint8_t value);

/*******************************************************************************
  Description:  1-Wire search procedure with search ROM command only

  Parameters:   family   - 8-bit family code.
                data     - pointer of SRAM where are stored the 8 bytes ROM 
                          codes returned by the devices.
                count    - number of devices wish to find.
                actCount - number of devices have been found.
 

  Returns:      W1_SUCCESS_STATUS   - if at least one device has been found.
                W1_NO_DEVICE_STATUS - if there are no any devices presented 
                                      on the bus with specified family code.
                W1_INVALID_CRC      - if during searching invalid CRC has 
                                      been read and no devices with 
                                      spicified family code has been found.
*******************************************************************************/
W1Status w1_deviceSearch(uint8_t family, uint8_t *data, uint8_t count, uint8_t *actCount);

/*******************************************************************************
  Description:  1-Wire search procedure with alarm search command only

  Parameters:   family   - 8-bit family code.
                data     - pointer of SRAM where are stored the 8 bytes ROM 
                          codes returned by the devices.
                count    - number of devices wish to find.
                actCount - number of devices have been found.

  Returns:      W1_SUCCESS_STATUS   - if at least one device has been found.
                W1_NO_DEVICE_STATUS - if there are no any devices presented 
                                      on the bus with specified family code.
                W1_INVALID_CRC      - if during searching invalid CRC has 
                                      been read and no devices with 
						              spicified family code has been found.
*******************************************************************************/
W1Status w1_alarmSearch(uint8_t family,
                        uint8_t *data,
			uint8_t count,
			uint8_t *actCount);

/*******************************************************************************
  Description:  Calculating 1-Wire 8-bit CRC

  Parameters:   data   - data buffer pointer.
                length - data length.

  Returns:      CRC value based on polynomial x^8 + x^5 + x^4 + 1
*******************************************************************************/
uint8_t w1_crc(uint8_t *data, uint8_t length);

#endif //W1_H_
// eof w1.h
