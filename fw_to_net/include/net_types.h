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
  net_types.h
  
  Net types declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef NET_TYPES_H_ 
#define NET_TYPES_H_

#include "com_types.h"

typedef enum // Reset reason.
{
  NODE_RESET_REASON_INVALID = 0,  // Invalid reset reason
  NODE_RESET_REASON_COLD,          // Cold reset has been performed.
  NODE_RESET_REASON_WARM,          // Cold reset occured.
  NODE_RESET_REASON_WD,            // Watchdog timer caused the reset.
} NodeResetReason;

enum
{
  EEPROM_USER_START = 0x0010,

  NET_BROADCAST_PANID = 0xFFFF,
  NET_BROADCAST_ADDR = 0xFFFF,

  NET_CHANNEL_UNAVAILABLE = 0xFF,
  NET_LQI_UNAVAILABLE =  0,
  NET_RSSI_UNAVAILABLE = -128,
};

typedef Uint16 NetSyncPeriod;
typedef Int8  NetTxPower;
typedef Uint16 NetDeliveryTimeout;
typedef Uint16 NetAutoNetwork;
typedef Uint16 NetPanId;
typedef Uint8  NetChannel;
typedef Uint32 NetChannelMask;

typedef struct // Power duration parameter.
{
  Uint16 active; // Active interval for end-device.
  Uint16 sleep;  // Sleep interval for end-device.
} NetPowerDuration;

typedef enum // Parameter IDs.
{
  NET_PARAM_ID_NODE_INVALID = 0,
  NET_PARAM_ID_NODE_ROLE,
  NET_PARAM_ID_CAPABILITY,
  NET_PARAM_ID_LOGICAL_ADDR,
  NET_PARAM_ID_NWK_ADDR,
  NET_PARAM_ID_MAC_ADDR,
  NET_PARAM_ID_PANID,
  NET_PARAM_ID_ACTUAL_PANID,
  NET_PARAM_ID_CURRENT_CHANNEL,
  NET_PARAM_ID_CHANNEL_MASK,
  NET_PARAM_ID_LQI,
  NET_PARAM_ID_TX_POWER,
  NET_PARAM_ID_PARENT_ADDR,
  NET_PARAM_ID_CHILDREN_ADDR,
  NET_PARAM_ID_AUTO_NETWORK,
  NET_PARAM_ID_POWER_DURATION,
  NET_PARAM_ID_UART_DTR,
  NET_PARAM_ID_CALIBR_PERIOD,
  NET_PARAM_ID_MANUFACTURER_ID,
  NET_PARAM_ID_MODEL_ID,
  NET_PARAM_ID_HARDWARE_SOFTWARE_ID,
  NET_PARAM_ID_DATA_RETRY,
  NET_PARAM_ID_DATA_TIMEOUT,
  NET_PARAM_ID_SYNC_PERIOD,
  NET_PARAM_ID_SECURITY_KEY,

  NET_PARAM_ID_FIRST_UNUSED
} NetParamId;

typedef enum
{
  NET_STATUS_INVALID = 0,
  NET_STATUS_SUCCESS,
  NET_STATUS_INVALID_PARAM,
  NET_STATUS_NO_ACK,
  NET_STATUS_INVALID_ENDPOINT, 

} NetStatus;

struct Net;

typedef Uint8  NetDataHandle;
typedef Uint16 NetNwkAddr; 
typedef Uint16 NetLogicalAddr; 
typedef Uint64 NetExtAddr;
typedef Uint8 NetEndPoint;

/*******************************************************************************/
typedef enum
{
  NET_ADDR_MODE_NWK,
  NET_ADDR_MODE_LOGICAL,
  NET_ADDR_MODE_EXT

} NetAddrMode;

/*******************************************************************************/
typedef struct
{
  union
  {
    NetNwkAddr     nwk;
    NetLogicalAddr logical;
    NetExtAddr     ext;
  } addr;
  NetAddrMode addrMode;
  
} NetAddr;

/*******************************************************************************/
typedef struct
{
  NetNwkAddr     nwk;
  NetLogicalAddr logical;
  NetExtAddr    ext;
  struct 
  {
    Uint8   nwkValid:       1;
    Uint8   logicalValid:   1;
    Uint8   extValid:       1;
  } flags;

} NetAllAddr;

/*******************************************************************************/
#endif // NET_TYPES_H_

//eof net_types.h
