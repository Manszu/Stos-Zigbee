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
  param_srv_fxns.h
  
  Parameter server for persistent parameters. 
  Supports fixed set of parameters.
  Written by P.Sobol.
*******************************************************************************/

#ifndef PARAM_SRV_FXNS_H_
#define PARAM_SRV_FXNS_H_

#include "com_types.h"
#include "eeprom_fxns.h"
#include "net_types.h"

struct ParamSrv;
struct ParamSrvSetDescr;

typedef enum
{
  PARAM_SRV_STATUS_SUCCESS,
  PARAM_SRV_STATUS_DEFAULT_VALUE,
  PARAM_SRV_STATUS_INVALID_ID,
  PARAM_SRV_STATUS_VALUE_OUT_OF_RANGE,
  PARAM_SRV_STATUS_EEPROM_FAIL
} ParamSrvStatus;

typedef enum
{
  PARAM_SRV_ID_INVALID = 0x0000,

  // PHY.
  PARAM_SRV_ID_PHY_TX_POWER = NET_PARAM_ID_TX_POWER,

  // MAC.

  PARAM_SRV_ID_MAC_ADDR = NET_PARAM_ID_MAC_ADDR,
  // NWK.

  // APS.
  PARAM_SRV_ID_APS_MAX_FRAME_RETRIES = NET_PARAM_ID_DATA_RETRY,
  PARAM_SRV_ID_APS_ACK_WAIT_DURATION = NET_PARAM_ID_DATA_TIMEOUT,

  // ZDO.

  // NET.
  PARAM_SRV_ID_NET_NODE_ROLE    = NET_PARAM_ID_NODE_ROLE,
  PARAM_SRV_ID_NET_LOGICAL_ADDR = NET_PARAM_ID_LOGICAL_ADDR,
  PARAM_SRV_ID_NET_PANID        = NET_PARAM_ID_PANID,
  PARAM_SRV_ID_NET_CHANNEL_MASK = NET_PARAM_ID_CHANNEL_MASK,
  PARAM_SRV_ID_NET_AUTO_NETWORK = NET_PARAM_ID_AUTO_NETWORK,
  PARAM_SRV_ID_NET_POWER_DURATION = NET_PARAM_ID_POWER_DURATION,
  PARAM_SRV_ID_NET_UART_DTR = NET_PARAM_ID_UART_DTR,
  PARAM_SRV_ID_NET_CALIBR_PERIOD = NET_PARAM_ID_CALIBR_PERIOD,
  PARAM_SRV_ID_NET_SYNC_PERIOD = NET_PARAM_ID_SYNC_PERIOD,
} ParamSrvId;

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
********************************************************************************/
ParamSrvStatus paramSrv_getValue(struct ParamSrv* pParamSrv, ParamSrvId id, Uint8* pGetValue);

/*****************************************************************************
  Description: set parameter (save to EEPROM).

  Parameters: pSrv - reference to the ParamSrv object,
              pDesc - reference to the parameter set command descriptor which
                     allocated by user and must be valid until confirm is got.
                     Unknows parameter ID in descriptor leads to ASSERT().

  Returns: none.

  Note: ParamSrv saves parameters to EEPROM, setValue() is splitted.
        The operation is completed only when getSetConf() returns a valid
        descriptor.
*****************************************************************************/
void paramSrv_setValue(struct ParamSrv* pSrv, struct ParamSrvSetDescr* pDesc);

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
Bool paramSrv_getSetStatus(struct ParamSrv* pParamSrv, struct ParamSrvSetDescr* pDescr);

#endif // PARAM_SRV_FXNS_H_

// eof param_srv_fxns.h

