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
  NWKFramesDef.h
  
  Declaration types and constants for NWK implementation.
 ********************************************************************************/
#ifndef _NWKFRAMESDEF_H_
#define _NWKFRAMESDEF_H_

#define ZIGBEE_VERSION        0x01
#define SET_ZIGBEE_VERSION_TO_FRAME(byte) {byte |= (ZIGBEE_VERSION & 0x0f) << 2;}

#define FRAME_CONTROL_SIZE    2
#define FRAME_CTL_LOWBYTE     0
#define FRAME_CTL_HIGHBYTE    1

#define FRAME_TYPE_MASK       0x3
#define DATA_FRAME            0x00
#define CMD_FRAME             0x01

#define RREQFRAME_SIZE        14
//#define RREPFRAME_SIZE        14
#define RREPFRAME_SIZE        16
#define RERRFRAME_SIZE        12
#define LEAVEFRAME_SIZE       10

#define DST_ADDR              2
#define SRC_ADDR              4
#define BROADCAST_RADIUS      6
#define BROADCAST_SEQNUMBER   7

#define NWK_HEADER            6
#define NWK_HEADER_BC         8

#define NWK_HEADER_MAX  NWK_HEADER_BC + MAC_DATA_HEADER_LENGTH_MAX

// field shifts from start of payload
#define COMMAND_CODE_SHIFT        0
#define COMMAND_OPTIONS_SHIFT     1

#define RREQID_SHIFT              2
#define RREQ_DST_SHIFT            3
#define RREQ_PATHCOST_SHIFT       5
#define RREP_ORIG_SHIFT           3
#define RREP_RESP_SHIFT           5
#define RREP_PATHCOST_SHIFT       7
#define RERR_ERRCODE_SHIFT        1
#define RERR_DSTADDR_SHIFT        2

#define CMD_RREQ                  0x01
#define CMD_RREP                  0x02
#define CMD_RERR                  0x03
#define CMD_LEAVE                 0x04
#define CMD_DATA                  0x05

#define ROUTE_REPAIR_FLAG         0x80
#define ROUTE_REPAIR_FLAG_SHIFT   7

// flags in low byte
//#define GET_DISCOVER_ROUTE_FLAG(val)       (0xc0 & val) >> 6;
//#define SET_DISCOVER_ROUTE_FLAG(ret, val)  {ret |= (val << 6) & 0xc0;}
#define GET_DISCOVER_ROUTE_FLAG(val)       (0xc0 & val) >> 6;
#define SET_DISCOVER_ROUTE_FLAG(ret, val)  {ret |= (val << 6);}

// flags in high byte
#define SECURITY_FLAG             0x02
#define SECURITY_FLAG_SHIFT       1

#define REQIND_FLAG               0x40
#define REMOVE_CHILDREN_FLAG      0x80

// 102 - 8
#define NWK_MAX_FRAME_SIZE        MAC_MAX_FRAME_SIZE - NWK_HEADER_BC

#define MACADDR_SIZE              2
#define MACPANID_SIZE             2

#define MAX_BEACON_PAYLOAD_LENGTH  3

#define GET_STACK_PROFILE(dst,src) {dst = src & 0x0f;}
#define SET_STACK_PROFILE(dst,src) {dst &= ~0x0f; dst |= src & 0x0f;}

#define GET_VERSION(dst,src) {dst = (src >> 4) & 0x0f;}
#define SET_VERSION(dst) {dst |= (ZIGBEE_VERSION & 0x0f) << 4;}

#define GET_PERMIT_END_DEV(dst,src) {dst = (src & 0x80) ? TRUE : FALSE;}
#define SET_PERMIT_END_DEV(dst,isset) {if(isset) dst |= 0x80; else dst &= ~0x80;}

#define GET_PERMIT_ROUTER(dst,src) {dst = (src & 0x04) ? TRUE : FALSE;}
#define SET_PERMIT_ROUTER(dst,isset) {if(isset) dst |= 0x04; else dst &= ~0x04;}

#define GET_DEPTH(dst,src) {dst = (src >> 3) & 0x0f;}
#define SET_DEPTH(dst,src) {dst &= ~(0x0f << 3); dst |= (src & 0x0f) << 3;}

//Security parameters
#ifdef _SECURITY
//Auxiliary header fields
#define NWK_AUX_HDR_LEN 14   // NWK auxialry header length
#define SEC_CONTROL 0        // Security control byte
#define SEC_FRAME_COUNTER 1  // Frame counter field
#define SEC_SOURCE_ADDRESS 5 // Device source address
#define SEC_KEY_SEQ_NUM 13   // Key sequence number
#define SEC_NWK_KEY_ID 0x01  // A Network key
#define SEC_KEY_ID_SHIFT 3   // key id shift
#define SEC_NONCE_NWK 0x01   // Extended nonce for NWK
#define SEC_NONCE_SHIFT 5    // Extended nonce shift
#define SEC_LEVEL_MASK 0x07  // Security level mask
#endif

#endif

// eof NWKFramesDef.h
