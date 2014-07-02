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
  NWK.h
  
  Type and constants declarations for NWK.


   Changes:
  Lilya Butorina
  - *bitset removed
  - leave-status fieds added to NIB
********************************************************************************/

#ifndef _NWK_H_
#define _NWK_H_

#include "MAC.h"
#include "conf.h"
#include "NWKFramesDef.h"
#include "Id.h"

//#define DEBUG_UART
#ifdef DEBUG_UART
void putToUART(uint8_t byte) __attribute__((C, spontaneous));
void initUART() __attribute__((C, spontaneous));
#else
#define putToUART(a); 
#define initUART(); 
#endif

//#define NWK_TRACE
#ifdef NWK_TRACE
#include "leds.h"
#include "uart.h"

 extern void processNWKerror(char*s) __attribute__((C, spontaneous));
 extern void initNWKtrace() __attribute__((C, spontaneous));
 extern void chrNWKtrace(char c) __attribute__((C, spontaneous));
 extern void strNWKtrace(char *s) __attribute__((C, spontaneous));
 extern void ui8NWKtrace(uint8_t i) __attribute__((C, spontaneous));
 extern void ux8NWKtrace(uint8_t i) __attribute__((C, spontaneous));
 extern void ux16NWKtrace(uint16_t i) __attribute__((C, spontaneous));

#else 
  #define processNWKerror(s); {}
  #define initNWKtrace();    {}
  #define chrNWKtrace(c);    {} 
  #define strNWKtrace(s);    {} 
  #define ui8NWKtrace(i);    {}
  #define ux8NWKtrace(i);    {}
  #define ux16NWKtrace(i);   {}
#endif

  #define RING_SET_CLEAN(n,h)    { n = 0; h = 0; }
  // expectation: ring not fill (n < N)
  #define RING_PUT_TAIL(N,n,h,t) { t = h + n; if(t >= N) t -= N; n++; }
  // expectation: ring not empty (n > 0)
  #define RING_POP_HEAD(N,n,h)   { n--; h++; if(h == N) h = 0;        }

typedef enum
{
  NWK_NO_STATE,
  NWK_STATE_FAIL,
  NWK_STATE_RESET,
  NWK_STATE_OUT_OF_NETWORK,
  NWK_STATE_JOINED,
  NWK_STATE_IN_NETWORK,
} NWKState;

// Any entry must not be 0
typedef enum
{
  NWK_PROC_IDLE = 1,
  NWK_PROC_INIT,
  NWK_PROC_START,
  NWK_PROC_STOP,
  NWK_PROC_RESET,
  NWK_PROC_DISCOVERY,
  NWK_PROC_FORMATION,  
  NWK_PROC_JOIN,
  NWK_PROC_REPAIR,
  NWK_PROC_START_ROUTER,
  NWK_PROC_PERMIT_JOINING,
  NWK_PROC_LEAVE,
} NWKProcedure;

typedef enum
{
  NWK_SUCCESS           = 0x00,
  NWK_INVALID_PARAMETER = 0xc1,
  NWK_INVALID_REQUEST   = 0xc2,
  NWK_NOT_PERMITTED     = 0xc3,
  NWK_STARTUP_FAILURE   = 0xc4,
  NWK_ALREADY_PRESENT   = 0xc5,
  NWK_SYNC_FAILURE      = 0xc6,
  NWK_TABLE_FULL        = 0xc7,
  NWK_UNKNOWN_DEVICE    = 0xc8,
  NWK_UNSUPPORTED_ATTRIBUTE = 0xc9,
  NWK_NO_NETWORKS       = 0xca,
  NWK_LEAVE_UNCONFIRMED = 0xcb,
  NWK_MAX_FRM           = 0xcc,
  NWK_NO_KEY            = 0xcd,
  NWK_BAD_CCM_OUTPUT    = 0xce,
  NWK_MEMORY_OVERFLOW   = 0xfd,
  NWK_MAC_BUSY          = 0xfe,
  NWK_BUSY              = 0xff,
} NWKSRVStatus;

