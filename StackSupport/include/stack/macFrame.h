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
  macFrame.h
  
  Types and constants declaration of IEEE802.15.4 MAC frame.
********************************************************************************/

#ifndef _MAC_FRAME_H
#define _MAC_FRAME_H

#include "MAC.h"

enum // Indexes of fields of frame.
{
  MAC_LENGTH_FRAME       = 0, // Length.
  MAC_CONTROL_FRAME_LOW  = 1, // The least significant part of control field.
  MAC_CONTROL_FRAME_HIGH = 2, // The most significant part of control field.
  MAC_DSN_FRAME          = 3, // Data sequence number.
};

typedef struct // Frame control.
{
  uint16_t type         : 3;
  uint16_t security     : 1;
  uint16_t framePending : 1;
  uint16_t ack          : 1;
  uint16_t intraPAN     : 1;
  uint16_t reserved1    : 3;
  uint16_t dstAddrMode  : 2;
  uint16_t reserved2    : 2;
  uint16_t srcAddrMode  : 2;
} MACFrameControl;

enum
{
  MAC_ACK_LENGTH = 5, // ACK frame length, no including length field.
};

typedef enum // Code of frame.
{
  MAC_BEACON_CODE                              = 0x00,
  MAC_DATA_CODE                                = 0x01,
  MAC_ACK_CODE                                 = 0x02,
  MAC_COMMAND_CODE                             = 0x03,
  MAC_ASSOCIATION_REQUEST_COMMAND_CODE         = 0x01,
  MAC_ASSOCIATION_RESPONSE_COMMAND_CODE        = 0x02,
  MAC_DISASSOCIATION_NOTIFICATION_COMMAND_CODE = 0x03,
  MAC_DATA_REQUEST_COMMAND_CODE                = 0x04,
  MAC_PANID_CONFLICT_NOTIFICATION_COMMAND_CODE = 0x05,
  MAC_ORPHAN_NOTIFICATION_COMMAND_CODE         = 0x06,
  MAC_BEACON_REQUEST_COMMAND_CODE              = 0x07,
  MAC_COORDINATOR_REALIGNMENT_COMMAND_CODE     = 0x08,
  MAC_GTS_REQUEST_COMMAND_CODE                 = 0x09,
} MACFrameCode;

typedef enum // Type of frame.
{
  MAC_NO_FRAME,
  MAC_BEACON,
  MAC_DATA,
  MAC_ACK,
  MAC_COMMAND,
  MAC_ASSOCIATION_REQUEST_COMMAND,
  MAC_ASSOCIATION_RESPONSE_COMMAND,
  MAC_DISASSOCIATION_NOTIFICATION_COMMAND,
  MAC_DATA_REQUEST_COMMAND,
  MAC_PANID_CONFLICT_NOTIFICATION_COMMAND,
  MAC_ORPHAN_NOTIFICATION_COMMAND,
  MAC_BEACON_REQUEST_COMMAND,
  MAC_COORDINATOR_REALIGNMENT_COMMAND,
  MAC_GTS_REQUEST_COMMAND,
} MACFrame;

typedef struct // GTS specification.
{
  uint8_t count    : 3;
  uint8_t reserved : 4;
  uint8_t permit   : 1;
} MACGTSSpecification;

typedef struct // GTS description.
{
  MACShortAddr shortAddr;
  struct
  {
    uint8_t startSlot : 4;
    uint8_t length    : 4;
  };
} MACGTSDescription;

typedef struct // Description of beacon MAC payload and current channel.
{
  uint32_t                                                   timeStamp;
  uint8_t                                                      channel;
  MACSuperframeSpecification                   superframeSpecification;
  MACGTSSpecification                                 gtsSpecification;
  MACGTSDirection                                         gtsDirection;
  MACGTSDescription*                              gtsList[MAC_GTS_MAX];
  MACPendingAddressSpecification                 pendAddrSpecification;
  MACAddr*                       pendAddrList[MAC_PENDING_ADDRESS_MAX];
} MACBeaconDescription;

typedef struct // Description of associate request command MAC payload.
{
  uint8_t capability;
} MACAssociationRequestDescription;

typedef struct // Description of associate response command MAC payload.
{
  MACShortAddr shortAddr;
  uint8_t status;
} MACAssociationResponseDescription;

typedef struct // Description of diassociate notification command MAC payload.
{
  uint8_t reason;
} MACDisassociationNotificationDescription;

typedef struct // Description of coordinator realignment command MAC payload.
{
  MACPANId panId;
  MACShortAddr coordShortAddr;
  uint8_t channel;
  MACShortAddr shortAddr;
} MACCoordinatorRealignmentDescription;

typedef struct
{
  MACFrame                type;
  MACFrameControl frameControl;
  uint8_t                  dsn; // DSN.
  MACPANId            dstPANId;
  MACAddr              dstAddr;
  MACPANId            srcPANId;
  MACAddr              srcAddr;
  union
  {
    MACBeaconDescription beacon;
    MACAssociationRequestDescription associationRequest;
    MACAssociationResponseDescription associationResponse;
    MACDisassociationNotificationDescription disassociationNotification;
    MACCoordinatorRealignmentDescription coordinatorRealignment;
    MACGTSCharacteristics gtsCharacteristics;
  } payload;
  
  uint8_t               *msdu; // MSDU.
  uint8_t          msduLength; // MSDU length.
  uint8_t      quality; // Link quality of incoming frame.
  int8_t       rssi; // rssi of incoming frame.
  bool                    gts; // GTS flag. For outgoing data frames only.
} MACFrameDescription;

enum // Constants of superframe specification field of beacon frame.
{
  MAC_SFS_MASK                  = 0x000F,
  MAC_SUPERFRAME_ORDER_SFS_DISP = 0x4,
  MAC_FINAL_CAP_SLOT_SFS_DISP   = 0x8,
  MAC_BATTERY_LIFE_EXT_SFS      = 0x1000,
  MAC_PAN_COORDINATOR_SFS       = 0x4000,
  MAC_ASSOCIATION_PERMIT_SFS    = 0x8000
};

#endif

// eof macFrame.h
