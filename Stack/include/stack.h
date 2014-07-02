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
  stack.h
  
  eZeeNet library header file.
********************************************************************************/

#ifndef _EZEENET_H_
#define _EZEENET_H_

#include "tos.h"
#include "sbuf.h"

/********************************************************************************
* Base types
********************************************************************************/

typedef uint16_t PANID_t;
typedef uint64_t IEEEAddr_t;
typedef uint16_t NWKAddr_t;
typedef uint8_t  EndPoint_t;
typedef int8_t   Handle_t;
typedef uint16_t ProfileID_t;
typedef uint8_t  ClusterID_t;

/********************************************************************************
* ZDO Network management service types
********************************************************************************/

typedef struct
{
  uint32_t   stackVersion;
  bool       coordinatorCapability;
  bool       routerCapability;
  bool       endDeviceCapability;
} StackInfo_t;

typedef struct
{
  uint32_t   channelMask;
  PANID_t    panID;
  IEEEAddr_t ownExtAddress;
} NetworkStartParams_t;

typedef struct
{
  NWKAddr_t  ownNetworkAddress;
  IEEEAddr_t parentExtAddress;
  NWKAddr_t  parentNetworkAddress;
  uint8_t    channel;
  PANID_t    panID;
} NetworkJoinParams_t;

typedef struct
{
  NWKAddr_t  networkAddr;
  IEEEAddr_t extAddr;
} NodeAddrInf_t;

typedef uint8_t ZDOStatus_t;
typedef uint8_t NodeLogicalType_t;
typedef uint8_t APSStatus_t;

enum
{
  ZDO_SUCCESS           = 0,
  ZDO_NO_ACK            = 1,
  ZDO_NETWORK_NOT_FOUND = 2,
  ZDO_LEFT_BY_PARENT    = 3,
  ZDO_NO_DATA           = 4,
  ZDO_INTERNAL_ERROR    = 5,
  ZDO_DATA_FAIL         = 6
}; // ZDOStatus_t

enum
{
  ZIGBEE_COORDINATOR_TYPE = 0,
  ZIGBEE_ROUTER_TYPE      = 1,
  ZIGBEE_END_DEVICE_TYPE  = 2
}; // NodeLogicalType_t

enum
{ 
  APS_SUCCESS_STATUS         = 0x00,
  APS_NO_BOUND_DEVICE_STATUS = 0xA0, /*TBD*/
  APS_TRANSACTION_OVERFLOW   = 0xF1,
  APS_SECURITY_FAIL_STATUS   = 0xA1, /*TBD*/
  APS_NO_ACK_STATUS          = 0xA2, /*TBD*/
  APS_NACK_STATUS            = 0xA3  /*TBD*/
}; // APSStatus_t 

#define MAX_DATA_PERIOD_ms  1006000L     /* Limit for dataPeriod parameter of ZDOM__TRACKING__setPeriods */
#define MAX_LIFETIMEOUT_ms  0x7FFFFFFFL  /* Limit for lifeTimeout parameter of ZDOM__TRACKING__setPeriods */
/********************************************************************************
* ZDO Network management service 
********************************************************************************/

/*******************************************************************************
 Description: init CControl interface.

 Parameters: initDone - callback function to notify end of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__CControl__init(void (*initDone)(void));
/*******************************************************************************
 Description: start CControl interface.

 Parameters: startDone - callback function to notify end of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__CControl__start(void (*startDone)(void));
/*******************************************************************************
 Description: stop CControl interface.

 Parameters: stopDone - callback function to notify end of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__CControl__stop(void (*stopDone)(void));

/*******************************************************************************
 Description: set device type.

 Parameters: type - device type.

 Returns: SUCCESS, if new device type accepted, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__DeviceType__set(NodeLogicalType_t type);

/*******************************************************************************
 Description: get device type and stack version.

 Parameters: info - pointer to variable for device information.

 Returns: nothing.
*******************************************************************************/
void ZDOM__DeviceType__get(StackInfo_t *info);

