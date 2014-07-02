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
  param_srv.c
  
  Parameter server implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "param_srv_fxns.h"
#include "param_srv_cntrl_fxns.h"
#include "com_assert.h"
#include <string.h>

#ifdef AVR_PROGMEM_USE

#include <avr/pgmspace.h>

#else

#define PROGMEM
#define memcpy_P memcpy

#endif // AVR_PROG_MEM_USE

const ParamValueDescr phyTxPowerDescr PROGMEM = {PARAM_SRV_ID_PHY_TX_POWER,
                                                 PARAM_SRV_PARAM_TYPE_INT8,
                                                 sizeof(Int8),
                                                 PARAM_SRV_ADDR_PHY_TX_POWER,
                                                 {-17},
                                                 {3},
                                                 {0}};

const ParamValueDescr apsMaxFrameRetriesDescr PROGMEM = {PARAM_SRV_ID_APS_MAX_FRAME_RETRIES,
                                                         PARAM_SRV_PARAM_TYPE_UINT8,
                                                         sizeof(Uint8),
                                                         PARAM_SRV_ADDR_APS_MAX_FRAME_RETRIES,
                                                         {0},
                                                         {5},
                                                         {3}};

const ParamValueDescr apsAckWaitDurationDescr PROGMEM = {PARAM_SRV_ID_APS_ACK_WAIT_DURATION,
                                                         PARAM_SRV_PARAM_TYPE_UINT16,
                                                         sizeof(Uint16),
                                                         PARAM_SRV_ADDR_APS_ACK_WAIT_DURATION,
                                                         {10},
                                                         {3000},
                                                         {1000}};

const ParamValueDescr netNodeRoleDescr PROGMEM = {PARAM_SRV_ID_NET_NODE_ROLE,
                                                  PARAM_SRV_PARAM_TYPE_UINT8,
                                                  sizeof(Uint8),
                                                  PARAM_SRV_ADDR_NET_NODE_ROLE,
                                                  {0},
                                                  {2},
                                                  {1}};
                                                    
const ParamValueDescr netLogicalAddrDescr PROGMEM = {PARAM_SRV_ID_NET_LOGICAL_ADDR,
                                                     PARAM_SRV_PARAM_TYPE_UINT16,
                                                     sizeof(Uint16),
                                                     PARAM_SRV_ADDR_NET_LOGICAL_ADDR,
                                                     {0},
                                                     {0xFFFF},
                                                     {0}};

const ParamValueDescr netPanIdDescr PROGMEM = {PARAM_SRV_ID_NET_PANID,
                                               PARAM_SRV_PARAM_TYPE_UINT16,
                                               sizeof(Uint16),
                                               PARAM_SRV_ADDR_NET_PANID,
                                               {0},
                                               {0xFFFF},
                                               {0xFFFF}};

const ParamValueDescr netChannelMaskDescr PROGMEM = {PARAM_SRV_ID_NET_CHANNEL_MASK,
                                                     PARAM_SRV_PARAM_TYPE_UINT32,
                                                     sizeof(Uint32),
                                                     PARAM_SRV_ADDR_NET_CHANNEL_MASK,
                                                     {0x800L},
                                                     {0x07FFF800L},
                                                     {0x800L}};

const ParamValueDescr macAddrDescr PROGMEM = {PARAM_SRV_ID_MAC_ADDR,
                                              PARAM_SRV_PARAM_TYPE_UINT64,
                                              sizeof(Uint64),
                                              PARAM_SRV_ADDR_MAC_ADDR,
                                              {0},
                                              {0xFFFFFFFFFFFFFFFFLL},
                                              {0}};

const ParamValueDescr netAutoNetworkDescr PROGMEM = {PARAM_SRV_ID_NET_AUTO_NETWORK,
                                                     PARAM_SRV_PARAM_TYPE_UINT16,
                                                     sizeof(Uint16),
                                                     PARAM_SRV_ADDR_NET_AUTO_NETWORK,
                                                     {0},
                                                     {0xFFFF},
                                                     {0}};

const ParamValueDescr netPowerDurationDescr PROGMEM = {PARAM_SRV_ID_NET_POWER_DURATION,
                                                       PARAM_SRV_PARAM_TYPE_UINT32,
                                                       sizeof(Uint32),
                                                       PARAM_SRV_ADDR_NET_POWER_DURATION,
                                                       {0},
                                                       {0xFFFFFFFF},
                                                       {0}};

const ParamValueDescr netUartDtrDescr PROGMEM = {PARAM_SRV_ID_NET_UART_DTR,
                                                 PARAM_SRV_PARAM_TYPE_UINT8,
                                                 sizeof(Uint8),
                                                 PARAM_SRV_ADDR_NET_POWER_DURATION,
                                                 {0},
                                                 {1},
                                                 {0}};

