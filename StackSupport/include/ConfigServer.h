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
  ConfigServer.h

  Parameters and memory blocks IDs definitions for ConfigServer
********************************************************************************/

#ifndef _CONFIG_SERVER_H_
#define _CONFIG_SERVER_H_

/************************************************************/
//  ConfigServer functions declaration.
/************************************************************/

/*******************************************************************************
  Description: get buffer from ConfigServer

  Parameters: moduleIndex - index of the requesting module,
              paramIndex  - index of the requested buffer.

  Returns: pointer to the buffer, must be cast to (void *).
*******************************************************************************/
/* (void*) */ int CServ__ConfigServer__getBuf(uint8_t moduleIndex, uint8_t paramIndex);

/*******************************************************************************
  Description: get parameter from ConfigServer

  Parameters: moduleIndex - index of the requesting module,
              paramIndex  - index of the requested parameter.

  Returns: pointer to the parameter, must be cast to appropraite pointer type.
*******************************************************************************/
/* (void*) */ int CServ__ConfigServer__getParam(uint8_t moduleIndex, uint8_t paramIndex);

/************************************************************/
//  Declaration types for ConfigServer implementation.
/************************************************************/

  typedef enum
  {
    ZDO_NETWORK_LIST_ID_BUFFER_INDEX                    = 0x00,
    ZDO_CHILD_TABLE_ID_BUFFER_INDEX                     = 0x01
  } ZDO_BUFFER_ID;

  typedef enum
  {
    ZDO_PARAM_ID_SCAN_DURATION_INDEX               = 0x00,
    ZDO_PARAM_ID_STACK_PROFILE_INDEX               = 0x01,
    ZDO_PARAM_ID_CHANNEL_DISCOVERY_ATTEMPTS_INDEX  = 0x02,
    ZDO_PARAM_ID_CHILD_LIFETIME_PRECISION_INDEX    = 0x03,
    ZDO_PARAM_ID_CHILD_DEFAULT_LIFETIME_INDEX      = 0x04,
//    ZDO_PARAM_ID_PARENT_SYNC_TIME_PRECISION_INDEX  = 0x05, //!!B
    ZDO_PARAM_ID_DEFAULT_DATA_PERIOD_INDEX         = 0x05,
  } ZDO_PARAM_ID;


/************************************************************/
// NWK parameters
/************************************************************/
  typedef enum
  {
    NWK_PARAM_ID_NWK_MAX_NEIB_NUMBER                  = 0x00,
    NWK_PARAM_ID_NWK_MAX_CHILD_NEIB_NUMBER            = 0x01,
    NWK_PARAM_ID_NWK_MAX_ROUTER_NEIB_NUMBER           = 0x02,
    NWK_PARAM_ID_NWK_MAX_DEPTH                        = 0x03,
    NWK_PARAM_ID_NWK_MAX_BTT                          = 0x04,
    NWK_PARAM_ID_NWK_MAX_RT                           = 0x05,
    NWK_PARAM_ID_NWK_MAX_RDT                          = 0x06,
    NWK_PARAM_ID_NWK_MAX_PAN_DESCR                    = 0x07,
    NWK_PARAM_ID_OWN_EXT_ADDRESS                      = 0x08,
    NWK_PARAM_ID_NWK_MAX_ITT                          = 0x09
  } NWK_PARAM_ID;

/************************************************************/
// NWK buffers
/************************************************************/
  typedef enum
  {
    NWK_BUFFER_ID_NEIGHBOR_TABLE                      = 0x00,
    NWK_BUFFER_ID_CHILD_NEIGHBOR_TABLE                = 0x01,
    NWK_BUFFER_ID_BTT                                 = 0x02,
    NWK_BUFFER_ID_RT                                  = 0x03,
    NWK_BUFFER_ID_RDT                                 = 0x04,
    NWK_BUFFER_ID_NWK_PAN_DESCR                       = 0x05,
    NWK_BUFFER_ID_ITT                                 = 0x06
  } NWK_BUFFER_ID;

/************************************************************/
// APS parameters
/************************************************************/
  typedef enum
  {
    APS_PARAM_ID_NUM_USERS            = 0x00,
    APS_PARAM_ID_TXPOOLSIZE           = 0x01,
    APS_PARAM_ID_RXPOOLSIZE           = 0x02,
    APS_PARAM_ID_NACKPOOLSIZE         = 0x03,
    APS_PARAM_ID_REQ_ATTS             = 0x04,
    APS_PARAM_ID_RXBUFFER_LENGTH      = 0x05
  } APS_PARAM_ID;

/************************************************************/
// APS buffers
/************************************************************/
  typedef enum
  {
    APS_BUFFER_ID_USERPARAM           = 0x00,
    APS_BUFFER_ID_TXPOOL              = 0x01,
    APS_BUFFER_ID_RXPOOL              = 0x02,
    APS_BUFFER_ID_NACKPOOL            = 0x03,
    APS_BUFFER_ID_RXBUF               = 0x04
  } APS_BUFFER_ID;

/************************************************************/
// FW parameters
/************************************************************/
  typedef enum
  {
    FW_PARAM_ID_MAX_USER_ENDPOINTS    = 0x00
  } FW_PARAM_ID;

/************************************************************/
// FW buffers
/************************************************************/
  typedef enum
  {
    FW_BUFFER_ID_ENDPOINTS_TABLE     = 0x00
  } FW_BUFFER_ID;
  
/************************************************************/
// Modules enumerations
/************************************************************/
  typedef enum
  {
    MODULE_ID_NWK       = 0x00,
    MODULE_ID_ZDO       = 0x01,
    MODULE_ID_APS       = 0x02,
    MODULE_ID_PHY       = 0x03,
    MODULE_ID_MAC       = 0x04,
    MODULE_ID_APL       = 0x05,
    MODULE_ID_FW        = 0x06
  } MODULE_ID;

#define CORETEST int
#endif //_CONFIG_SERVER_H_