/*******************************************************************************
 Description: open network and register cllback functions.

 Parameters: joined - callback function for joined event.
          frozen - callback function for frozen event.
          lost - callback function for lost event.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__NETWORK__open(void (*joined)(NetworkJoinParams_t *params), void (*frozen)(void), void (*lost)(ZDOStatus_t status));

/*******************************************************************************
 Description: close network.

 Parameters: nothing.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__NETWORK__close(void);

/*******************************************************************************
 Description: start network.

 Parameters: params - params of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__NETWORK__start(NetworkStartParams_t *params);

/*******************************************************************************
 Description: stop network.

 Parameters: stopDone - callback function to notify end of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__NETWORK__stop(void (*stopDone)(void));

/*******************************************************************************
 Description: open command of topology interface.

 Parameters: addNode - add none event handler.
          deleteNode - delete node event handler.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__TOPOLOGY__open(void (*addNode)(NodeAddrInf_t *addr), void (*deleteNode)(NodeAddrInf_t *addr));

/*******************************************************************************
 Description: close command of topoloy interface.

 Parameters: nothing.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__TOPOLOGY__close(void);

/*******************************************************************************
 Description: get LQ command of topology interface.

 Parameters: addr - network address.
          lqi - pointer to LQI variable.
          rssi - pointer to RSSI variable.

 Returns: result of LQ request from NIB.
*******************************************************************************/
result_t ZDOM__TOPOLOGY__getLQ(NWKAddr_t addr, uint8_t* lqi, int8_t* rssi);

/*******************************************************************************
 Description: set tracking periods.

 Parameters: dataPeriod - awake time.
          lifeTimeout - sleep time.
          setDone - callback function that notify end of operation.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__TRACKING__setPeriods(uint32_t dataPeriod, uint32_t lifeTimeout, void (*setDone)(APSStatus_t status));

/*******************************************************************************
 Description: TRACKING.sync - syncronize end device with router.

 Parameters: syncDone - callback function on sync done event.
          (status - ZDO_SUCCESS - incoming data present, TRACKING.sync needs 
                                  to be called again after data processing,
                    ZDO_NO_DATA - no incoming data present,
                    ZDO_NO_ACK  - syncronisation with parent failed due to parent
                                  loss or channel busy ("network lost" callback
                                  will follow),
                    ZDO_INTERNAL_ERROR - NLME_SYNC failed with 
                                          NWK_INVALID_PARAMETER 
                                          or due to another unkwoun reason.

 Returns: FAIL if device is coordinator, or not in network, or NULL callback pointer passed,
        BUSY if syncronization is already running,
        SUCCESS if syncronization started and the callback will be invoked.
*******************************************************************************/
result_t ZDOM__TRACKING__sync(void (*syncDone)(ZDOStatus_t status));

/*******************************************************************************
 Description: traceroute open.

 Parameters: ind - callback function that notify traceroute has been opened.

 Returns: FAIL.
*******************************************************************************/
result_t ZDOM__TRACEROUTE__open(void (*ind)(uint8_t handle));

/*******************************************************************************
 Description: traceroute close.

 Parameters: nothing

 Returns: FAIL.
*******************************************************************************/
result_t ZDOM__TRACEROUTE__close(void);

/*******************************************************************************
 Description: traceroute open.

 Parameters: dstAddr - destination NWK address.
          handle - hanler number.
          conf - callback function for confirm handler.

 Returns: FAIL.
*******************************************************************************/
result_t ZDOM__TRACEROUTE__request(NWKAddr_t dstAddr, uint8_t handle, void (*conf)(void));

/*******************************************************************************
 Description: sleep command of power interface.

 Parameters: sleepDone - sleep done handler.

 Returns: FAIL if sleepDone handler doesn't exists, SUCCES otherwise.
*******************************************************************************/
result_t ZDOM__Power__sleep(void (*sleepDone)(void));

/*******************************************************************************
 Description: wakeup command of power interface.

 Parameters: wakeupDone - wakeup done hanler.

 Returns: SUCCESS, if request was accepted for processing, FAIL otherwise.
*******************************************************************************/
result_t ZDOM__Power__wakeup(void (*wakeupDone)(void));

/********************************************************************************
* APS Data service types
********************************************************************************/

#define APS_GARBAGE_EP 0xFF