typedef enum
{
  REL_PARENT,
  REL_CHILD,
  REL_SIBLING,
  REL_OTHER,
} NWKRelation;

typedef enum
{
  NWK_CAP_COORD = 0,
  NWK_CAP_DEVICE_TYPE = 1,
  NWK_CAP_POWER = 2,
  NWK_CAP_RXON_IDLE = 3,
  NWK_CAP_SECURITY = 6,
  NWK_CAP_NEED_SHORT_ADDR = 7,
} NWKCapabilitiesShifts;

typedef enum
{
  Undefined = 0xff,
  ZigBeeCoord = 0x00,
  ZigBeeRouter = 0x01,
  ZigBeeEndDev = 0x02
} NWKDeviceType;

enum
{
#if 0
// implemented as MACROS _COORDINATOR_CAPABLE
  nwkcCoordinatorCapable,
#endif
  nwkcDefaultSecurutyLevel =    0x00,
  nwkcDiscoveryRetryLimit =     0x03,
  nwkcMaxDepth =                0x0f,
  nwkcMaxFrameOverhead =        0x08,
  nwkcMaxPayloadSize =          92,
  nwkcProtocolVersion =         0x01,
  nwkcRepairThreshold =         0x03,

// NIB constants
#if defined PLATFORM_RAT || defined PLATFORM_PC
  uint32_t nwkcRouteDiscoveryTime = ROUTE_DISC_TIME;
#else
  nwkcRouteDiscoveryTime =      0x2710,
#endif

  nwkcMaxBroadcastJitter =      0x40,

  nwkcInitialRREQRetries =      0x03,
  nwkcRREQRetries =             0x02,

  nwkcRREQRetryInterval =       0xfe,
  nwkcMinRREQJitter =           0x01,
  nwkcMaxRREQJitter =           0x40,
};

typedef enum
{
  ACTIVE,
  DISCOVERY_UNDERWAY,
  DISCOVERY_FAILED,
  INACTIVE,
  NO_WAY = 0xff,
} NWKRTStatus;

enum
{
  ERR_NO_ROUTE_AVAILABLE,
  ERR_TREE_LINK_FAILURE,
  ERR_NON_TREE_LINK_FAILURE,
  ERR_LOW_BATTERY_LEVEL,
  ERR_NO_ROUTING_CAPACITY,
};

#ifdef _SECURITY
enum
{
  NWK_KEY_LEN = 16,
  IN_FRAMERS_MAX = 10,
};
#endif

typedef struct
{
 uint16_t       On : 1; // 1 == own child leaving started (at the end confirm APL)
 uint16_t     Done : 1; // 1 == finished child leaving
 uint16_t sendNeed : 1; // 1 == need to send LEAVE_CMD 
 uint16_t     Send : 1; // 1 == LEAVE_CMD was sent, waiting for sendDone event  
 uint16_t sendDone : 1; // 1 == sendDone event received
 uint16_t  indNeed : 1; // 1 == waiting for receiving LEAV_CMD
 uint16_t  indDone : 1; // 1 == LEAVE_IND command was received
 uint16_t dassDone : 1; // 1 == MLME_DISASSOCIATION.indication() was received
 uint16_t sendRetries : 4; // num of send retries of LEAVE_CMD during lvTime period
 uint16_t timeRetries : 4; // num of lvTime periods 
} PACK lvStatus_t;

typedef struct
{
// standard part,
// mandatory fields:

  MACExtAddr extendedAddress;
  MACShortAddr networkAddress;
  NWKDeviceType deviceType;

// optional fields

  bool rxOnWhenIdle;
  uint8_t LQI;
  int8_t RSSI;

  uint8_t failuresCounter;

  lvStatus_t    lv;
  bool          lvRemoveChildren;
  uint16_t      lvTime;
  NWKSRVStatus  lvResult;

} PACK NWKChildNeighborTable_t;

