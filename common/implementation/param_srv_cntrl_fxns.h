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
  param_srv_ctrl_fxns.h
  
  Parameter server control interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef PARAM_SRV_CTRL_FXNS_H_
#define PARAM_SRV_CTRL_FXNS_H_

#include "param_srv_fxns.h"
#include "eeprom_fxns.h"
#include "net_types.h"

enum
{
  // PHY.
  PARAM_SRV_ADDR_PHY_TX_POWER = 0x0000,

  // MAC.
  PARAM_SRV_ADDR_MAC_ADDR = PARAM_SRV_ADDR_PHY_TX_POWER + 2 * sizeof(NetTxPower),

  // APS.
  PARAM_SRV_ADDR_APS_MAX_FRAME_RETRIES = PARAM_SRV_ADDR_MAC_ADDR + 2 * sizeof(NetExtAddr),
  PARAM_SRV_ADDR_APS_ACK_WAIT_DURATION = PARAM_SRV_ADDR_APS_MAX_FRAME_RETRIES + 2 * sizeof(Uint8),

  // Net.
  PARAM_SRV_ADDR_NET_NODE_ROLE = PARAM_SRV_ADDR_APS_ACK_WAIT_DURATION + 2 * sizeof(Uint16),
  PARAM_SRV_ADDR_NET_LOGICAL_ADDR = PARAM_SRV_ADDR_NET_NODE_ROLE + 2 * sizeof(Uint8),
  PARAM_SRV_ADDR_NET_PANID = PARAM_SRV_ADDR_NET_LOGICAL_ADDR + 2 * sizeof(NetLogicalAddr),
  PARAM_SRV_ADDR_NET_CHANNEL_MASK = PARAM_SRV_ADDR_NET_PANID + 2 * sizeof(NetPanId),

  PARAM_SRV_ADDR_NET_AUTO_NETWORK = PARAM_SRV_ADDR_NET_CHANNEL_MASK + 2 * sizeof(NetChannelMask),
  PARAM_SRV_ADDR_NET_POWER_DURATION = PARAM_SRV_ADDR_NET_AUTO_NETWORK + 2 * sizeof(NetAutoNetwork),
  PARAM_SRV_ADDR_NET_UART_DTR = PARAM_SRV_ADDR_NET_POWER_DURATION + 2 * sizeof(NetPowerDuration),
  PARAM_SRV_ADDR_NET_CALIBR_PERIOD = PARAM_SRV_ADDR_NET_UART_DTR + 2 * sizeof(Uint8),
  PARAM_SRV_ADDR_NET_SYNC_PERIOD = PARAM_SRV_ADDR_NET_CALIBR_PERIOD + 2 * sizeof(Uint16),

  PARAM_SRV_EEPROM_SIZE = 0x0100,
  PARAM_SRV_MAX_PARAM = 15,
};

typedef enum
{
  PARAM_SRV_PARAM_TYPE_INVALID = 0,
  PARAM_SRV_PARAM_TYPE_UINT8,
  PARAM_SRV_PARAM_TYPE_INT8,
  PARAM_SRV_PARAM_TYPE_UINT16,
  PARAM_SRV_PARAM_TYPE_UINT32,
  PARAM_SRV_PARAM_TYPE_UINT64,
} ParamSrvParamType;

typedef union
{
  Uint64 uint64;
  Uint8  uint8;
  Int8    int8;
  Uint16 uint16;
  Uint16 uint32;
} ParamValue;

typedef struct
{
  ParamSrvId id;
  ParamSrvParamType paramType;
  Uint8 length;
  Uint16 addr;
  ParamValue min;
  ParamValue max;
  ParamValue defaultValue;
} ParamValueDescr;

typedef struct
{
  const ParamValueDescr* pParamValueDescr;
  void* pParamValue;
} ParamDescrRecord;

typedef enum
{
  PARAM_SRV_STATE_INVALID = 0,
  PARAM_SRV_STATE_INIT,
  PARAM_SRV_STATE_WORK,
} ParamSrvState;

typedef enum
{
  PARAM_SRV_INIT_STATE_INVALID = 0,
  PARAM_SRV_INIT_STATE_IDLE,
  PARAM_SRV_INIT_STATE_WAIT_FOR_EEPROM,
} ParamSrvInitState;

struct ParamSrvSetDescr       // Parameter set request descriptor
{
  ListItem        listItem; // Standard header for adding to a list.
  ParamSrvId      id;       // - Parameter ID. Supported a fixed set of
                            //   parameters defined in header. Parameter length
                            //   determined by ID.
  Uint8*          pValue;   // Reference to the value to set

  EepromCmdDescr eepromCmdDescr;
  Bool waitForEeprom;

  ParamSrvStatus  status;   // status of the confirm
};

struct ParamSrv
{
  EepromFxns eepromFxns;
  
  List setPendList;
  
  Int8            txPower;
  Uint8 apsMaxFrameRetries;
  Uint16 apsAckWaitDuration;
  Uint8 netNodeRole;
  Uint16 netLogicalAddr;
  Uint16 netPanId;
  Uint32 netChannelMask;

  NetExtAddr macAddr;
  NetAutoNetwork netAutoNetwork;
  NetPowerDuration netPowerDuration;
  Uint8 netUartDtr;
  Uint16 netCalibrPeriod;
  Uint16 netSyncPeriod;

  ParamDescrRecord paramBaseDescr[PARAM_SRV_MAX_PARAM + 1];
  
  ParamSrvState state;
  
  ParamSrvInitState initState;
  Uint8 initParamInd;
  EepromCmdDescr initEepromCmdDescr;
};

/*******************************************************************************
  Description: inits the parameter server.
  	
  Parameters: pParamSrv - parameter server reference.
              pEepromFxns - EEPROM interface reference.
  	
  Returns: nothing.
*******************************************************************************/
void paramSrv_init(struct ParamSrv* pParamSrv, EepromFxns* pEepromFxns);

/*******************************************************************************
  Description: determines whether init phase has been completed.
  	
  Parameters: pParamSrv - parameter server reference.
  	
  Returns: true if init phase has been completed and false, otherwise.
*******************************************************************************/
Bool paramSrv_isInitCompleted(struct ParamSrv* pParamSrv);

#endif // PARAM_SRV_CTRL_FXNS_H_

//eof param_srv_ctrl_fxns.h
