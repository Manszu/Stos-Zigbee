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
  MAC.h
  
  Types and constants declaration for IEEE802.15.4 MAC implementation.
********************************************************************************/

#ifndef _MAC_H
#define _MAC_H

#ifndef PACK
#ifdef PACK_STRUCTURES
#define PACK __attribute__ ((packed))
#else
#define PACK
#endif
#endif

#include "MACAddrTypes.h"
#include "tos.h"

typedef uint8_t PHYEnergyLevel; // Energy level type.

enum
{
  PHY_MAX_FRAME_SIZE   = 128,  // Max size of PSDU, including length (1 byte), data and CRC (2 bytes).
  PHY_MIN_ENERGY_LEVEL = 0x00,
  PHY_MAX_ENERGY_LEVEL = 0xFF
};

typedef enum // PPDU link quality type.
{
  PHY_LINK_QUALITY_LEVEL_0 = 0x00,
  PHY_LINK_QUALITY_LEVEL_1 = 0x20,
  PHY_LINK_QUALITY_LEVEL_2 = 0x40,
  PHY_LINK_QUALITY_LEVEL_3 = 0x60,
  PHY_LINK_QUALITY_LEVEL_4 = 0x80,
  PHY_LINK_QUALITY_LEVEL_5 = 0xA0,
  PHY_LINK_QUALITY_LEVEL_6 = 0xC0,
  PHY_LINK_QUALITY_LEVEL_7 = 0xE0
} PHYLinkQuality;

enum // PPDU link quality mask.
{
  PHY_LINK_QUALITY_MASK = 0x1F
};

typedef enum // MAC primitives return codes.
{
  MAC_SUCCESS_STATUS                = 0x00, // The requested operation was completed 
                                            // successfully. For a transmission request,
                                            // this value indicates successful transmission.
  MAC_PAN_AT_CAPACITY_STATUS        = 0x01,
  MAC_PAN_ACCESS_DENIED_STATUS      = 0x02,
  MAC_BEACON_LOSS_STATUS            = 0xE0,
  MAC_CHANNEL_ACCESS_FAILURE_STATUS = 0xE1,
  MAC_DENIED_STATUS                 = 0xE2,
  MAC_DISABLE_TRX_FAILURE_STATUS    = 0xE3,
  MAC_FAILED_SECURITY_CHECK_STATUS  = 0xE4,
  MAC_FRAME_TOO_LONG_STATUS         = 0xE5,
  MAC_INVALID_GTS_STATUS            = 0xE6,
  MAC_INVALID_HANDLE_STATUS         = 0xE7,
  MAC_INVALID_PARAMETER_STATUS      = 0xE8,
  MAC_NO_ACK_STATUS                 = 0xE9,
  MAC_NO_BEACON_STATUS              = 0xEA,
  MAC_NO_DATA_STATUS                = 0xEB,
  MAC_NO_SHORT_ADDRESS_STATUS       = 0xEC,
  MAC_OUT_OFF_CAP_STATUS            = 0xED,
  MAC_PAN_ID_CONFLICT_STATUS        = 0xEE,
  MAC_REALIGNMENT_STATUS            = 0xEF,
  MAC_TRANSACTION_EXPIRED_STATUS    = 0xF0,
  MAC_TRANSACTION_OVERFLOW_STATUS   = 0xF1,
  MAC_TX_ACTIVE_STATUS              = 0xF2,
  MAC_UNAVAILABLE_KEY_STATUS        = 0xF3,
  MAC_UNSUPPORT_ATTRIBUTE_STATUS    = 0xF4
} MACStatus;