typedef struct
{
// standard part,
// mandatory fields:

  MACPANId PANId;
  MACExtAddr extendedAddress;
  MACShortAddr networkAddress;
  NWKDeviceType deviceType;

// optional fields
  NWKRelation relationship;
  uint8_t depth;
  uint8_t beaconOrder;

  //!!TODO convert to the bit-fields
  bool rxOnWhenIdle;
  bool permitEndDevJoining;
  bool permitRouterJoining;
  bool potentialParent;

  uint8_t LQI;
  int8_t RSSI;
  uint8_t logicalChannel;

#if 0
  uint32_t incomingBeaconTimestamp;
  uint32_t beaconTransmitionTimeOffset;
#endif

#if 0
  uint8_t transmitFailure;
#endif
} PACK NWKNeighborTable_t;

typedef enum
{
  ACK_FALSE = 0,
  ACK_TRUE = 1,
  ACK_FREE = 2,
} ACK_t;

typedef enum
{
  SUPPRESS_RD = 0,
  ENABLE_RD = 1,
  FORCE_RD = 2,
} RouteFlag_t;

typedef struct
{
  uint8_t BCSN;
  MACShortAddr bcSrcAddress;
  ACK_t acked;
} PACK NWKBTT_t;

typedef struct
{
  MACShortAddr dstAddr;
  NWKRTStatus  status; // NO_WAY - empty entry
  MACShortAddr nextHop;
  uint8_t      failuresCounter; // must increased for all entries with the same nextHop
  int16_t      rdtCounter; // counter of Route Discovery sessions for entry
} PACK NWKRT_t;

typedef struct
{
  uint8_t      rreqId;            // 0 - empty entry
  MACShortAddr srcAddr;           // RREQ Src
  MACShortAddr senderAddr;        // best (lowest cost) next hop for RREP transmission
  uint8_t      forwardCost;
  uint8_t      residualCost;
  uint16_t     expTime;           // milleseconds until route discovery expires
  
  uint16_t     rreqTime;          // milliseconds until next RREQ 
  uint8_t      rreqRetries;       // counter of RREQ transmissions

  MACShortAddr dataSenderAddr;    //source Node of DataFrame-reason of Mesh Discovery
  // on expTime out if(srcAddr != dataSrcAddr && srcAddr == nibPtr->ownAddr) RERRsend
  
  NWKRT_t      *rt;               //link with NWKRT-entry
  uint8_t      radius;            // radius of RREQ-frame, sended on RDT-entry

  bool         repair;            // flag is set only on device emitted RREQ

  int16_t      rtIndex;             //obsolet TODO - delete
  MACShortAddr dataSenderAddress;    //obsolet TODO - delete
} PACK NWKRDT_t;


typedef struct  // indirect transaction description
{
  uint8_t  hdl;   // src handle
  uint8_t *skb;   // link to skb-elsement (DATAFrame/LeaveFrame/macParams) of indirect Tx queue
  uint8_t *bf;    // pointer to alocated SKBUFF buffer of macParams (init after compose*Frame)
  uint8_t *msdu;  // pointer to msdu in MAC-format frame (init after compose*Frame)
  MACShortAddr dstAddr; //for LEAVEFrame = desteny, for DATAFrame = 0
} NWKIT_t;

#ifdef _SECURITY
//The incoming frame counter (for each node)
typedef struct _NWKInFrameCounter_
{
  MACExtAddr addr; //node address
  uint32_t inCounter; //incoming frames counter
} NWKInFrameCounter;

//The security material description
typedef struct _NWKSecMat_t_
{
  uint8_t keySeqNum; //key sequence number
  uint32_t outFrameCounter; //outgoing frame counter
  NWKInFrameCounter inFrameCounters[IN_FRAMERS_MAX]; //maximal number of 
  uint8_t key[NWK_KEY_LEN]; //cifering key
} NWKSecMat_t;
#endif

