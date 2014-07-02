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

/*******************************************************************************
  eeprom.h

  EEPROM control header file.
*******************************************************************************/

#ifndef EEPROM_H_
#define EEPROM_H_

#define EEPROM_DATA_MEMORY_SIZE 0x1000u

// the return codes of eeprom module
typedef enum
{
  EEPROM_SUCCESS_STATUS,
  EEPROM_ADDR_TOO_FAR_STATUS,
  EEPROM_LENGTH_TOO_LONG_STATUS,
  EEPROM_HARDWARE_ERROR_STATUS,
  EEPROM_OPERATION_OVERFLOW_STATUS,
  EEPORM_INVALID_DATA_POINTER_STATUS
}EEPROMStatus_t;

typedef struct
{
  uint16_t addr;           // eeprom address
  uint8_t *data;           // pointer to data memory
  uint16_t length;         // number of bytes
  EEPROMStatus_t status;   // contains result of operation
}EEPROMParams_t;

/*******************************************************************************
  Description: init EEPROM

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
result_t eeprom_init();

/*******************************************************************************
  Description:  Writes the data block to the EEPROM.

  Parameters:   params - address of parameters structure.
                writeDone - callback.

  Returns:      SUCCESS -  no operations is performing and length is not 0
                FAIL - in other case.
*******************************************************************************/
result_t eeprom_write(EEPROMParams_t *params, void (*writeDone)());

/*******************************************************************************
  Description:  Reads data block from the EEPROM.

  Parameters:   params - address of parameters structure.

  Returns:      SUCCESS -  no operations is performing and length is not 0 
                FAIL - in other case.
*******************************************************************************/
result_t eeprom_read(EEPROMParams_t *params, void (*readDone)());

#define eeprom__init eeprom_init
#define eeprom__write eeprom_write
#define eeprom__read eeprom_read

#endif	/* EEPROM_H_ */
// eof EEPROM.h

