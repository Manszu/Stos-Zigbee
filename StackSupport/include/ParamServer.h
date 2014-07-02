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
  ParamServer.h
  
  ParamServer functions declarations, global stack parameters IDs.
********************************************************************************/

#ifndef PARAM_SERVER_H_
#define PARAM_SERVER_H_

#include "ConfigServer.h"

#define PARAM_SERVER_EEPROM_LENGTH EEPROM_DATA_MEMORY_SIZE / 2 //2048

enum // Module IDs.
{
  ZDO_PS_MODULE_ID = MODULE_ID_ZDO,
  APS_PS_MODULE_ID = MODULE_ID_APS,
  NWK_PS_MODULE_ID = MODULE_ID_NWK,
  MAC_PS_MODULE_ID = MODULE_ID_MAC,
  PHY_PS_MODULE_ID = MODULE_ID_PHY,
  APL_PS_MODULE_ID = MODULE_ID_APL,
  FW_PS_MODULE_ID  = MODULE_ID_FW,
};

enum // Parameter IDs.
{
  // NWK parameters.
  NWK_PS_PASSIVE_ACK_TIMEOUT_ID     = 0x82,
  NWK_PS_BROADCAST_RETRIES_ID       = 0x83,
  NWK_PS_REPORT_CONSTANT_COST_ID    = 0x89,
  NWK_PS_ROUTE_DISCOVERY_RETRIES_ID = 0x8A,
  NWK_PS_SYM_LINK_ID                = 0x8E,
  NWK_PS_USE_TREE_ROUTING_ID        = 0x91,

  // MAC parameters.
  MAC_PS_BATT_LIFE_EXT_ID         = 0x43,
  MAC_PS_BATT_LIFE_EXT_PERIOD_ID  = 0x44,
  MAC_PS_MAX_CSMA_BACKOFFS_ID     = 0x4E,
  MAC_PS_MIN_BE_ID                = 0x4F,

  // PHY parameters.
  PHY_PS_TRANSMIT_POWER_ID  = 0x02,
  PHY_PS_CCA_MODE_ID        = 0x03,
  PHY_PS_LQI_TYPE_ID        = 0x04,

  // APS parameters.
  APS_PS_MAX_FRAME_RETRIES_ID = 0xC2,
  APS_PS_ACK_WAIT_DURATION_ID = 0xC3,

  // FrameWork parameters.
  FW_PS_NODE_ROLE_ID          = 0x00,
  FW_PS_NODE_LOGICAL_ADDR_ID  = 0x01,
  FW_PS_NODE_MAC_ADDR_ID      = 0x02,
  FW_PS_PANID_ID              = 0x03,
  FW_PS_CHANNEL_MASK_ID       = 0x04,
  FW_PS_AUTO_NETWORK_ID       = 0x05,
  FW_PS_POWER_DURATION_ID     = 0x06,
  FW_PS_SYNC_PERIOD_ID        = 0x07,
  FW_PS_SECURITY_KEY_ID       = 0x08,
  // For frameWork correct processing all GPIO IDs should be
  // counted one after another  without interrupting
  // by other IDs. FW_PS_GPIO_0_ID should be the first.
  // Start GPIO IDs
  FW_PS_GPIO_0_ID             = 0x09,
  FW_PS_GPIO_1_ID             = 0x0A,
  FW_PS_GPIO_2_ID             = 0x0B,
  FW_PS_GPIO_3_ID             = 0x0C,
  FW_PS_GPIO_4_ID             = 0x0D,
  FW_PS_GPIO_5_ID             = 0x0E,
  FW_PS_GPIO_6_ID             = 0x0F,
  FW_PS_GPIO_7_ID             = 0x10,
  FW_PS_GPIO_8_ID             = 0x11,
  FW_PS_GPIO_I2C_CLK_ID       = 0x12,
  FW_PS_GPIO_I2C_DATA_ID      = 0x13,
  FW_PS_GPIO_UART_TXD_ID      = 0x14,
  FW_PS_GPIO_UART_RTS_ID      = 0x15,
  FW_PS_GPIO_UART_CTS_ID      = 0x16,
  FW_PS_GPIO_ADC_INPUT_3_ID   = 0x17,
  FW_PS_GPIO_ADC_INPUT_2_ID   = 0x18,
  FW_PS_GPIO_ADC_INPUT_1_ID   = 0x19,
  FW_PS_GPIO_BAT_ID           = 0x1A,
  FW_PS_GPIO_1WR_ID           = 0x1B,
  FW_PS_GPIO_UART_DTR_ID      = 0x1C,
  FW_PS_GPIO_USART0_RXD_ID    = 0x1D,
  FW_PS_GPIO_USART0_TXD_ID    = 0x1E,
  FW_PS_GPIO_USART0_EXTCLK_ID = 0x1F,
  FW_PS_GPIO_IRQ7_ID          = 0x20,
  FW_PS_GPIO_IRQ6_ID          = 0x21,
  // Stop GPIO IDs
  FW_PS_UART_DTR_ID           = 0x22,
  FW_PS_UART_SPEED_ID         = 0x23,
  FW_PS_UART_FC_ID            = 0x24,
  FW_PS_ADC_CONFIG_ID         = 0x25,
  FW_PS_I2C_CONFIG_ID         = 0x26,
  FW_PS_USART_CONFIG_ID       = 0x27,
  FW_PS_CALIBR_PERIOD_ID      = 0x28,
  FW_PS_CLEAN_EEP_ID          = 0x29,
};