typedef struct
{
// Standard part
  uint8_t nwkSequenceNumber;
  uint8_t nwkPassiveAckTimeout;
  uint8_t nwkMaxBroadcastRetries;
  uint8_t nwkMaxDepth;
  // ZRD-children takes entries [0 nwkMaxRouters], then places ZED-children
  NWKChildNeighborTable_t * nwkChildNeighborTable;
  uint8_t nwkMaxChildren;
  NWKNeighborTable_t * nwkNeighborTable;
  uint8_t nwkMaxNeighbors;
  uint8_t nwkNetworkBroadcastDeliveryTime;
  uint8_t nwkReportConstantCost;
  uint8_t nwkRouteDiscoveryRetriesPermitted;

#ifdef _WITH_MESH_
  NWKRT_t * nwkRouteTable;
  uint8_t nwkMaxRouteTable;
  NWKRDT_t * nwkRouteDiscoveryTable;
  uint8_t nwkMaxRouteDiscoveryTable;
#endif

  uint8_t nwkMaxRouters;
  bool nwkSymLink;
  uint8_t nwkCapabilityInformation;
  bool nwkUseTreeAddressAlloc;
  bool nwkUseTreeRouting;
  uint16_t nwkNextAddress;
  uint16_t nwkAvailableAddresses; // is used for both addressing schemes !!!
  uint16_t nwkAddressIncrement;
  uint16_t nwkTransactionPersistenceTime;
// End of standard part

// Implementer-dependent part
  NWKProcedure nwkProcedure;
  NWKState nwkState;
  NWKDeviceType nwkDeviceType;
  
#ifdef _WITH_BROADCAST_
  NWKBTT_t * nwkBroadcastTransactionTable;
  uint8_t nwkMaxBTT;
#endif
  
  uint8_t rreqId;
  
  uint8_t beaconPayload[MAX_BEACON_PAYLOAD_LENGTH + MAC_BEACON_HEADER_LENGTH_MAX];
  PANDescriptor * panDescriptor;
  uint8_t nwkMaxPANDescriptors;
  // assume, that BEACON_NOTIFY are coming in the same order as ACTIVE SCAN results arrive
  
  uint8_t energyLevel;
  uint8_t curChannel;
  
  uint8_t stackProfile;
  uint16_t cskip;        // number of all descendants for every own router-child
  uint16_t cskip_1;      // number of all own descendants 
  uint8_t depth;
  MACExtAddr parentExtAddr;
  MACShortAddr parentAddr;
  MACShortAddr ownAddr;
  MACExtAddr ownExtAddress;
  MACPANId panid;
  uint8_t networkCountRequested;
  
  uint32_t channels;  // save it to repair tree
  uint8_t duration;   // save it to repair tree

#if (defined _WITH_INDIRECT_TX_)// && (!defined _ZBED_)
  uint8_t maxIndirectTransactions;
  NWKIT_t * IndirectTransactions; 
#endif

  bool indirectTxOverflow;

#ifdef _SECURITY
  //Security parameters
  bool nwkSecureAllFrames; //if NWK level has to securely process frames
  uint8_t nwkActiveKeySeqNumber; //Active key sequence number
  /* Indicates whether incoming NWK frames must be all checked for
   freshness when the memory for incoming frame counts is exceeded. */
  bool nwkAllFresh; 
  uint8_t nwkSecurityLevel; //The NWK security level
  NWKSecMat_t* nwkSecurityMaterialSet; //security material set
#endif

#ifndef _WO_REPAIR_ 
#ifdef _DBG_REPAIR_
//  dbg_repair_on set to 1 NWK_tester on command from WSN_tetster
// => next NLDEDataRM:SNDUC.sendDone force status=MAC_NO_ACK_STATUS & fill failureCounter 
// => start tree repair
  bool dbg_repair_on; 
#endif
#endif
} PACK NWKNIB_t;

