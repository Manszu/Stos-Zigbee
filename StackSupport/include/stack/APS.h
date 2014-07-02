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
  APS.h
  
  APS layer header file.
********************************************************************************/

#ifndef _APS_H_
#define _APS_H_

#include "stack.h"
#include "Config.h"

/********************************************************************************
* APS frames structure
********************************************************************************/

/* APS header struct */
typedef uint8_t  APS_frameControl;
typedef uint8_t  APS_dstEndpoint;
typedef uint8_t  APS_clusterID;
typedef uint16_t APS_profileID;
typedef uint8_t  APS_srcEndpoint;

#define APS_HEADER_SIZE (sizeof(APS_frameControl) + sizeof(APS_dstEndpoint) + \
	                 sizeof(APS_clusterID) + sizeof(APS_profileID) + \
	                 sizeof(APS_srcEndpoint))
#define APSDef_footer 0

/* Masks for APS frame control field */
//Frame type field
#define APS_FRAME_TYPE_MASK 3
#define APS_DATA_FRAME      0 /* data frame */
#define APS_CMD_FRAME       1 /* APS command frame */
#define APS_ACK_FRAME       2 /* APS ACK frame */
#define APS_NACK_FRAME      3 /* APS NACK frame */
//Delivery mode
#define APS_DELIVERY_MODE_MASK     0x0C  
#define APS_UNICAST                0x00
#define APS_INDIRECT               0x04
#define APS_BROADCAST              0x08
#define APS_DELIVERY_MODE_RESERVED 0x0C
//Bits
#define APS_INDIRECT_MODE_MASK 0x10   /* indirect addresing mode specific flag*/
#define APS_SECURITY_MASK      0x20   /* secured frame */  
#define APS_ACK_MASK           0x40   /* frame needs ack */


/********************************************************************************
* Internal APS types
********************************************************************************/
#define APS_ACK_TIMEOUT_BASE      50    // timing accuracy for APS ACK waiting, ms
#define APS_DEFAULT_DATA_RETRIES  3     // default attempts to repeat data send if
                                        // no APS ACK received within ACK timeout

#define APS_SYNC_PERIOD         40      // SYNC period when APS is waiting for ACK, ms
#define APS_SYNC_ATTEMPTS       5       // attempts to do SYNC when SYNC_FAILURE status obtained
#define APS_SYNC_REPEAT_TIMEOUT 15      // SYNC repeat timeout when SYNC_FAILURE status obtained, ms
#define APS_SYNC_BURST_FLAG     FALSE   // if TRUE: When APS is waiting for ACK, it repeats every
                                        // SYNC immediately until NO DATA status comes.

typedef enum 
{
  TXSTATE_FREE              = 0,
  TXSTATE_REQUESTED         = 1,
  TXSTATE_TO_BE_CONFIRMED   = 2,
  TXSTATE_NWK_REQ_SUCCEEDED = 3,
  TXSTATE_WAITING_FOR_ACK   = 4,
  TXSTATE_ACK_EXPIRED       = 5,
  TXSTATE_READY_FOR_REQUEST = 6,
  TXSTATE_ED_CONFIRM_AT_SYNC_CONF = 7,
} TxBufState_t;

typedef enum 
{
  RXSTATE_FREE              = 10,
  RXSTATE_RECEIVED          = 11,
  RXSTATE_INDICATED         = 12,
  RXSTATE_ACK_REQ_SUCCEEDED = 13,
  RXSTATE_SEND_ACK          = 14,
  RXSTATE_PENDING           = 15
} RxBufState_t;	

typedef enum 
{ 
  NACKSTATE_REQUEST         = 20,   
  NACKSTATE_FREE            = 21,      
  NACKSTATE_REQ_SUCCEEDED   = 23
} NackBufState_t;     

typedef enum
{
  APS_NLDEREQ_TYPE_DATA = 0x00,
  APS_NLDEREQ_TYPE_ACK  = 0x01,
  APS_NLDEREQ_TYPE_NACK = 0x02
} APSNLDEReqType_t;

/********************************************************************************
* APS layer structures used to allocate external memery pools.
********************************************************************************/

typedef struct 
{
  Sbuf_t          bufDesc;
  APSIndication_t indDesc;
  RxBufState_t    state;
  uint8_t         ackBuf[APS_HEADER_SIZE];
  uint8_t         nwkReqCounter;
  uint8_t         attemptsCounter;
  EndPoint_t      targetEP;
  uint8_t         targetSeqNum;
} APSRxPool_t;

typedef struct  
{
  uint8_t        buf[APS_HEADER_SIZE];
  NackBufState_t state;
  uint8_t        nwkReqCounter;
  uint8_t        attemptsCounter;
  NWKAddr_t      dstAddr;
} APSNackPool_t;

typedef struct 
{
  APSRequest_t *   reqDesc;
  void             (*conf)(APSRequest_t *);
  TxBufState_t     state;
  Handle_t         deviceHandle;
  uint8_t          nwkReqCounter;
  uint8_t          attemptsCounter;
  uint8_t          timeoutCounter;
} APSTxPool_t;

typedef struct  
{
  EndPoint_t          endpoint;
  bool                registered;
  bool                rx_enable;
  result_t            (*ind)(APSIndication_t *);
  uint8_t             seqNum;
  uint8_t             pendingFrames;
} APSUserDevice_t;


#endif /* _APS_H_ */

// eof APS.h