const ParamValueDescr netCalibrPeriodDescr PROGMEM = {PARAM_SRV_ID_NET_CALIBR_PERIOD,
                                                      PARAM_SRV_PARAM_TYPE_UINT16,
                                                      sizeof(Uint16),
                                                      PARAM_SRV_ADDR_NET_CALIBR_PERIOD,
                                                      {0},
                                                      {0xFFFF},
                                                      {0}};

const ParamValueDescr netSyncPeriodDescr PROGMEM = {PARAM_SRV_ID_NET_SYNC_PERIOD,
                                                    PARAM_SRV_PARAM_TYPE_UINT16,
                                                    sizeof(Uint16),
                                                    PARAM_SRV_ADDR_NET_SYNC_PERIOD,
                                                    {10},
                                                    {30000UL},
                                                    {10}};

// Checks for parameter for being in range.
static Bool isParamInRange(ParamValue* pParamValue, ParamValueDescr* pParamValueDescr);

/*******************************************************************************
  Description: inits the parameter server.

  Parameters: pParamSrv - parameter server reference.
              pEepromFxns - EEPROM interface reference.

  Returns: nothing.
*******************************************************************************/
void paramSrv_init(struct ParamSrv* pParamSrv, EepromFxns* pEepromFxns)
{
  Uint8 paramInd = 0;

  ASSERT(pParamSrv && pEepromFxns);

  // Save reference to EEPROM interface.
  pParamSrv->eepromFxns = *pEepromFxns;

  // Init parameters descriptor.
  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &phyTxPowerDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->txPower;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &apsMaxFrameRetriesDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->apsMaxFrameRetries;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &apsAckWaitDurationDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->apsAckWaitDuration;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netNodeRoleDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netNodeRole;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netLogicalAddrDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netLogicalAddr;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netPanIdDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netPanId;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netChannelMaskDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netChannelMask;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &macAddrDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->macAddr;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netAutoNetworkDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netAutoNetwork;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netPowerDurationDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netPowerDuration;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netUartDtrDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netUartDtr;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netCalibrPeriodDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netCalibrPeriod;
  paramInd++;

  ASSERT(paramInd < PARAM_SRV_MAX_PARAM);
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = &netSyncPeriodDescr;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = &pParamSrv->netSyncPeriod;
  paramInd++;

  // End of table.
  pParamSrv->paramBaseDescr[paramInd].pParamValueDescr = 0;
  pParamSrv->paramBaseDescr[paramInd].pParamValue = 0;

  // Clear list for pending set operations.
  list_init(&pParamSrv->setPendList);

  // Prepare for reading all parameters to cache.
  pParamSrv->state = PARAM_SRV_STATE_INIT;
  pParamSrv->initParamInd = 0;
  pParamSrv->initState = PARAM_SRV_INIT_STATE_IDLE;
}

/*******************************************************************************
  Description: determines whether init phase has been completed. It requires
               continious invocation until completion.

  Parameters: pParamSrv - parameter server reference.

  Returns: true if init phase has been completed and false, otherwise.
*******************************************************************************/
Bool paramSrv_isInitCompleted(struct ParamSrv* pParamSrv)
{
  Bool isCompleted = false;

  ASSERT(pParamSrv);
  ASSERT(PARAM_SRV_STATE_INIT == pParamSrv->state);

  if (PARAM_SRV_INIT_STATE_IDLE == pParamSrv->initState)
  {
    // Pass to next parameter.
    if (pParamSrv->paramBaseDescr[pParamSrv->initParamInd].pParamValueDescr)
    {
      // Start reading next parameter.
      ParamValueDescr paramValueDescr;
      ParamValue* pParamValue = pParamSrv->paramBaseDescr[pParamSrv->initParamInd].pParamValue;

      memcpy_P(&paramValueDescr, pParamSrv->paramBaseDescr[pParamSrv->initParamInd].pParamValueDescr, sizeof(ParamValueDescr));

      // Read from EEPROM.
      pParamSrv->initEepromCmdDescr.id = EEPROM_CMD_ID_READ;
      pParamSrv->initEepromCmdDescr.startAddr = paramValueDescr.addr;
      pParamSrv->initEepromCmdDescr.data = (Uint8*)pParamValue;
      pParamSrv->initEepromCmdDescr.length = paramValueDescr.length;
      pParamSrv->eepromFxns.putCmd(pParamSrv->eepromFxns.pEeprom,
                                   &pParamSrv->initEepromCmdDescr);
      pParamSrv->initState = PARAM_SRV_INIT_STATE_WAIT_FOR_EEPROM;
    }
    else
    {
      // It is the end of all parameters reading.
      pParamSrv->state = PARAM_SRV_STATE_WORK;
      isCompleted = true;
    }
  }
  else
  {
    if (PARAM_SRV_INIT_STATE_WAIT_FOR_EEPROM == pParamSrv->initState)
    {
      // Wait for previous EEPROM request completion.
      Bool isEepromCompleted;

      isEepromCompleted = pParamSrv->eepromFxns.getStatus(pParamSrv->eepromFxns.pEeprom,
                                                          &pParamSrv->initEepromCmdDescr);
      if (isEepromCompleted)
      {
        {
          // Check for being in range.
          ParamValueDescr paramValueDescr;
          ParamValue* pParamValue = pParamSrv->paramBaseDescr[pParamSrv->initParamInd].pParamValue;

          memcpy_P(&paramValueDescr, pParamSrv->paramBaseDescr[pParamSrv->initParamInd].pParamValueDescr, sizeof(ParamValueDescr));

          if (!isParamInRange(pParamValue, &paramValueDescr))
            memcpy(pParamValue, &paramValueDescr.defaultValue, paramValueDescr.length);
        }

        // Pass to the next parameter.
        pParamSrv->initParamInd++;
        pParamSrv->initState = PARAM_SRV_INIT_STATE_IDLE;
      }
    }
    else
      ASSERT(false);
  }
  return isCompleted;
}

