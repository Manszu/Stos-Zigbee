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
  eeprom_cntrl_fxns.h

  EEPROM control interface declaration.
  Written by P.Sobol. 
*******************************************************************************/

#ifndef EEPROM_CNTRL_FXNS_H_
#define EEPROM_CNTRL_FXNS_H_

#include "eeprom_fxns.h"

#define EEPROM_DATA_MEMORY_SIZE 0x1000u

typedef enum
{
  EEPROM_STATE_INVALID = 0,
  EEPROM_STATE_IDLE,
  EEPROM_STATE_WRITING,
  EEPROM_STATE_READING
} EepromState;

typedef struct
{
  List          jobQueue;    // Pending jobs queue
  List          doneQueue;   // Completed jobs queue
  
  EepromState   state;       // Eeprom state
  
  Uint16        bytesLeft;   // Active job bytes left to process
  Uint8*        dataPointer; // Active job data pointer
  Uint16        dataAddress; // Active job current EEPROM address
  EepromCmdId   cmdId;       // Active job command 
}Eeprom;

/*****************************************************************************
  Description: Returns EEPROM interface implementation reference.
  	
  Parameters: pEepromFxns - EEPROM interface reference.
  	
  Returns: nothing.
*****************************************************************************/
void eeprom_getFxns(EepromFxns* pEepromFxns);

/*******************************************************************************
  Description: inits the Eeprom object.
  	
  Parameters: pEeprom - EEPROM object reference.
  	
  Returns: nothing.
*******************************************************************************/
void eeprom_init(Eeprom* pEeprom);

#endif // EEPROM_CNTRL_FXNS_H_ 

//eof eeprom_cntrl_fxns.h
