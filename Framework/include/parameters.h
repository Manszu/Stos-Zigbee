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
 
/*******************************************************************************
**  parameters.h
**  
**  eZeeNet framework: parameters header file
*******************************************************************************/
#ifndef _PARAMETERS_H_DEFINED_
#define _PARAMETERS_H_DEFINED_

#include "stack.h"
#include "frameint.h"
#include "framework.h" // Necessary for some types (eq FW_PowerDuration_t).

// Default parameters' values.
enum
{
  FW_NODE_ROLE_PARAM_DEFAULT         =  ZIGBEE_ROUTER_TYPE,   // Node role.
  FW_NODE_LOGICAL_ADDR_PARAM_DEFAULT =                   0,   // Logical address.
  FW_MAC_ADDR_PARAM_ID_DEFAULT       =                   0,   // MAC address.
  FW_COORD_PANID_PARAM_DEFAULT       =              0xFFFE,   // PAN ID for coordinator.
  FW_RED_PANID_PARAM_DEFAULT         =              0xFFFF,   // PAN ID for router and end-device.
  FW_ACTUAL_PANID_PARAM_DEFAULT      =              0xFFFF,   // Actual PAN ID for all (when not in network).
  FW_CHANNEL_MASK_PARAM_DEFAULT      =        0x00000800UL,   // Channels mask.
  FW_TX_POWER_PARAM_DEFAULT          =                   0,   // Transmission power.
  FW_AUTO_NETWORK_PARAM_DEFAULT      =                   0,   // Automatic joining is disabled.
  FW_SLEEP_DURATION_PARAM_DEFAULT    =                10ul,   // Sleep interval for end-device.
  FW_ACTIVE_DURATION_PARAM_DEFAULT   =                10ul,   // Active interval for end-device.
  FW_SYNC_PERIOD_PARAM_DEFAULT       =                  10,   // Tracking period for router (in 100us units).
// Default value of security key is                      0    // Security key.
  FW_DATA_DELIVERY_TIMEOUT_DEFAULT   =                1000,   // Timeout for data delivery.
  FW_DATA_RETRY_MAX_DEFAULT          =                   3,   // Maximum value of data retransmissions.
  FW_GPIO_CONFIG_STATE_DEFAULT       =              GPIO_Z,   // GPIO config state.
  FW_UART_DTR_DEFAULT                =               FALSE,   // UART DTR state.
  FW_UART_SPEED_DEFAULT              = UART_BAUD_RATE_38400,  // UART speed.
  FW_UART_FC_DEFAULT                 =               FALSE,   // UART flow control.
  FW_ADC_CONFIG_DEFAULT              =                   0,   // ADC configuration.
  FW_I2C_CONFIG_DEFAULT              =   I2C_CLOCK_RATE_62,   // I2C configuration.
  FW_USART_STATE_DEFAULT             =   FW_USART_MODE_OFF,   // USART configuration.
  FW_CALIBR_PERIOD_DEFAULT           =                  60,   // Calibration period.
};

enum 
{
  NODE_NOT_IN_NETWORK_CHANNEL = 0xFF,       // This defines channel parameter value if node not in netwok.
  SUPPORTED_CHANNELS          = 0x07fff800, // Mask which defines supported channels.
  DATA_DELIVERY_MIN_TIMEOUT   = 200,        // Minimum data delivery timeout.
  DATA_RETRY_MAX              = 10,         // Maximum number of data repetitions.
  DATA_RETRY_MIN              = 1,          // Minimum number of data repetitions.
  MAX_GPIO_PINS_NUMBER        = 26,         // Max GPIO pins number.
};

// Field names correspond to FW_ParamValue_t union field names.
typedef struct _parameters_tag
{
  int16_t            role;
  /* NodeCapability is absent from this structure.
     If requested, it is obtained through the ZDOM__DeviceType__get. */
  uint16_t           logicalAddr;
  NWKAddr_t          NWKAddr;
  uint64_t           macAddr;
  uint16_t           panID;
  uint16_t           actPANID;
  uint8_t            channel;
  uint32_t           channelMask;
  FW_LQIParam_t      lqi;
  int8_t             txPower;
  NodeAddrInf_t      parentAddrInfo;
  /* ChildMACAddr is absent from this structure, because
     it is a readonly parameter. It is not clear now
     how to implement it. */
  uint16_t           autoNetwork;
  FW_PowerDuration_t powerDuration;
  uint16_t           syncPeriod;
  uint128_t          securityKey;
  /* ManufacturerId, modelId, hardsoftId are absent from this
     structure; they are "hardcoded" const values (in fw_version.v). */
  uint16_t           dataDeliveryTimeout;
  uint8_t            dataRetryMax;
  FW_GPIOConfig_t    gpioConfig [MAX_GPIO_PINS_NUMBER];
  bool               uartDTR;
  UartBaudRate       uartSpeed;
  bool               uartFC;
  uint8_t            adcConfig;
  I2CMode_t          i2cConfig;
  FW_USARTConfig_t   usartConfig;
  uint16_t           calibrPeriod;
} parametersType;

typedef struct
{
  int16_t            role;
  uint16_t           logicalAddr;
  uint64_t           macAddr;
  uint16_t           panID;
  uint32_t           channelMask;
  FW_PowerDuration_t powerDuration;
  uint16_t           syncPeriod;
  bool               uartDTR;
} ParametersLockedAtJoinSubset_t;

extern ParametersLockedAtJoinSubset_t parametersLocked;
extern parametersType parameters;

#endif//_PARAMETERS_H_DEFINED_

// eof parameters.h