/*******************************************************************************
  Description: gets parameter value immediately.

  Parameters: pParamSrv - reference to the ParamSrv object,
              id        - parameter ID. Supported a fixed set of parameters
                          defined in header.
              pGetValue - reference to the buffer where the value should be put.

  Returns: PARAM_SRV_STATUS_SUCCESS - parameter is available,
           PARAM_SRV_STATUS_DEFAULT_VALUE - parameter is available, but set
                                            to default value by ParamSrv
                                   (EEPROM is cleaned or parameter corrupted).
                                   Parameter has default status until it is
                                   set explicitly to a valid non-default value.
*******************************************************************************/
ParamSrvStatus paramSrv_getValue(struct ParamSrv* pParamSrv, ParamSrvId id, Uint8* pGetValue)
{
  ParamSrvStatus status = PARAM_SRV_STATUS_INVALID_ID;
  Uint8 paramInd = 0;

  ASSERT(pParamSrv && pGetValue);
  ASSERT(PARAM_SRV_STATE_WORK == pParamSrv->state);
  
  while (pParamSrv->paramBaseDescr[paramInd].pParamValueDescr)
  {
    ParamValueDescr paramValueDescr;
    ParamValue* pParamValue = pParamSrv->paramBaseDescr[paramInd].pParamValue;

    memcpy_P(&paramValueDescr, pParamSrv->paramBaseDescr[paramInd].pParamValueDescr, sizeof(ParamValueDescr));
    if (paramValueDescr.id == id)
    {
      memcpy(pGetValue, pParamValue, paramValueDescr.length);
      if (memcmp(pParamValue, &paramValueDescr.defaultValue, paramValueDescr.length))
        status = PARAM_SRV_STATUS_SUCCESS;
      else
        status = PARAM_SRV_STATUS_DEFAULT_VALUE;
      break;
    }
    paramInd++;
  };
  return status;
}

/*******************************************************************************
  Description: sets parameter (saves it to EEPROM). The operation is splitted.
               It is completed only when getSetStatus() returns a true.

  Parameters: pParamSrv - reference to the ParamSrv object,
              pDesc     - reference to the parameter set command descriptor which
                          allocated by user and must be valid until confirm is got.

  Returns: nothing.
*******************************************************************************/
void paramSrv_setValue(struct ParamSrv* pParamSrv, struct ParamSrvSetDescr* pSetDescr)
{
  Uint8 paramInd = 0;
  pSetDescr->waitForEeprom = false;

  ASSERT(pParamSrv && pSetDescr);
  ASSERT(PARAM_SRV_STATE_WORK == pParamSrv->state);

  while (1)
  {
    if (pParamSrv->paramBaseDescr[paramInd].pParamValueDescr)
    {
      ParamValueDescr paramValueDescr;
      ParamValue* pParamValue = pParamSrv->paramBaseDescr[paramInd].pParamValue;

      memcpy_P(&paramValueDescr, pParamSrv->paramBaseDescr[paramInd].pParamValueDescr, sizeof(ParamValueDescr));
      if (paramValueDescr.id == pSetDescr->id)
      {
        // Parameter description has been found.
        // Check for valid range.

        if (isParamInRange((ParamValue*)pSetDescr->pValue, &paramValueDescr))
        {
          pSetDescr->status = PARAM_SRV_STATUS_SUCCESS;
          pSetDescr->waitForEeprom = true;
          // Save cached value.
          memcpy(pParamValue, pSetDescr->pValue, paramValueDescr.length);
          // Save to EEPROM.

          pSetDescr->eepromCmdDescr.id = EEPROM_CMD_ID_WRITE;
          pSetDescr->eepromCmdDescr.startAddr = paramValueDescr.addr;
          pSetDescr->eepromCmdDescr.data = (Uint8*)pParamValue;
          pSetDescr->eepromCmdDescr.length = paramValueDescr.length;
          pParamSrv->eepromFxns.putCmd(pParamSrv->eepromFxns.pEeprom,
                                       &pSetDescr->eepromCmdDescr);
        }
        else
          pSetDescr->status = PARAM_SRV_STATUS_VALUE_OUT_OF_RANGE;
        break;
      }
      paramInd++;
    }
    else
    {
      pSetDescr->status = PARAM_SRV_STATUS_INVALID_ID;
      break;
    }
  };
  list_queueTail(&pParamSrv->setPendList, (ListItem*)pSetDescr);
}