typedef enum // MAC PIB attribute ID type.
{
  MAC_PIB_ACK_WAIT_DURATION_ID                = 0x40,
  MAC_PIB_ASSOCIATION_PERMIT_ID               = 0x41,
  MAC_PIB_AUTO_REQUEST_ID                     = 0x42,
  MAC_PIB_BATT_LIFE_EXT_ID                    = 0x43,
  MAC_PIB_BATT_LIFE_EXT_PERIODS_ID            = 0x44,
  MAC_PIB_BEACON_PAYLOAD_ID                   = 0x45,
  MAC_PIB_BEACON_PAYLOAD_LENGTH_ID            = 0x46,
  MAC_PIB_BEACON_ORDER_ID                     = 0x47,
  MAC_PIB_BEACON_TX_TIME_ID                   = 0x48,
  MAC_PIB_BSN_ID                              = 0x49,
  MAC_PIB_COORD_EXT_ADDR_ID                   = 0x4A,
  MAC_PIB_COORD_SHORT_ADDR_ID                 = 0x4B,
  MAC_PIB_DSN_ID                              = 0x4C,
  MAC_PIB_GTS_PERMIT_ID                       = 0x4D,
  MAC_PIB_MAX_CSMA_BACKOFFS_ID                = 0x4E,
  MAC_PIB_MIN_BE_ID                           = 0x4F,
  MAC_PIB_PANID_ID                            = 0x50,
  MAC_PIB_PROMISCUOUS_MODE_ID                 = 0x51,
  MAC_PIB_RX_ON_WHEN_IDLE_ID                  = 0x52,
  MAC_PIB_SHORT_ADDR_ID                       = 0x53,
  MAC_PIB_EXT_ADDR_ID                         = 0x6F,
  MAC_PIB_SUPERFRAME_ORDER_ID                 = 0x54,
  MAC_PIB_TRANSACTION_PERSISTENCE_TIME_ID     = 0x55,
} MACPIBId;

enum // MAC constants.
{
  MAC_BASE_SLOT_DURATION        = 60,
  MAC_NUM_SUPERFRAME_SLOTS      = 16,
  MAC_BASE_SUPERFRAME_DURATION  = 960,
  MAC_MAX_BE                    = 5,
  MAC_MAX_BEACON_OVERHEAD       = 75,
  MAC_MAX_BEACON_PAYLOAD_LENGTH = 52,
  MAC_GTS_DESC_PERSISTENCE_TIME = 4,
  MAC_MAX_FRAME_OVERHEAD        = 25,
  MAC_MAX_FRAME_RESPONSE_TIME   = 1220,
  MAC_MAX_FRAME_RETRIES         = 3,
  MAC_MAX_LOST_BEACONS          = 4,
  MAC_MAX_FRAME_SIZE            = 102,
  MAC_MAX_SIFS_FRAME_SIZE       = 18,
  MAC_MIN_CAP_LENGTH            = 440,
  MAC_MIN_LIFS_PERIOD           = 40,
  MAC_MIN_SIFS_PERIOD           = 12,
  MAC_RESPONSE_WAIT_TIME        = 30720,
  MAC_UNIT_BACKOFF_PERIOD       = 20
};

enum
{
  MAC_BROADCAST_PANID      = 0xFFFF, // Broadcast PANID.
  MAC_BROADCAST_SHORT_ADDR = 0xFFFF, // Broadcast short address, no association.
  MAC_NO_SHORT_ADDR        = 0xFFFE, // After association no short address.
  MAC_GTS_MAX             = 7,       // Max of GTS.
  MAC_PENDING_ADDRESS_MAX = 7,       // Max of pending address in beacon.
};

