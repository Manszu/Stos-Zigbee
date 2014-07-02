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
  ParamServer.c
  
  Parameters storage module using EEPROM.
********************************************************************************/


#include "sched.h"
#include "avrhardware.h"
#include "hardware.h"
#include "eeprom.h"
#include "ParamServer.h"

#define EEPROM_ADDR_WR_LEN  0x0000
#define EEPROM_ADDR_OFFSET  0x0002
#define EEPROM_LENGTH       PARAM_SERVER_EEPROM_LENGTH

enum
{
  PARAMSERV_IDLE_STATE = 0,
  PARAMSERV_GET_STATE,
  PARAMSERV_SET_STATE,
  PARAMSERV_CHECK_STATE,
  PARAMSERV_UPDATE_STATE,
  PARAMSERV_ERASE_STATE,
} PARAMSERV__state;

enum
{
  PARAMSERV_SUBSTATE_IDLE = 0,
  PARAMSERV_SUBSTATE_1,
  PARAMSERV_SUBSTATE_2,
  PARAMSERV_SUBSTATE_3,
  PARAMSERV_SUBSTATE_4,
  PARAMSERV_SUBSTATE_5,
  PARAMSERV_SUBSTATE_6,
  PARAMSERV_SUBSTATE_7,
  PARAMSERV_SUBSTATE_8,
  PARAMSERV_SUBSTATE_9,
} PARAMSERV__subState;

typedef struct
{
  uint16_t len;
  uint16_t l;
  uint16_t addr;
  uint16_t id;
  ParamServerStatus_t st;
  uint16_t fcs;
  uint16_t crc;
} ParamServerParams_t;

typedef struct
{
  uint16_t addr;
  uint8_t data;
} ParamServerCrc_t;

// Server parameter's length of image loading.
volatile uint16_t paramServer_InitLength_eeprom __attribute__ ((section (".eeprom"))) = 0;

EEPROMParams_t eParams;

//temporary parameters
ParamServerParams_t sParams;
ParamServerCrc_t sCrc;
uint8_t dat[2] = {0x00, 0x00};

//incoming parameters
void (*ParamServer__operationDone)(ParamServerStatus_t status);
uint16_t ParamServer__paramId;
uint16_t ParamServer__length;
void* ParamServer__data; 

void ParamServer__callDispatcher();
result_t update_crc16();
result_t update_crc16Done();
void read_param();
void write_param();
void read_len();
void end_func();
result_t begin_func();

