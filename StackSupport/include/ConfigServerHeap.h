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
  ConfigServerHeap.h
  
  This module contains ConfigServer global memory and parameters allocations
********************************************************************************/

#ifndef _CONFIG_SERVER_HEAP_H_
#define _CONFIG_SERVER_HEAP_H_

/************************************************************/
// ZDO variables
/************************************************************/
  
  uint8_t  ZDO_scanDuration = ZDO_SCAN_DURATION          ; // scan duration
  uint8_t  ZDO_stackProfile = ZDO_STACK_PROFILE          ; // stack profile

  #ifdef _CHILDREN_EXIST_
    ZDOChildRec_t childTable[NWK_MAX_CHILD_NEIB_NUMBER];
    uint32_t  ZDO_childLifetimePrecision = ZDO_CHILD_LIFETIME_PRECISION;
    uint32_t  ZDO_childDefaultLifetime = ZDO_CHILD_DEFAULT_LIFETIME;
    uint32_t  ZDO_defaultDataPeriod = ZDO_DEFAULT_DATA_PERIOD; 
  #else
     ZDOChildRec_t* childTable = NULL;
  #endif

  #ifdef _PARENT_EXIST_    
    NWKNetworkDescr_t networkList[MAX_PAN_DESCRIPTORS];
    uint8_t  ZDO_channelDiscoveryAttempts = ZDO_CHANNEL_DISC_ATTEMPTS ; // number of attempts to scan each channel
  #endif
  
  //bufZDO must be strictly syncronized with ZDO_BUFFER_ID !
  void * bufZDO[] = 
  {
    #ifdef _PARENT_EXIST_    
      (void*) networkList,
    #else
      NULL,
    #endif
    
    #ifdef _CHILDREN_EXIST_
      (void*) childTable
    #else
      NULL,
    #endif
    
  };

  //paramZDO must be strictly syncronized with ZDO_PARAM_ID !
  void * paramZDO[] = 
  {
	&ZDO_scanDuration, 
	&ZDO_stackProfile, 
    
    #ifdef _PARENT_EXIST_    
  	  &ZDO_channelDiscoveryAttempts,
    #else
      NULL,
    #endif
    
    #ifdef _CHILDREN_EXIST_
      &ZDO_childLifetimePrecision,
      &ZDO_childDefaultLifetime,
	  &ZDO_defaultDataPeriod
    #else
      NULL,
      NULL,
      NULL,
    #endif
  };

/************************************************************/
// NWK variables
/************************************************************/
  NWKNeighborTable_t NWK_NT[NWK_MAX_NEIB_NUMBER];
  uint8_t NWK_nwkMaxNeighbors =  NWK_MAX_NEIB_NUMBER;


#ifdef _CHILDREN_EXIST_
  NWKChildNeighborTable_t NWK_CNT[NWK_MAX_CHILD_NEIB_NUMBER];
  uint8_t NWK_nwkMaxChildren =  NWK_MAX_CHILD_NEIB_NUMBER;

  NWKIT_t NWK_ITT[NWK_MAX_ITT];
  uint8_t NWK_nwkMaxIT = NWK_MAX_ITT;
#else
  #define NWK_CNT NULL
  uint8_t NWK_nwkMaxChildren =  0;

  #define NWK_ITT NULL
  uint8_t NWK_nwkMaxIT = 0;
#endif  

  NWKBTT_t NWK_BTT[NWK_MAX_BTT];  // Broadcast Transaction Table
  uint8_t NWK_nwkMaxBroadcastTransactions = NWK_MAX_BTT;

#ifdef _WITH_MESH_
  NWKRT_t NWK_RT[NWK_MAX_RT];     // Routing Table
  uint8_t NWK_nwkRouteTable = NWK_MAX_RT;

  NWKRDT_t NWK_RDT[NWK_MAX_RDT];  // Route Discovery Table
  uint8_t NWK_nwkMaxRouteDiscoveryTable = NWK_MAX_RDT;
#else
  #define NWK_RT NULL
  uint8_t NWK_nwkRouteTable = 0;
  #define NWK_RDT NULL
  uint8_t NWK_nwkMaxRouteDiscoveryTable = 0;
#endif
  
  PANDescriptor NWK_PANDSCR[MAX_PAN_DESCRIPTORS];
  uint8_t NWK_nwkMaxPANDescriptors = MAX_PAN_DESCRIPTORS;
  
  uint8_t NWK_nwkMaxRouters = NWK_MAX_ROUTER_NEIB_NUMBER;
  uint8_t NWK_nwkMaxDepth = NWK_MAX_DEPTH;

  MACExtAddr NWK_ownExtAddress = 0;