typedef union
{
// Standard part
  uint8_t nwkSequenceNumber;
  uint8_t nwkPassiveAckTimeout;
  uint8_t nwkMaxBroadcastRetries;
  uint8_t nwkMaxChildren;
  uint8_t nwkMaxDepth;
  uint8_t nwkMaxRouters;
//  NWKNeighborTable_t * nwkNeighborTable;
  uint8_t nwkNetworkBroadcastDeliveryTime;
  uint8_t nwkReportConstantCost;
  uint8_t nwkRouteDiscoveryRetriesPermitted;
  void * nwkRouteTable;
  uint8_t nwkSecureAllFrames;
  uint8_t nwkSecurityLevel;
  bool nwkSymLink;
  uint8_t nwkCapabilityInformation;
  bool nwkUseTreeAddressAlloc;
  bool nwkUseTreeRouting;
  uint16_t nwkNextAddress;
  uint16_t nwkAvailableAddresses;
  uint16_t nwkAddressIncrement;
  uint16_t nwkTransactionPersistenceTime;
// End of standard part
} NWKNIBAttr_t;

typedef struct
{
  MACPANId panid;
  uint8_t logicalChannel;
  uint8_t stackProfile;
  uint8_t zigbeeVersion;
  uint8_t beaconOrder;
  uint8_t superframeOrder;
  bool permitRouterJoining;
  bool permitEndDevJoining;
} PACK NWKNetworkDescr_t;

typedef enum
{
  NWK_BCSN_ID =                               0x81,
  NWK_PASSIVE_ACK_TIMEOUT_ID =                0x82,
  NWK_MAX_BROADCAST_RETRIES_ID =              0x83,
  NWK_MAX_CHILDREN_ID =                       0x84,
  NWK_MAX_DEPTH_ID =                          0x85,
  NWK_MAX_ROUTERS_ID =                        0x86,
  NWK_NEIGHBOR_TABLE_ID =                     0x87,
  NWK_NETWORK_BROADCAST_DELIVERY_TIME_ID =    0x88,
  NWK_REPORT_CONSTANT_COST_ID =               0x89,
  NWK_ROUTE_DISCOVERY_RETRIES_PERMITTED_ID =  0x8a,
  NWK_ROUTE_TABLE_ID =                        0x8b,
  NWK_SECURE_ALL_FRAMES_ID =                  0x8c,
  NWK_SECURITY_LEVEL_ID =                     0x8d,
  NWK_SYMLINK_ID =                            0x8e,
  NWK_CAPABILITY_INFORMATION_ID =             0x8f,
  NWK_USE_TREE_ADDRESS_ALLOC_ID =             0x90,
  NWK_USE_TREE_ROUTING_ID =                   0x91,
  NWK_NEXT_ADDRESS_ID =                       0x92,
  NWK_AVAILABLE_ADDRESSES_ID =                0x93,
  NWK_ADDRESS_INCREMENT_ID =                  0x94,
  NWK_TRANSACTION_PERSISTENCE_TIME_ID =       0x95
} NWKNIBId;

enum
{
  NWK_PASSIVE_ACK_TIMEOUT_DEFAULT =           0x03,
  NWK_MAX_BROADCAST_RETRIES_DEFAULT =         0x03,
  NWK_MAX_CHILDREN_DEFAULT =                  0x07,
  NWK_MAX_DEPTH_DEFAULT =                     0x05,
  NWK_MAX_ROUTERS_DEFAULT =                   0x05,
  NWK_NETWORK_BROADCAST_DELIVERY_TIME_DEFAULT = 0x09,
  NWK_REPORT_CONSTANT_COST_DEFAULT =          0x00,
  NWK_ROUTE_DISCOVERY_RETRIES_PERMITTED_DEFAULT = nwkcDiscoveryRetryLimit,
  NWK_SYMLINK_DEFAULT =                       FALSE,
  NWK_CAPABILITY_INFORMATION_DEFAULT =        0x00,
  NWK_TRANSACTION_PERSISTENCE_TIME_DEFAULT =  0x01f4,
#ifdef _SECURITY
  NWK_SECURE_ALL_FRAMES_DEFAULT =             FALSE,
  NWK_ALL_FRESH_DEFAULT =                     TRUE,
  NWK_ACTIVE_KEY_DEFAULT =                    0x00,
  NWK_SECURITY_LEVEL_DEFAULT =                0x06,
#endif
};