enum // MAC PIB constants.
{
  MAC_PIB_ACK_WAIT_DURATION_54                     = 54,
  MAC_PIB_ACK_WAIT_DURATION_120                    = 120,
  MAC_PIB_ACK_WAIT_DURATION_DEFAULT                = MAC_PIB_ACK_WAIT_DURATION_54,
  MAC_PIB_ASSOCIATION_PERMIT_DEFAULT               = FALSE,
  MAC_PIB_AUTO_REQUEST_DEFAULT                     = TRUE,
  MAC_PIB_BATT_LIFE_EXT_DEFAULT                    = FALSE,
  MAC_PIB_BATT_LIFE_EXT_PERIODS_6                  = 6,
  MAC_PIB_BATT_LIFE_EXT_PERIODS_8                  = 8,
  MAC_PIB_BATT_LIFE_EXT_PERIODS_DEFAULT            = MAC_PIB_BATT_LIFE_EXT_PERIODS_6,
  MAC_PIB_BEACON_PAYLOAD_DEFAULT                   = 0,
  MAC_PIB_BEACON_PAYLOAD_LENGTH_MAX                = MAC_MAX_BEACON_PAYLOAD_LENGTH,
  MAC_PIB_BEACON_PAYLOAD_LENGTH_DEFAULT            = 0,
  MAC_PIB_BEACON_ORDER_MAX                         = 15, // Max beacon order.
  MAC_PIB_BEACON_ORDER_DEFAULT                     = MAC_PIB_BEACON_ORDER_MAX,
  MAC_PIB_BEACON_TX_TIME_DEFAULT                   = 0,
  MAC_PIB_COORD_SHORT_ADDR_DEFAULT                 = 0xFFFF,
  MAC_PIB_GTS_PERMIT_DEFAULT                       = TRUE,
  MAC_PIB_MAX_CSMA_BACKOFFS_MAX                    = 5,
  MAC_PIB_MAX_CSMA_BACKOFFS_DEFAULT                = 4,
  MAC_PIB_MIN_BE_MAX                               = 3,
  MAC_PIB_MIN_BE_DEFAULT                           = MAC_PIB_MIN_BE_MAX,
  MAC_PIB_PANID_DEFAULT                            = 0xFFFF,
  MAC_PIB_PROMISCUOUS_MODE_DEFAULT                 = FALSE,
  MAC_PIB_RX_ON_WHEN_IDLE_DEFAULT                  = FALSE,
  MAC_PIB_SHORT_ADDR_DEFAULT                       = 0xFFFF,
  MAC_PIB_SUPERFRAME_ORDER_MAX                     = 15, 
  MAC_PIB_SUPERFRAME_ORDER_DEFAULT                 = MAC_PIB_SUPERFRAME_ORDER_MAX,
  MAC_PIB_TRANSACTION_PERSISTENCE_TIME_DEFAULT     = 0x01F4,
};

typedef struct // MAC PIB type.
{
  uint8_t               ackWaitDuration;
  bool                  associationPermit;  
  bool                  autoRequest;
  bool                  battLifeExt;
  uint8_t               battLifeExtPeriods;
  uint8_t               beaconPayloadLength; 
  uint8_t               *beaconPayload;      
  uint8_t               beaconOrder;
  uint32_t              beaconTxTime;
  uint8_t               bsn;
  MACExtAddr            coordExtAddr;
  MACShortAddr          coordShortAddr;
  uint8_t               dsn;
  bool                  gtsPermit;
  uint8_t               maxCSMABackoffs;
  uint8_t               minBE;
  MACPANId              panId;
  bool                  promiscuousMode;
  bool                  rxOnWhenIdle;
  MACShortAddr          shortAddr;
  MACExtAddr            extAddr;
  uint8_t               superframeOrder;
  uint16_t              transactionPersistenceTime;
} PACK MACPIBType;

typedef union // MAC PIB attribute type.
{
  uint8_t               ackWaitDuration;
  bool                  associationPermit;  
  bool                  autoRequest;
  bool                  battLifeExt;
  uint8_t               battLifeExtPeriods;
  uint8_t               *beaconPayload;
  uint8_t               beaconPayloadLength;
  uint8_t               beaconOrder;
  uint32_t              beaconTxTime;
  uint8_t               bsn;
  MACExtAddr            coordExtAddr;
  MACShortAddr          coordShortAddr;
  uint8_t               dsn;
  bool                  gtsPermit;
  uint8_t               maxCSMABackoffs;
  uint8_t               minBE;
  MACPANId              panId;
  bool                  promiscuousMode;
  bool                  rxOnWhenIdle;
  MACShortAddr          shortAddr;
  MACExtAddr            extAddr;
  uint8_t               superframeOrder;
  uint16_t              transactionPersistenceTime;
} MACPIBAttr;

typedef enum // MAC address mode type.
{
  MAC_NO_ADDR_MODE    = 0x00,
  MAC_RSRV_ADDR_MODE  = 0x01,
  MAC_SHORT_ADDR_MODE = 0x02,
  MAC_EXT_ADDR_MODE   = 0x03
} MACAddrMode;

typedef union // MAC address. 
{
  MACShortAddr sh;
  MACExtAddr ext;
} MACAddr;

typedef enum // MAC TxOptions.
{
  MAC_ACK_TXOPTION      = 0x01,
  MAC_GTS_TXOPTION      = 0x02,
  MAC_INDIRECT_TXOPTION = 0x04,
  MAC_SECURE_TXOPTION   = 0x08
} MACTxOptions;

