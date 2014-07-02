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
  Config.h
  
  eZeeNet profile and ZigBee stack user defined parameters
********************************************************************************/
 
#ifndef _CONFIG_H_
#define _CONFIG_H_

/************************************************************/
//  Declaration types and constants for IEEE802.15.4 MAC implementation.
/************************************************************/
// Number of beacons that can be saved while scanning
#ifndef MAX_PAN_DESCRIPTORS
#define MAX_PAN_DESCRIPTORS         5
#endif

// Number of the End Devices among all children
#ifndef NWK_MAX_END_DEVICES
#define NWK_MAX_END_DEVICES         5
#endif

// Number of the Roters among all children
#ifndef NWK_MAX_ROUTER_NEIB_NUMBER
#define NWK_MAX_ROUTER_NEIB_NUMBER  5
#endif

// The total number of the children (routers + end devices),
#ifndef NWK_MAX_CHILD_NEIB_NUMBER
#define NWK_MAX_CHILD_NEIB_NUMBER   (NWK_MAX_END_DEVICES + NWK_MAX_ROUTER_NEIB_NUMBER)
#endif

// The maximum depth of the tree
#ifndef NWK_MAX_DEPTH
#define NWK_MAX_DEPTH               5
#endif

// Number of the Neighbours Table entries. The entries number should be more or equal than MAX_PAN_DESCRIPTORS
#define NWK_MAX_NEIB_NUMBER        (MAX_PAN_DESCRIPTORS + 0)

// Number of the Broadcasts Table entries (should be enougth for saving passive acknowledgment).
#define NWK_MAX_BTT                 0 

// Number of the Routes Table entries (should be equal 0 if no Mesh)
#ifndef NWK_MAX_RT
#define NWK_MAX_RT                  0
#endif

// Number of the Routes Discovery table entries (should be equal 0 if no Mesh)
#ifndef NWK_MAX_RDT
#define NWK_MAX_RDT                 0
#endif

// Max number of indirect transaction in NWK-skbuff
#ifndef NWK_MAX_ITT
#define NWK_MAX_ITT                 6
#endif


/************************************************************/
// ZDO default parameters
/************************************************************/
// Duration of scanning every channel in ZigBee defined scan duration terms
#ifndef ZDO_SCAN_DURATION
#define ZDO_SCAN_DURATION           3
#endif

// Stack profile identifier used in beacons (range 0x00 - 0x0F)
#ifndef ZDO_STACK_PROFILE
#define ZDO_STACK_PROFILE           11
#endif

// Attempts to scan every channel when searching for own network
#ifndef ZDO_CHANNEL_DISC_ATTEMPTS
#define ZDO_CHANNEL_DISC_ATTEMPTS   4
#endif

// Precision of counting children node lifetime (in milliseconds)
#ifndef ZDO_CHILD_LIFETIME_PRECISION
#define ZDO_CHILD_LIFETIME_PRECISION  100
#endif

// Default children node lifetime, assumed after node join (in milliseconds)
#ifndef ZDO_CHILD_DEFAULT_LIFETIME
#define ZDO_CHILD_DEFAULT_LIFETIME   5000    /* Must be multiple of ZDO_CHILD_LIFETIME_PRECISION !!!*/
#endif

// Defalut children node data sending period, assumed after node join (in milliseconds)
#ifndef ZDO_DEFAULT_DATA_PERIOD
#define ZDO_DEFAULT_DATA_PERIOD      1000 
#endif

/************************************************************/
// APS default parameters
/************************************************************/

// Max number of endpoints registered on APS
#ifndef APS_NUM_USERS
#define APS_NUM_USERS       10
#endif

// Max number of transmit buffers APS can simultaneously process
#ifndef APS_TXPOOLSIZE
#define APS_TXPOOLSIZE      3
#endif

// Number of receive buffers on APS
#ifndef APS_RXPOOLSIZE
#define APS_RXPOOLSIZE      3
#endif

// Number of buffers for NACK frames (sent if receive buffers overflow occurs on APS)
#ifndef APS_NACKPOOLSIZE
#define APS_NACKPOOLSIZE    3
#endif

// Number of attempts to call NLDE_DATA.request before reject if NWK busy
#ifndef APS_NLDE_REQ_ATTS
#define APS_NLDE_REQ_ATTS   10
#endif

// Max APS payload length to receive
#ifndef APS_RXBUFFER_LENGTH
#define APS_RXBUFFER_LENGTH 88
#endif

/************************************************************/
// FW default parameters
/************************************************************/

// Max number of user endpoints registered on FW = (Max APS endpoints) - (1 FW control ep) - (1 ZDO ep)
#ifndef FW_MAX_USER_ENDPOINTS
#define FW_MAX_USER_ENDPOINTS (APS_NUM_USERS - 2)
#endif

/************************************************************/
#endif //_CONFIG_H_
//eof