#define APS_OPEN_ERROR_ALREADY_REGISTERED -1
#define APS_OPEN_ERROR_NULL_CALLBACK      -2
#define APS_OPEN_ERROR_NO_MORE_USERS      -3	      

#define APS_MIN_ACK_WAIT_DURATION_ms  100
#define APS_MAX_ACK_WAIT_DURATION_ms 6300

#define APS_MAX_PAYLOAD_LENGTH      88

typedef uint8_t APSDiscoverRoute_t;
typedef uint8_t APSAddrMode_t;
typedef uint8_t APSTxOptions_t;
typedef uint8_t APSSecurityStatus_t; 

enum
{
  APS_SUPPRESS_ROUTE_DISCOVERY  = 0x00,
  APS_ENABLE_ROUTE_DISCOVERY    = 0x01,
  APS_FORCE_ROUTE_DISCOVERY     = 0x02
}; // APSDiscoverRoute_t

enum
{
  APS_NO_ADDR_MODE    = 0x00,
  APS_SHORT_ADDR_MODE = 0x01,
  APS_EXT_ADDR_MODE   = 0x02
}; //  APSAddrMode_t

enum
{
  APS_NO_ACK       = 0x00,
  APS_SECURITY     = 0x01,
  APS_USE_NWK_KEY  = 0x02,
  APS_ACK          = 0x04
}; // APSTxOptions_t

enum
{
  APS_UNSECURED = 0x0,
  APS_SECURED_NWK_KEY = 0x1,
  APS_SECURED_LINK_KEY = 0x2
}; // APSSecurityStatus_t

typedef union
{
  NWKAddr_t  networkAddr;
  IEEEAddr_t extAddr;
} DeviceAddr_t;

typedef struct
{
  uint8_t rxHeader;
  uint8_t rxFooter;
} APSInitParams_t;

typedef struct
{
  EndPoint_t          dstEndpoint;
  APSAddrMode_t       srcAddrMode;
  DeviceAddr_t        srcAddr;
  EndPoint_t          srcEndpoint;
  ProfileID_t         profile;
  ClusterID_t         cluster;
  Sbuf_t *            data;
  bool                wasBroadcast;
  APSSecurityStatus_t securityStatus;
  uint8_t             lqi;                   // LQI value.
  int8_t              rssi;                  // RSSI value.
} APSIndication_t;

typedef struct
{
  APSAddrMode_t       dstAddrMode;
  DeviceAddr_t        dstAddr;
  EndPoint_t          dstEndpoint;
  ProfileID_t         profile;
  ClusterID_t         cluster;
  EndPoint_t          srcEndpoint;
  APSTxOptions_t      txOptions;
  APSDiscoverRoute_t  discoverRoute;
  uint8_t             radiusCounter;
  Sbuf_t *            data;
  APSStatus_t         status;
} APSRequest_t;

/********************************************************************************
* APS Data service 
********************************************************************************/

void     APSMsgM__APSDE__getAffixInfo(APSTxOptions_t txOptions, uint8_t* header, uint8_t* footer);
result_t APSMsgM__APSDE__init (APSInitParams_t* params);
Handle_t APSMsgM__APSDE__open(EndPoint_t endPoint, result_t (*ind)(APSIndication_t*));
result_t APSMsgM__APSDE__request(Handle_t handle, APSRequest_t* params, void (*conf)(APSRequest_t*));
result_t APSMsgM__APSDE__close(Handle_t handle);
result_t APSMsgM__APSDE__rxBufferRelease(Handle_t handle, APSIndication_t* buffer);
result_t APSMsgM__APSDE__rxControl(Handle_t handle, bool enable);
result_t APSMsgM__APSDE__registerRxBufferEmpty(void (*empty)(void));

/*******************************************
*   For router with power management
********************************************/

void NIBM__NO_CHILDREN_BEACON__request(void);
void APL__NO_CHILDREN_BEACON__confirm(void);
void ParSuppM__DISABLE_RX__turnOff(void);
void ParSuppM__DISABLE_RX__turnOn(void);
void APL__DISABLE_RX__confirm(void);
#endif

//eof stack.h