typedef enum // Diassociation reasons.
{
  MAC_COORDINATOR_WISHES_DEVICE_LEAVE_PAN_REASON = 0x01,
  MAC_DEVICE_WISHES_LEAVE_PAN_REASON             = 0x02,
} DeassociationReason;

typedef struct // Superframe specification.
{
  uint16_t beaconOrder       : 4;
  uint16_t superframeOrder   : 4;
  uint16_t finalSlot         : 4;
  uint16_t battLifeExt       : 1;
  uint16_t reserved          : 1;
  uint16_t panCoordinator    : 1;
  uint16_t associationPermit : 1;
} MACSuperframeSpecification;

typedef struct // PAN descriptor.
{
  MACAddrMode coordAddrMode;
  MACPANId coordPANId;
  MACAddr coordAddr;
  uint8_t channel;
  MACSuperframeSpecification superframeSpecification;
  bool gtsPermit;
  uint8_t quality;
  int8_t rssi;
  uint32_t timeStamp;
} PACK PANDescriptor;

typedef enum
{
  MAC_ED_SCAN      = 0x00,
  MAC_ACTIVE_SCAN  = 0x01,
  MAC_PASSIVE_SCAN = 0x02,
  MAC_ORPHAN_SCAN  = 0x03
} MACScanType;

typedef struct // Params and status of DATA request.
{
  MACAddrMode srcAddrMode;
  MACPANId srcPANId;
  MACAddr srcAddr;
  MACAddrMode dstAddrMode;
  MACPANId dstPANId;
  MACAddr dstAddr;
  uint8_t *msdu;           // Data payload buffer.
  uint8_t msduLength;      // Data payload length.
  uint8_t msduHandle;
  MACTxOptions txOptions;
  MACStatus status;
} PACK MACDataParams;

typedef struct // Params of DATA indication.
{
  MACAddrMode srcAddrMode;
  MACPANId srcPANId;
  MACAddr srcAddr;
  MACAddrMode dstAddrMode;
  MACPANId dstPANId;
  MACAddr dstAddr;
  uint8_t msduLength;     // Data payload length.
  uint8_t *msdu;          // Data payload.
  uint8_t quality;
  int8_t rssi;
} PACK MACDataIndParams;

typedef struct // Params of ASSOCIATE request.
{
  uint8_t channel;
  MACAddrMode coordAddrMode;
  MACPANId coordPANId;
  MACAddr coordAddr;
  uint8_t capability;
} PACK MACAssociateReqParams;

typedef struct // Params of ASSOCIATE confirm.
{
  MACShortAddr shortAddr;
  MACStatus status;
} PACK MACAssociateConfParams;

typedef struct // Params of ASSOCIATE indication.
{
  MACExtAddr extAddr;
  uint8_t capability;
} PACK MACAssociateIndParams;

typedef struct // Params of ASSOCIATE response.
{
  MACExtAddr extAddr;
  MACShortAddr shortAddr;
  MACStatus status;
  uint8_t *msdu;          // Buffer for outgoing frame.
} PACK MACAssociateRespParams;

typedef struct // Params of COMM_STATUS indication.
{
  MACPANId panId;
  MACAddrMode srcAddrMode;
  MACAddr srcAddr;
  MACAddrMode dstAddrMode;
  MACAddr dstAddr;
  MACStatus status;
} PACK MACCommStatusIndParams;

typedef struct // Params and status of DISASSOCIATE request.
{
  MACExtAddr extAddr;
  MACShortAddr shortAddr;
  DeassociationReason reason;
  uint8_t *msdu;              // Buffer for outgoing frame, when coordinator inits operation.
  MACStatus status;           // Status of request operation.
} PACK MACDisassociateReqParams;

typedef struct // Params of DISASSOCIATE indication.
{
  MACExtAddr extAddr;
  DeassociationReason reason;
} PACK MACDisassociateIndParams;

typedef struct // Pending address specification.
{
  uint8_t shortAddr : 3;
  uint8_t reserved  : 1;
  uint8_t extAddr   : 3;
} MACPendingAddressSpecification;

typedef struct // Params of BEACON indication.
{
  uint8_t bsn;
  PANDescriptor panDescriptor;
  MACPendingAddressSpecification pendAddrSpecification;
  MACAddr*                       pendAddrList[MAC_PENDING_ADDRESS_MAX];
  uint8_t msduLength;                            // Beacon payload length.
  uint8_t *msdu;                                 // Beacon payload.
} PACK MACBeaconIndParams;