enum
{
  NWK_PASSIVE_ACK_TIMEOUT_MAX =               0x0a,
  NWK_MAX_BROADCAST_RETRIES_MAX =             0x05,
  NWK_MAX_DEPTH_MAX =                         nwkcMaxDepth,
  NWK_REPORT_CONSTANT_COST_MAX =              0x01,
  NWK_ROUTE_DISCOVERY_RETRIES_PERMITTED_MAX = 0x03,
  NWK_SECURITY_ALL_FRAMES_MAX =               0x01,
  NWK_SECURITY_LEVEL_MAX =                    nwkcDefaultSecurutyLevel,
};

typedef struct
{
  uint32_t scanChannels;
  uint8_t scanDuration;
  uint8_t networkCount;
  NWKNetworkDescr_t * networkList;
  NWKSRVStatus status;
} PACK NWKDiscoveryReq_t;

typedef struct
{
  uint32_t scanChannels;
  uint8_t scanDuration;
  uint8_t beaconOrder;
  uint8_t superframeOrder;
  MACPANId * pPANId;
  bool BatteryLifeExtention;
} PACK NWKFormationReq_t;

typedef struct
{
 uint8_t beaconOrder;
 uint8_t superframeOrder;
 bool BatteryLifeExtention;
} PACK NWKStartRouterReq_t;

typedef struct
{
  MACPANId PANId;
  bool joinAsRouter;
  bool rejoinNetwork;
  uint32_t scanChannels;
  uint8_t scanDuration;
  uint8_t powerSource;
  uint8_t rxOnWhenIdle;
  bool MACSecurity;
} PACK NWKJoinReq_t;

typedef struct
{
  MACPANId PANId;
  NWKSRVStatus status;
} PACK NWKJoinResp_t;

typedef struct
{
  MACShortAddr shortAddr;
  MACExtAddr extAddr;
  uint8_t capability;
  bool MACSecureJoin;
} PACK NWKJoinInd_t;

typedef struct
{
  MACExtAddr deviceAddress;
  union
  {
    uint8_t capability;
    NWKSRVStatus status;
  } appendix;
} PACK NWKDirJoin_t;

typedef struct
{
  MACExtAddr *extAddr;
  bool removeChildren;
  NWKSRVStatus status;
} PACK NWKLeaveReq_t;

typedef struct
{
  MACExtAddr *extAddr;
  NWKSRVStatus status;
} PACK NWKLeaveConf_t;

typedef struct
{
  MACExtAddr *extAddr;
  bool removeChildren;
} PACK NWKLeaveInd_t;

typedef struct
{
  NWKNIBId attrId;
  uint8_t attrLen;
  NWKNIBAttr_t * attr;
  NWKSRVStatus status;
} PACK NWKAttr_t;

typedef struct
{
  MACShortAddr dstAddr;
  uint8_t nsduLength;
  uint8_t * nsdu;
  uint8_t nsduHandle;
  uint8_t radius;
  RouteFlag_t discoverRoute;
  bool securityEnable;
} PACK NWKDataReq_t;

typedef struct
{
  uint8_t nsduHandle;
  NWKSRVStatus status;
} PACK NWKDataResp_t;

typedef struct
{
  MACShortAddr srcAddr;
  uint8_t nsduLength;
  uint8_t * nsdu;
  uint8_t lq;
  int8_t rssi;
} PACK NWKDataInd_t;

typedef struct
{
  uint8_t capability;
  MACShortAddr addr;
  uint8_t index;
} PACK NWKChildNeighbor;

typedef enum
{
  OK,
  DROP_PACKET,
  LOST_RTCAP,
} NWKRTResult_t;

typedef enum
{
  J_JOINED,
  J_NOT_JOINED,     // there's no place for given device type in CNT
  J_LAST_JOINED,    // 
  J_FULL,
} CNTStatus;

#endif
// eof NWK.h