/************************************************************/
// NWK parameters
/************************************************************/

  //paramNWK must be strictly syncronized with NWK_PARAM_ID !
  void * paramNWK[] =
  {
    &NWK_nwkMaxNeighbors,
    &NWK_nwkMaxChildren,
    &NWK_nwkMaxRouters,
    &NWK_nwkMaxDepth,
    &NWK_nwkMaxBroadcastTransactions,
    &NWK_nwkRouteTable,
    &NWK_nwkMaxRouteDiscoveryTable,
    &NWK_nwkMaxPANDescriptors,
    &NWK_ownExtAddress,
    &NWK_nwkMaxIT
  };

/************************************************************/
// NWK buffers
/************************************************************/

  //bufNWK must be strictly syncronized with NWK_BUFFER_ID !
  void * bufNWK[] =
  {
    (void*) NWK_NT,
    (void*) NWK_CNT,
    (void*) NWK_BTT,
    (void*) NWK_RT,
    (void*) NWK_RDT,
    (void*) NWK_PANDSCR,
    (void*) NWK_ITT

  };

/************************************************************/
// APS variables
/************************************************************/

  APSUserDevice_t   APS_userdevice[APS_NUM_USERS];
  APSTxPool_t       APS_txpool[APS_TXPOOLSIZE];
  APSRxPool_t       APS_rxpool[APS_RXPOOLSIZE];
  APSNackPool_t     APS_nackpool[APS_NACKPOOLSIZE];
  uint8_t           APS_rxBuf[APS_RXPOOLSIZE * APS_RXBUFFER_LENGTH];
  uint8_t           APS_num_users = APS_NUM_USERS;
  uint8_t           APS_txpool_size = APS_TXPOOLSIZE;
  uint8_t           APS_rxpool_size = APS_RXPOOLSIZE;
  uint8_t           APS_nackpool_size = APS_NACKPOOLSIZE;
  
  uint8_t           APS_nlde_req_atts = APS_NLDE_REQ_ATTS;
  uint8_t           APS_rxbuf_len = APS_RXBUFFER_LENGTH;

/************************************************************/
// APS parameters
/************************************************************/
  // paramAPS must be strictly syncronized with APS_PARAM_ID  !
  void * paramAPS[] =
  {
    &APS_num_users,
    &APS_txpool_size,
    &APS_rxpool_size,
    &APS_nackpool_size,
    &APS_nlde_req_atts,
    &APS_rxbuf_len
  };

/************************************************************/
// APS buffers
/************************************************************/
  // bufAPS must be strictly syncronized with APS_BUFFER_ID !
  void * bufAPS[] =
  {
    (void*) APS_userdevice,
    (void*) APS_txpool,
    (void*) APS_rxpool,
    (void*) APS_nackpool,
    (void*) APS_rxBuf
  };

/************************************************************/
//
/************************************************************/
  void * bufPHY[] = {};
  void * bufMAC[] = {};
  void * paramPHY[] = {};
  void * paramMAC[] = {};

#ifndef _NO_FRAMEWORK_
/************************************************************/
// FW variables
/************************************************************/

  FW_EPEntry_t FW_epTable[FW_MAX_USER_ENDPOINTS]; // table containing information about endpoints registered by user
  uint8_t      FW_max_user_endpoints = FW_MAX_USER_ENDPOINTS;

/************************************************************/
// FW parameters
/************************************************************/
  // paramFW must be strictly syncronized with FW_PARAM_ID  !
  void * paramFW[] =
  {
    &FW_max_user_endpoints,
  };

/************************************************************/
// FW buffers
/************************************************************/
  // bufFW must be strictly syncronized with FW_BUFFER_ID !
  void * bufFW[] =
  {
    (void*) FW_epTable,
  };
#endif

/************************************************************/
// Global
/************************************************************/

  //moduleBuf and moduleParam must be strictly syncronized with MODULE_ID !
  void * moduleBuf[] = 
  {
    (void*) bufNWK,
    (void*) bufZDO,
    (void*) bufAPS,
    (void*) bufPHY,
    (void*) bufMAC,
    (void*) NULL,
    #ifndef _NO_FRAMEWORK_
      (void*) bufFW,
    #else
      (void*) NULL,
    #endif
  };

  //moduleBuf and moduleParam must be strictly syncronized by MODULE_ID !
  void * moduleParam[] = 
  {
    (void*) paramNWK,
    (void*) paramZDO,
    (void*) paramAPS,
    (void*) paramPHY,
    (void*) paramMAC,
    (void*) NULL,
    #ifndef _NO_FRAMEWORK_
      (void*) paramFW,
    #else
      (void*) NULL,
    #endif
  };

#endif //_CONFIG_SERVER_HEAP_H_
//eof
