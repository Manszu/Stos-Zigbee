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
  net_mng_fxns.h
  
  Net management interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef NET_MNG_FXNS_H_
#define NET_MNG_FXNS_H_

#include "net_types.h"
#include "eeprom_fxns.h"

struct Net;

/*******************************************************************************
 Data types
*******************************************************************************/

/*******************************************************************************
 Net parameters
*******************************************************************************/

/*******************************************************************************/
typedef enum
{
  NET_NODE_ROLE_COORDINATOR = 0,
  NET_NODE_ROLE_ROUTER      = 1,
  NET_NODE_ROLE_END_DEVICE  = 2,
  NET_NODE_ROLE_UNKNOWN     = 10,

} NetNodeRole;

/******************************************************************************/
typedef struct // Possible node's roles.
{
  Bool coordinator; // Node can be coordinator.
  Bool router;      // Node can be router.
  Bool enddevice;   // Node can be end-device.

} NetNodeCapability;

/******************************************************************************/
typedef struct // LQI parameter.
{
  NetAddr src;
  Uint8 lqi;     // LQI value.
  Int8  rssi;    // RSSI value in dBm.

} NetNodeLqi;

/******************************************************************************/
typedef struct // Children address list.
{
   Uint8     size; // Children address information descriptor size.
   NetAllAddr* pAddr; // Children address information array.
} NetChildrenList;

/******************************************************************************/

/*******************************************************************************/
typedef union // Parameters.
{
  NetNodeRole                        role;
  NetNodeCapability        nodeCapability;
  NetLogicalAddr              logicalAddr;
  NetNwkAddr                      nwkAddr;
  NetExtAddr                      extAddr;
  NetPanId                          panId;
  NetPanId                       actPanId;
  NetChannel                      channel;
  NetChannelMask              channelMask;
  NetNodeLqi                      nodeLqi;
  NetTxPower                      txPower;
  NetAllAddr                   parentAddr;
  NetChildrenList            childrenList;
  NetAutoNetwork              autoNetwork;
  NetPowerDuration          powerDuration;
  NetSyncPeriod                syncPeriod;
  Uint128                     securityKey;
  const char**                      ppStr;
  NetDeliveryTimeout  dataDeliveryTimeout;
  Uint8                      dataRetryMax;
  Uint16                     calibrPeriod;
  Bool                            uartDtr;
} NetParamValue;

/*******************************************************************************/
typedef struct // Parameter structure.
{
  NetParamId    id;       // Parameter ID.
  NetParamValue value; // Parameter value.
  Bool          isDefault; //
  NetStatus status;
} NetParam;

/*******************************************************************************
 Events
*******************************************************************************/

typedef enum
{
  NET_EVENT_JOINED,
  NET_EVENT_LOST,
  NET_EVENT_ADD_CHILD,
  NET_EVENT_DELETE_CHILD,
  NET_EVENT_READY_TO_SLEEP         //for ED

} NetEventId;

/*******************************************************************************/
typedef struct
{
  NetEventId id;
  union
  {
    NetAllAddr childAddr; // for NET_EVENT_ADD_CHILD, NET_EVENT_DELETE_CHILD
  } param;

} NetEvent;



/*******************************************************************************/
typedef struct
{
  Uint16      addr;           // eeprom address
  Uint8*      data;           // pointer to data memory
  Uint16      length;         // number of bytes
  EepromStatus  status;         // contains result of operation

} EepromParams;

/*******************************************************************************
 Interface functions
*******************************************************************************/

typedef struct
{
  struct Net* pNet;

  // common
  Bool (*getEvent)(struct Net* pNet, NetEvent *event);
  void (*stackRestart)(struct Net* pNet);
  void (*warmReset)(struct Net* pNet, Bool factoryDefaults);

  // network
  void (*join)(struct Net* pNet);
  void (*leave)(struct Net* pNet);
  Bool (*isJoined)(struct Net* pNet);

  // parameters
  void (*setParam)(struct Net* pNet, NetParam *param);
  void (*getParam)(struct Net* pNet, NetParam *param);

  // power mgmt
  void (*setPwrMng)(struct Net* pNet, Bool enable);
  void (*forceToSleep)(struct Net* pNet);
  Bool (*appReadyToSleep)(struct Net* pNet);
  void (*forceWakeup)(struct Net* pNet);

  //eeprom
  void (*eepromWrite)(struct Net* pNet, EepromParams *params);
  void (*eepromRead)(struct Net* pNet, EepromParams *params);

} NetMngFxns;

#endif // NET_MNG_FXNS_H_

//eof net_mng_fxns.h
