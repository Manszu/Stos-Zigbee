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
The implementation of the one wire interface.
******************************************************************************/

#include <hplw1.h>
#include <w1.h>

#define ROM_BYTE_LEN 8
#define ROM_BIT_LEN 64
#define CRC_POLINOM 0x8C
#define SEARCH_ROM_CMD 0xF0
#define SEARCH_ALRM_CMD 0xEC

W1Status w1_commSearch(uint8_t cmd,
                       uint8_t family,
                       uint8_t *data,
                       uint8_t count,
                       uint8_t *actCount);
W1Status w1_search(uint8_t cmd);
uint8_t crc8(uint8_t *ptr, uint8_t n);

// 8-byte buffer that contains the current ROM registration 
// number discovered
static uint8_t ROM_NO[ROM_BYTE_LEN];
// Bit index that identifies from which bit the next search 
// discrepancy check should start
static uint8_t LastDiscrepancy;
// Flag to indicate previos search was the last device
static uint8_t LastDeviceFlag;

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
W1Status w1_deviceSearch(uint8_t family,
                         uint8_t *data,
						 uint8_t count,
						 uint8_t *actCount)
{
  return w1_commSearch(SEARCH_ROM_CMD, family, data, count, actCount);
}

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
                        uint8_t *actCount)
{
  return w1_commSearch(SEARCH_ALRM_CMD, family, data, count, actCount);
}

/*******************************************************************************
  Description:  1-Wire search procedure for all devices discovering

  Parameters:   cmd      - ROM function command.
                family   - 8-bit family code.
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
W1Status w1_commSearch(uint8_t cmd,
                       uint8_t family,
                       uint8_t *data,
                       uint8_t count,
                       uint8_t *actCount)
{
  W1Status w1_result;
  uint8_t i;
  // Reset the searh state
  LastDiscrepancy = 0;
  LastDeviceFlag = 0;
  // Setup family code
  if (family != W1_ANY_FAMILY)
  {
    ROM_NO[0] = family;
	LastDiscrepancy = ROM_BIT_LEN + 1;
	for (i = 1; i < ROM_BYTE_LEN; i++) ROM_NO[i] = 0;
  }
  i = 0;
  // Discovering
  do 
  {
    w1_result = w1_search(cmd);
	if (w1_result != W1_SUCCESS_STATUS) break;
	// There is some device with specified family code
	if ((family == ROM_NO[0]) || (family == W1_ANY_FAMILY))
	{
	  *(uint64_t *)(data + 8 * i) = *(uint64_t *)ROM_NO;
	  i++;
    }
	// There are not any devices with specified family code
	else {
	  w1_result = W1_NO_DEVICE_STATUS;
	  break;
    }
  }
  while (!LastDeviceFlag && (i < count));
  *actCount = i;
  if (i != 0) 
    return W1_SUCCESS_STATUS;
  else 
    return w1_result;
}

/*******************************************************************************
  Description:  1-Wire search procedure for one device discovering

  Parameters:   cmd - ROM function command.

  Returns:      W1_SUCCESS_STATUS   - if device has been found, ROM number in ROM_NO buffer.
                W1_NO_DEVICE_STATUS - if device has not been found.
                W1_INVALID_CRC      - if during searching invalid CRC has been read.
*******************************************************************************/
W1Status w1_search(uint8_t cmd)
{
  uint8_t id_bit_number;
  uint8_t last_zero, rom_byte_number, search_result;
  uint8_t id_bit, cmp_id_bit;
  uint8_t rom_byte_mask, search_direction;

  // initialize for search
  id_bit_number = 1;
  last_zero = 0;
  rom_byte_number = 0;
  rom_byte_mask = 1;
  search_result = 0;

  // 1-Wire reset
  if(w1_reset() == W1_NO_DEVICE_STATUS) {
    LastDiscrepancy = 0;
    LastDeviceFlag = 0;
    return W1_NO_DEVICE_STATUS;
  }

  // issue the search command
  w1_write(cmd);
  // search 64-bit uniqued registration number
  do
  {
    // read a bit and its complement
    id_bit = w1_read_bit();
    cmp_id_bit = w1_read_bit();

	// check for no devices on 1-wire
	if((id_bit == 1) && (cmp_id_bit == 1))
	  return W1_NO_DEVICE_STATUS;

	// all devices coupled have 0 or 1
    if(id_bit != cmp_id_bit) search_direction = id_bit;
	// there is descepancy
	else
	{
	  // if this discrepancy if before the Last Discrepancy
      // on a previous next then pick the same as last time (old branch)
      if (id_bit_number < LastDiscrepancy)
	    search_direction = ((ROM_NO[rom_byte_number] & rom_byte_mask) > 0);
	  // if equal to last pick 1, if not then pick 0 (new branch)
	  else search_direction = (id_bit_number == LastDiscrepancy);
	  // if 0 was picked then record its position in LastZero
	  if (search_direction == 0) last_zero = id_bit_number;
	}

	if (search_direction == 1) ROM_NO[rom_byte_number] |= rom_byte_mask;
	else ROM_NO[rom_byte_number] &= ~rom_byte_mask;
	w1_write_bit(search_direction);
	id_bit_number++;
	rom_byte_mask <<= 1;

	if (rom_byte_mask == 0)
	{
	  rom_byte_number++;
	  rom_byte_mask = 1;
	}
  }
  while (rom_byte_number < ROM_BYTE_LEN);

  // Invalid CRC
  if (w1_crc(ROM_NO, ROM_BYTE_LEN))
    return W1_INVALID_CRC;
  
  LastDiscrepancy = last_zero;
  // check for last device
  if (LastDiscrepancy == 0) LastDeviceFlag = 1;
  return W1_SUCCESS_STATUS;
}

/*******************************************************************************
  Description:  Calculating 1-Wire 8-bit CRC

  Parameters:   data   - data buffer pointer.
                length - data length.

  Returns:      CRC value based on polynomial x^8 + x^5 + x^4 + 1
*******************************************************************************/
uint8_t w1_crc(uint8_t *data, uint8_t length)
{
  uint8_t reg;
  uint8_t b;
  for (reg = 0; length > 0; length--, data++) 
  {
    for (b = 0; b < 8; b++)
	{
	  if ((((*data >> b) & 1) ^ (reg & 1)))
	  {
	    reg >>= 1;
		reg ^= CRC_POLINOM;
      }
	  else reg >>= 1;
    }
  }
  return reg;
}

// eof w1.c