/*****************************************************************************
  Description: gets status set parameter operation.

  Parameters: pParamSrv - reference to the ParamSrv object,
              pDesc - reference to the "set parameter" command descriptor.

  Returns: false - operation not complete,
           true  - operation complete, see status in the descriptor:

                   PARAM_SRV_STATUS_SUCCESS - set operation is successfully completed,
                   PARAM_SRV_STATUS_OUT_OF_RANGE - parameter is out of range,
                                                   set operation is not started.
                   PARAM_SRV_STATUS_EEPROM_FAIL - hardware EEPORM failure,
                                                  set operation is not completed.
*****************************************************************************/
Bool paramSrv_getSetStatus(struct ParamSrv* pParamSrv, struct ParamSrvSetDescr* pDescr)
{
  Bool isCompleted = false;
  struct ParamSrvSetDescr* pPendDescr;

  ASSERT(pParamSrv && pDescr);
  ASSERT(PARAM_SRV_STATE_WORK == pParamSrv->state);

  pPendDescr = (struct ParamSrvSetDescr*)list_peekHead(&pParamSrv->setPendList);
  while(pPendDescr)
  {
    if (pPendDescr == pDescr)
    {
      if (pPendDescr->waitForEeprom)
      {
        isCompleted = pParamSrv->eepromFxns.getStatus(pParamSrv->eepromFxns.pEeprom,
                                                      &pPendDescr->eepromCmdDescr);
        if (isCompleted)
        {
          if (EEPROM_STATUS_SUCCESS != pPendDescr->eepromCmdDescr.status)
            pPendDescr->status = PARAM_SRV_STATUS_EEPROM_FAIL;
        }
      }
      if (isCompleted)
      {
        list_unlink(&pParamSrv->setPendList, (ListItem*)pPendDescr);
        break;
      }
    }
    pPendDescr = (struct ParamSrvSetDescr*)pPendDescr->listItem.next;
  };

  return isCompleted;
}

/*****************************************************************************
  Description: checks for parameter for being in range.

  Parameters: pParamValue - reference to parameter to check.
              pParamValueDescr - reference to descriptor of parameter to check.
                
  Returns: true if parameter is in range and false, otherwise.
*****************************************************************************/
static Bool isParamInRange(ParamValue* pValue, ParamValueDescr* pDescr)
{
  Bool isInRange = false;
  
  switch (pDescr->paramType)
  {
    case PARAM_SRV_PARAM_TYPE_UINT8:
         isInRange = ((pValue->uint8 >= pDescr->min.uint8) && (pValue->uint8 <= pDescr->max.uint8)) ? true : false;
         break;
    
    case PARAM_SRV_PARAM_TYPE_INT8:
         isInRange = (pValue->int8 >= pDescr->min.int8) && (pValue->int8 <= pDescr->max.int8);
         break;
    
    case PARAM_SRV_PARAM_TYPE_UINT16:
         isInRange = ((pValue->uint16 >= pDescr->min.uint16) && (pValue->uint16 <= pDescr->max.uint16)) ? true : false;
         break;
    
    case PARAM_SRV_PARAM_TYPE_UINT32:
         isInRange = ((pValue->uint32 >= pDescr->min.uint32) && (pValue->uint32 <= pDescr->max.uint32)) ? true : false;
         break;
    
    case PARAM_SRV_PARAM_TYPE_UINT64:
         isInRange = ((pValue->uint64 >= pDescr->min.uint64) && (pValue->uint64 <= pDescr->max.uint64)) ? true : false;
         break;
    
    default:
         ASSERT(false);
  }
  return isInRange;
}

//eof param_srv.c