/*******************************************************************************
  Description: ParamServer initialization.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t PServ__ParamServer__init()
{
  if (eeprom__init() != SUCCESS) 
    return FAIL;
  PARAMSERV__state = PARAMSERV_IDLE_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_IDLE;
  return SUCCESS;
}

/*******************************************************************************
  Description: indicate end of ParamServer operation.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void ParamServer__tOperationDone()
{
  ParamServer__operationDone(sParams.st);
}

/*******************************************************************************
  Description: get parameter.

  Parameters: moduleId  - ID of module.
              paramId   - ID of parameter.
              length    - length of parameter.
              data      - pointer to memory area where function
                          writes value of parameter.
              getDone   - pointer to callback function that
                          indicates end of operation.

  Returns: FAIL if ParamServer busy, pointers to data or getDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
result_t PServ__ParamServer__get(uint8_t moduleId,
                                 uint8_t paramId,
                                 uint16_t length,
                                 void* data,
                                 void (*getDone)(ParamServerStatus_t status))
{
  if (!data || !getDone)
    return FAIL;

  if (PARAMSERV__state != PARAMSERV_IDLE_STATE)
    return FAIL;

  PARAMSERV__state = PARAMSERV_GET_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_1;

  ParamServer__operationDone = getDone;
  ParamServer__paramId = paramId + (moduleId << 8);
  ParamServer__length = length;
  ParamServer__data = data;

  read_len();
  sParams.addr += EEPROM_ADDR_OFFSET;
  return SUCCESS;
}

/*******************************************************************************
  Description: set parameter.

  Parameters: moduleId  - ID of module.
              paramId   - ID of parameter.
              length    - length of parameter.
              data      - pointer to memory area from function
                          writes value of parameter.
              setDone   - pointer to callback function that
                          indicates end of operation.

  Returns: FAIL if ParamServer busy, pointers to data or setDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
result_t PServ__ParamServer__set(uint8_t moduleId,
                                 uint8_t paramId,
                                 uint16_t length,
                                 void* data,
                                 void (*setDone)(ParamServerStatus_t status))
{
  if (!data || !setDone) 
    return FAIL;

  if (PARAMSERV__state != PARAMSERV_IDLE_STATE)
    return FAIL;

  PARAMSERV__state = PARAMSERV_SET_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_1;

  ParamServer__operationDone = setDone;
  ParamServer__paramId = paramId + (moduleId << 8);
  ParamServer__length = length;
  ParamServer__data = data;

  //does parameter exist?
  read_len();
  sParams.addr += EEPROM_ADDR_OFFSET;
  return SUCCESS;
}

/*******************************************************************************
  Description: check FCS

  Parameters: checkDone  - pointer to callback function that
                           indicates end of operation.

  Returns: FAIL if ParamServer busy or checkDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
result_t PServ__ParamServer__checkFCS(void (*checkDone)(ParamServerStatus_t status))
{
  if (!checkDone)
    return FAIL;

  if (PARAMSERV__state != PARAMSERV_IDLE_STATE)
    return FAIL;

  PARAMSERV__state = PARAMSERV_CHECK_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_1;

  ParamServer__operationDone = checkDone;

  read_len();
  return SUCCESS;
}

/*******************************************************************************
  Description: update FCS.

  Parameters: updateDone  - pointer to callback function that
                            indicates end of operation.

  Returns: FAIL if ParamServer busy or updateDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
result_t PServ__ParamServer__updateFCS(void (*updateDone)(ParamServerStatus_t status))
{
  if (!updateDone)
    return FAIL;

  if (PARAMSERV__state != PARAMSERV_IDLE_STATE)
    return FAIL;

  PARAMSERV__state = PARAMSERV_UPDATE_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_1;

  ParamServer__operationDone = updateDone;

  read_len();
  return SUCCESS;
}

/*******************************************************************************
  Description: erase parameter.

  Parameters: eraseDone  - pointer to callback function that
                           indicates end of operation.

  Returns: FAIL if ParamServer busy or updateDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
result_t PServ__ParamServer__erase(void (*eraseDone)(ParamServerStatus_t status))
{
  if (!eraseDone)
    return FAIL;

  if (PARAMSERV__state != PARAMSERV_IDLE_STATE)
    return FAIL;

  PARAMSERV__state = PARAMSERV_ERASE_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_1;

  ParamServer__operationDone = eraseDone;

  sParams.addr = EEPROM_ADDR_WR_LEN;
  eParams.data = dat; 
  write_param();

  return SUCCESS;
}

/*******************************************************************************
  Description: state machine dispatcher of ParamServer.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void ParamServer__callDispatcher()
{
  switch (PARAMSERV__state)
  {
    case PARAMSERV_GET_STATE: 
      switch (PARAMSERV__subState)
      {
        case PARAMSERV_SUBSTATE_1:
          if(begin_func() == FAIL)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
            return;
          }
          if (sParams.len == 0)
          {
            sParams.st = PARAMSERV_S_INVALID_LENGTH;
            end_func();
            return;
          }
        case PARAMSERV_SUBSTATE_2:
          if (sParams.addr < sParams.len)
          {
            //get param id
            PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
            eParams.data = (uint8_t *)&sParams.id;
            read_param();
          }
          else
          {
            sParams.st = PARAMSERV_S_UNKNOWN_PARAM;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          break;
        case PARAMSERV_SUBSTATE_3:
          //get param len
          sParams.addr += EEPROM_ADDR_OFFSET;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_4;
          eParams.data = (uint8_t *)&sParams.l;
          read_param();
          break;
        case PARAMSERV_SUBSTATE_4:
          //get param data
          sParams.addr += EEPROM_ADDR_OFFSET;
          if (sParams.id == ParamServer__paramId)
          {
            sParams.st = PARAMSERV_S_SUCCESS;
            if (ParamServer__length > sParams.l)
            {
              ParamServer__length = sParams.l;
              sParams.st = PARAMSERV_S_INVALID_LENGTH;
            }
            //EEPROMParams_t struct
            eParams.addr = sParams.addr;
            eParams.length = ParamServer__length;
            eParams.data = (uint8_t *)ParamServer__data;
            if (eeprom__read(&eParams, ParamServer__callDispatcher) != SUCCESS)
            {
              sParams.st = PARAMSERV_S_EEPROM_FAIL;
              //TOS_post(ParamServer__tOperationDone);
              end_func();
            }
            PARAMSERV__subState = PARAMSERV_SUBSTATE_5;
          }
          else
          {
            sParams.addr += sParams.l;
            PARAMSERV__subState = PARAMSERV_SUBSTATE_2;
            TOS_post(ParamServer__callDispatcher);
          }
          break;
        case PARAMSERV_SUBSTATE_5:
          end_func();
          break;
        default:
          break;
      }
      break;
    case PARAMSERV_SET_STATE:
      switch(PARAMSERV__subState)
      {
        case PARAMSERV_SUBSTATE_1:
          if (begin_func() == FAIL)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
            return;
          }
        case PARAMSERV_SUBSTATE_2:
          if (sParams.addr < sParams.len)
          {
            //get param id
            PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
            eParams.data = (uint8_t *)&sParams.id;
            read_param();
          }
          else
          {
            //new parameter. 
            //is database full?
            sParams.len += EEPROM_ADDR_OFFSET;
            if (sParams.len == EEPROM_LENGTH)
            {
              sParams.st = PARAMSERV_S_DATABASE_FULL;
              //TOS_post(ParamServer__tOperationDone);
              end_func();
              break;
            }
            //new param & database not full
            //param id
            PARAMSERV__subState = PARAMSERV_SUBSTATE_6;
            eParams.data = (uint8_t *)&ParamServer__paramId;
            write_param();
          }
          break;
        case PARAMSERV_SUBSTATE_3:
          //get param len
          sParams.addr += EEPROM_ADDR_OFFSET;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_4;
          eParams.data = (uint8_t *)&sParams.l;
          read_param();
          break;
        case PARAMSERV_SUBSTATE_4:
          //get param data
          sParams.addr += EEPROM_ADDR_OFFSET;
          if (sParams.id == ParamServer__paramId)
          {
            sParams.st = PARAMSERV_S_SUCCESS;
            if (ParamServer__length > sParams.l)
            {
              ParamServer__length = sParams.l;
              sParams.st = PARAMSERV_S_INVALID_LENGTH;
            }
            //EEPROMParams_t struct
            eParams.addr = sParams.addr;
            eParams.length = ParamServer__length;
            eParams.data = (uint8_t *)ParamServer__data;
            if (eeprom__write(&eParams, ParamServer__callDispatcher) != SUCCESS)
            {
              sParams.st = PARAMSERV_S_EEPROM_FAIL;
              //TOS_post(ParamServer__tOperationDone);
              end_func();
            }
            PARAMSERV__subState = PARAMSERV_SUBSTATE_5;
          }
          else
          {
            sParams.addr += sParams.l;
            PARAMSERV__subState = PARAMSERV_SUBSTATE_2;
            TOS_post(ParamServer__callDispatcher);
          }
          break;
        case PARAMSERV_SUBSTATE_5:
          end_func();
          break;
        case PARAMSERV_SUBSTATE_6:
          //param len 
          sParams.addr += EEPROM_ADDR_OFFSET;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_7;
          eParams.data = (uint8_t *)&ParamServer__length;
          write_param();
          break;
        case PARAMSERV_SUBSTATE_7:
          //param data
          sParams.addr += EEPROM_ADDR_OFFSET;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_8;
          //EEPROMParams_t struct
          eParams.addr = sParams.addr;
          eParams.length = ParamServer__length;
          eParams.data = (uint8_t *)ParamServer__data;
          if (eeprom__write(&eParams, ParamServer__callDispatcher) != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          break;
        case PARAMSERV_SUBSTATE_8:
          //write total len
          sParams.addr = sParams.addr + ParamServer__length - EEPROM_ADDR_OFFSET;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_9;
          //EEPROMParams_t struct
          eParams.addr = EEPROM_ADDR_WR_LEN;
          eParams.length = 2;
          eParams.data = (uint8_t *)&sParams.addr;
          if (eeprom__write(&eParams, ParamServer__callDispatcher) != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          break;
        case PARAMSERV_SUBSTATE_9:
          sParams.st = PARAMSERV_S_SUCCESS;
          end_func();
          break;
        default:
          break;
      }
      break;
    case PARAMSERV_CHECK_STATE:
      switch (PARAMSERV__subState)
      {
        case PARAMSERV_SUBSTATE_1:
          if (sParams.len > EEPROM_LENGTH - 2 * EEPROM_ADDR_OFFSET)
          {
            //sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //end_func();
            //return;
            PARAMSERV__subState = PARAMSERV_SUBSTATE_7;
            sParams.addr = EEPROM_ADDR_WR_LEN;
            eParams.data = dat; 
            write_param();
          }
          if (sParams.len == 0)
          {
            sParams.st = PARAMSERV_S_SUCCESS;
            end_func();
            return;
          }
          //database fcs
          sParams.addr = sParams.addr + EEPROM_ADDR_OFFSET + sParams.len;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_2;
          eParams.data = (uint8_t *)&sParams.fcs;
          read_param();
          break;
        case PARAMSERV_SUBSTATE_2:
          //calculation fcs
          PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
          //init values
          sParams.crc = 0;
          sCrc.addr = EEPROM_ADDR_WR_LEN;
        case PARAMSERV_SUBSTATE_3:
          if (update_crc16() != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          break;
        case PARAMSERV_SUBSTATE_4:
          if (update_crc16Done() != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          else
          {
            PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
            TOS_post(ParamServer__callDispatcher);
          }
          break;
        case PARAMSERV_SUBSTATE_6:
          if (sParams.fcs == sParams.crc)
          {
            sParams.st = PARAMSERV_S_SUCCESS;
            end_func();
          }
          else
          {
            PARAMSERV__subState = PARAMSERV_SUBSTATE_7;
            sParams.addr = EEPROM_ADDR_WR_LEN;
            eParams.data = dat; 
            write_param();
          }
          break;
        case PARAMSERV_SUBSTATE_7:
          sParams.st = PARAMSERV_S_INVALID_FCS;
          //TOS_post(ParamServer__tOperationDone);
          end_func();
          break;
        default:
          break;
      }
      break;
    case PARAMSERV_UPDATE_STATE:
      switch (PARAMSERV__subState)
      {
        case PARAMSERV_SUBSTATE_1:
          if (sParams.len > EEPROM_LENGTH - 2 * EEPROM_ADDR_OFFSET)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
            return;
          }
          if (sParams.len == 0)
          {
            sParams.st = PARAMSERV_S_SUCCESS;
            end_func();
            return;
          }
          sParams.addr = sParams.addr + EEPROM_ADDR_OFFSET + sParams.len;
          PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
          //init values
          sParams.crc = 0;
          sCrc.addr = EEPROM_ADDR_WR_LEN;
        case PARAMSERV_SUBSTATE_3:
          //calculation fcs
          if (update_crc16() != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          break;
        case PARAMSERV_SUBSTATE_4:
          if (update_crc16Done() != SUCCESS)
          {
            sParams.st = PARAMSERV_S_EEPROM_FAIL;
            //TOS_post(ParamServer__tOperationDone);
            end_func();
          }
          else
          {
            PARAMSERV__subState = PARAMSERV_SUBSTATE_3;
            TOS_post(ParamServer__callDispatcher);
          }
          break;
        case PARAMSERV_SUBSTATE_6:
          PARAMSERV__subState = PARAMSERV_SUBSTATE_7;
          eParams.data = (uint8_t *)&sParams.crc;
          write_param();
          break;
        case PARAMSERV_SUBSTATE_7:
          sParams.st = PARAMSERV_S_SUCCESS;
          end_func();
          break;
        default: 
          break;
      }
      break;
    case PARAMSERV_ERASE_STATE:
      switch (PARAMSERV__subState)
      {
        case PARAMSERV_SUBSTATE_1:
          sParams.st = PARAMSERV_S_SUCCESS;
          end_func();
          break;
        default:
          break;
      }
      break;
    default:
      break;
  }
}

/*******************************************************************************
  Description: write parameter to EEPROM.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void write_param()
{
  eParams.addr=sParams.addr;
  eParams.length = 2;
  if(eeprom__write(&eParams, ParamServer__callDispatcher) != SUCCESS)
  {
    sParams.st = PARAMSERV_S_EEPROM_FAIL;
    //TOS_post(ParamServer__tOperationDone);
    end_func();
  }
}

/*******************************************************************************
  Description: read parameter from EEPROM.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void read_param()
{
  //EEPROMParams_t struct
  eParams.addr = sParams.addr;
  eParams.length = 2;
  if (eeprom__read(&eParams, ParamServer__callDispatcher) != SUCCESS)
  {
    sParams.st = PARAMSERV_S_EEPROM_FAIL;
    //TOS_post(ParamServer__tOperationDone);
    end_func();
  }
}

/*******************************************************************************
  Description: read length of parameter.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void read_len()
{
  sParams.addr = EEPROM_ADDR_WR_LEN;
  eParams.data = (uint8_t *)&sParams.len;
  read_param();
}

/*******************************************************************************
  Description: reset state machine of ParamsServer at the end of operation.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void end_func()
{
  TOS_post(ParamServer__tOperationDone);
  PARAMSERV__state = PARAMSERV_IDLE_STATE;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_IDLE;
}

/*******************************************************************************
  Description: check address range of parameter.

  Parameters: nothing.

  Returns: FAIL if address is out of range, SUCCESS otherwise.
*******************************************************************************/
result_t begin_func()
{
  sParams.len += EEPROM_ADDR_OFFSET;
  if (sParams.len > EEPROM_LENGTH - EEPROM_ADDR_OFFSET)
    return FAIL;
  PARAMSERV__subState = PARAMSERV_SUBSTATE_2;
  return SUCCESS;
}