typedef struct // Params of SET/GET requests and confirms.
{
  MACPIBId id;
  MACPIBAttr attr;
  MACStatus status;
} PACK MACPIBParam;

typedef struct // Params of ORPHAN indication.
{
  MACExtAddr extAddr;
} PACK MACOrphanIndParams;

typedef struct // Params of ORPHAN response.
{
  MACExtAddr extAddr;
  MACShortAddr shortAddr;
  bool associate;
} PACK MACOrphanRespParams;

typedef struct // Params and status of RX_ENABLE request.
{
  bool     deferPermit;
  uint32_t rxOnTime;
  uint32_t rxOnDuration;
  MACStatus status;      // 
} PACK MACRxEnableParams;

typedef union // SCAN result.
{
  PHYEnergyLevel *energy;
  PANDescriptor *panDescriptor;
} PACK MACScanResult;

typedef struct // Params of SCAN request.
{
  MACScanType type;
  uint32_t channels;
  uint8_t duration;
  uint8_t maxResultSize;     // Max number of scan results.
  MACScanResult result;
} PACK MACScanReqParams;

typedef struct // Params of SCAN confirm.
{
  MACStatus status;
  uint32_t unScannedChannels;
  uint8_t resultSize;
} PACK MACScanConfParams;

typedef struct // Params and status of START request.
{
  MACPANId panId;
  uint8_t channel;
  uint32_t startTime;
  uint8_t beaconOrder;
  uint8_t superframeOrder;
  bool    panCoordinator;
  bool    batteryLifeExt;
  bool    coordRealignment;
  MACStatus status;  
} PACK MACStartParams;

typedef struct // Params and status of POLL request.
{
  MACAddrMode coordAddrMode;
  MACPANId coordPANId;
  MACAddr  coordAddr;
  MACStatus status;  
} PACK MACPollParams;

typedef enum
{
  MAC_GTS_DIRECTION_TRANSMIT = 0,
  MAC_GTS_DIRECTION_RECEIVE  = 1
} MACGTSDirection;

typedef enum
{
  MAC_GTS_TYPE_DEALLOCATE = 0,
  MAC_GTS_TYPE_ALLOCATE   = 1
} MACGTSType;

typedef struct
{
  uint8_t    length : 4;
  uint8_t direction : 1;
  uint8_t      type : 1;
  uint8_t  reserved : 2;
} PACK MACGTSCharacteristics;

typedef struct // Params and status of GTS request.
{
  MACGTSCharacteristics characteristics;
  MACStatus status;
} PACK MACGTSReqParams;

typedef struct // Params of GTS indication.
{
  MACShortAddr shortAddr;
  MACGTSCharacteristics characteristics;
} PACK MACGTSIndParams;

typedef enum // Type of frame.
{
  MAC_BEACON_TYPE,
  MAC_DATA_TYPE,
  MAC_ASSOCIATION_RESPONSE_COMMAND_TYPE,
  MAC_DISASSOCIATION_NOTIFICATION_COMMAND_TYPE,
} MACFrameType;

typedef struct // Parameters of the frame for header and footer lengths getting.
{
  MACFrameType type; // Type of frame.
  union
  {
    MACStartParams           *beacon;
    MACDataParams            *data;
    MACAssociateRespParams   *association_response;
    MACDisassociateReqParams *disassociation_request;
  } params;
  uint8_t header; // Header length.
  uint8_t footer; // Footer length.
} PACK MACFrameParams;

enum // Some MAC frame size, included PHY header (1 byte).
{
  MAC_COORDINATOR_REALIGNMENT_COMMAND_LENGTH_MAX = 34,
  MAC_BEACON_HEADER_LENGTH_MAX                   = 96,
  MAC_DATA_HEADER_LENGTH_MAX                     = 24,
  MAC_ASSOCIATION_RESPONSE_LENGTH                = 30,
  MAC_DISASSOCIATION_REQUEST_LENGTH              = 28
};

enum // Maximum and minimum TX power values.
{
  PHY_MIN_TX_POWER     = -17,
  PHY_MAX_TX_POWER     =  3,
  PHY_DEFAULT_TX_POWER =  0,
};

#endif

// eof MAC.h
