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
 eeprom_fxns.h
  
 EEPROM interface declaration.
 Written by P.Sobol.
*******************************************************************************/

#ifndef EEPROM_FXNS_H_
#define EEPROM_FXNS_H_

#include "com_types.h"
#include "com_list.h"

typedef enum
{
  EEPROM_STATUS_INVALID = 0,
  EEPROM_STATUS_SUCCESS,          // Command finished successfully
  EEPROM_STATUS_HARDWARE_ERROR,   // Hardware error occured while processing command
  EEPROM_STATUS_OUT_OF_RANGE      // Command address is out of range
} EepromStatus;

typedef enum
{
  EEPROM_CMD_ID_INVALID = 0,
  EEPROM_CMD_ID_READ,
  EEPROM_CMD_ID_WRITE
} EepromCmdId;

typedef struct
{
  ListItem listItem;
  EepromCmdId id;
  Uint16        startAddr; // Start address in EEPROM
  Uint8* data;
  Uint16 length;
  EepromStatus status;
} EepromCmdDescr;

struct Eeprom;

typedef struct
{
  struct Eeprom* pEeprom;
  
  /*****************************************************************************
    Description: Puts command to EEPROM.
                 Invalid cmdId, zero data length lead to ASSERT().
                 Double put of the same pCmdDescr without waiting for the first one to complete 
                 (by means of getStatus) lead to ASSERT()
  	  
    Parameters: pEeprom - EEPROM object reference.
                pCmdDescr - EEPROM command descriptor.
    
    Returns: nothing.
  *****************************************************************************/
  void (*putCmd)(struct Eeprom* pEeprom, EepromCmdDescr* pCmdDescr);
  
  /*****************************************************************************
    Description: Tests whether previously issued command has been completed.
  	  
    Parameters: pEeprom - EEPROM object reference.
                pCmdDescr - EEPROM command descriptor that is being waiting
                            to complete.
    
    Returns: true if command has been completed or false, otherwise.
  *****************************************************************************/
  Bool (*getStatus)(struct Eeprom* pEeprom, EepromCmdDescr* pCmdDescr);
  
} EepromFxns; 

#endif // EEPROM_FXNS_H_

// eof eeprom_fxns.h