typedef enum
{
  PARAMSERV_S_SUCCESS,
  PARAMSERV_S_UNKNOWN_PARAM,
  PARAMSERV_S_INVALID_LENGTH,
  PARAMSERV_S_DATABASE_FULL,
  PARAMSERV_S_INVALID_FCS,
  PARAMSERV_S_EEPROM_FAIL,
} ParamServerStatus_t;

/*******************************************************************************
  Description: ParamServer initialization.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
extern result_t PServ__ParamServer__init();

/*******************************************************************************
  Description: get parameter.

  Parameters: moduleId  - ID of module.
              paramId   - ID of parameter.
              length    - length of parameter.
              data      - pointer to memory area where function
                          writes value of parameter.
              getDone   - pointer to callback function that
                          indicates end of operation.

  Returns: FAIL if ParamServer busy, pointers to data or getDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
extern result_t PServ__ParamServer__get(uint8_t moduleId,
                                        uint8_t paramId,
                                        uint16_t length,
                                        void* data,
                                        void (*getDone)(ParamServerStatus_t status));

/*******************************************************************************
  Description: set parameter.

  Parameters: moduleId  - ID of module.
              paramId   - ID of parameter.
              length    - length of parameter.
              data      - pointer to memory area from function
                          writes value of parameter.
              setDone   - pointer to callback function that
                          indicates end of operation.

  Returns: FAIL if ParamServer busy, pointers to data or setDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
extern result_t PServ__ParamServer__set(uint8_t moduleId,
                                        uint8_t paramId,
                                        uint16_t length,
                                        void* data,
                                        void (*setDone)(ParamServerStatus_t status));

/*******************************************************************************
  Description: check FCS

  Parameters: checkDone  - pointer to callback function that
                           indicates end of operation.

  Returns: FAIL if ParamServer busy or checkDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
extern result_t PServ__ParamServer__checkFCS(void (*checkDone)(ParamServerStatus_t status));

/*******************************************************************************
  Description: update FCS.

  Parameters: updateDone  - pointer to callback function that
                            indicates end of operation.

  Returns: FAIL if ParamServer busy or updateDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
extern result_t PServ__ParamServer__updateFCS(void (*updateDone)(ParamServerStatus_t status));

/*******************************************************************************
  Description: erase parameter.

  Parameters: eraseDone  - pointer to callback function that
                           indicates end of operation.

  Returns: FAIL if ParamServer busy or updateDone equal NULL,
           SUCCESS othewise.
*******************************************************************************/
extern result_t PServ__ParamServer__erase(void (*eraseDone)(ParamServerStatus_t status));

#endif  /* PARAM_SERVER_H_ */

// eof ParamServer.h