/*******************************************************************************
  Description: update CRC16.

  Parameters: nothing.

  Returns: FAIL if there is EEPROM reading problem, SUCCESS otherwise.
*******************************************************************************/
result_t update_crc16()//uint16_t num, uint16_t *crc
{
  if(sCrc.addr < sParams.addr)
  {
    PARAMSERV__subState = PARAMSERV_SUBSTATE_4;
    //EEPROMParams_t struct
    eParams.addr = sCrc.addr;
    eParams.length = 1;
    eParams.data = (uint8_t *)&sCrc.data;
    if (eeprom__read(&eParams, ParamServer__callDispatcher) != SUCCESS)
      return FAIL;
  }
  else
  {
    PARAMSERV__subState = PARAMSERV_SUBSTATE_6;
    TOS_post(ParamServer__callDispatcher);
  }
  return SUCCESS;
}

/*******************************************************************************
  Description: check end of CRC16 update operation done.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t update_crc16Done()
{
  uint8_t i = 0;

  do
  {
    sParams.crc >>= 1;
    if ((uint8_t)(((uint8_t)sParams.crc ^ sCrc.data) & 1))
      sParams.crc ^= 0xA001;
    sCrc.data >>= 1;
  } while (!(++i & 0x8));
  sCrc.addr++;
  return SUCCESS;
}

// eof ParamServer.c
