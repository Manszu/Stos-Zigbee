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
**  framework.h
**  
**  eZeeNet framework library header file.
*******************************************************************************/

#ifndef FRAMEWORK_H_
#define FRAMEWORK_H_

#include "stack.h"
#include "gpio.h"
#include "uart_fxns.h"
#include "spi_master_fxns.h"
#include "i2cpacket.h"
#include "eeprom.h"
#include "hplinterrupt.h"

/*******************************************************************************
**  Framework data types and constants declaration.
*******************************************************************************/

typedef enum // Reset reason.
{
  FW_COLD_RESET = 0,  // Cold reset has been performed.
  FW_WARM_RESET = 1,  // Cold reset occured.
  FW_WD_RESET   = 2,    // Watchdog timer caused the reset.
} FW_ResetReason_t;

/*******************************************************************************
**  Parameter management types and functions.
*******************************************************************************/

#define FW_BROADCAST_ADDR                              0xFFFF   // Network broadcast address.

typedef struct 
{
  uint64_t lo;
  uint64_t hi;
} uint128_t;


enum
{
  FW_DATA_MAX_LENGTH  = 84, // Maximum data length for transmission.
  SECURITY_KEY_SIZE   = 16, // Size of the security key.
  MANUFACTURE_ID_SIZE = 11, // Size of manufacture ID.
  MODEL_ID_SIZE       = 7,  // Size of model ID.
  HARDSOFT_ID_SIZE    = 47  // Size of hard and soft ID.
};


typedef enum // Parameter IDs.
{
  FW_NODE_ROLE_PARAM_ID,
  FW_NODE_CAPABILITY_PARAM_ID,
  FW_NODE_LOGICAL_ADDR_PARAM_ID,
  FW_NODE_NWK_ADDR_PARAM_ID,
  FW_MAC_ADDR_PARAM_ID,
  FW_PANID_PARAM_ID,
  FW_ACTUAL_PANID_PARAM_ID,
  FW_CURRENT_CHANNEL_PARAM_ID,
  FW_CHANNEL_MASK_PARAM_ID,
  FW_LQI_PARAM_ID,
  FW_TX_POWER_PARAM_ID,
  FW_PARENT_ADDR_INFO_PARAM_ID,
  FW_CHILDREN_ADDR_INFO_PARAM_ID,
  FW_AUTO_NETWORK_PARAM_ID,
  FW_POWER_DURATION_PARAM_ID,
  FW_SECURITY_KEY_PARAM_ID,
  FW_MANUFACTURER_ID_PARAM_ID,
  FW_MODEL_ID_PARAM_ID,
  FW_HARDWARE_SOFTWARE_ID_PARAM_ID,
  FW_DATA_RETRY_PARAM_ID,
  FW_DATA_TIMEOUT_PARAM_ID,
  FW_SYNC_PERIOD_PARAM_ID,
  FW_GPIO_CONFIG_PARAM_ID,
  FW_UART_DTR_PARAM_ID,
  FW_UART_SPEED_PARAM_ID,
  FW_UART_FC_PARAM_ID,
  FW_ADC_CONFIG_PARAM_ID,
  FW_I2C_CONFIG_PARAM_ID,
  FW_USART_CONFIG_PARAM_ID,
  FW_CALIBR_PERIOD_PARAM_ID, 
  FW_FIRST_UNUSED_PARAM_ID, // This ID must be the last; user application
                            // allowed to use IDs starting from this value
} FW_ParamID_t;

typedef struct // Possible node's roles.
{
  bool coordinator; // Node can be coordinator.
  bool router;      // Node can be router.
  bool endDevice;   // Node can be end-device.
} NodeCapability_t;

typedef int NodeLogicalAddr_t; // Node logical address.

typedef struct // LQI parameter.
{
  IEEEAddr_t extAddr; // MAC address of the node from which LQI ans RSSI are requested,
  uint8_t        lqi; // LQI value.
  int8_t        rssi; // RSSI value in dBm.
} FW_LQIParam_t;

typedef struct // Children address descriptor.
{
   uint8_t        size; // Children address information descriptor size.
   NodeAddrInf_t* addr; // Children address information array.
} FW_ChildrenAddrInfo_t;

typedef struct // Power duration parameter.
{
  uint16_t active; // Active interval for end-device.
  uint16_t sleep;  // Sleep interval for end-device.
} FW_PowerDuration_t;

typedef struct // GPIO configuration parameter.
{
  uint8_t    gpio; // GPIO number.
  GPIOMode_t mode; // GPIO mode.
} FW_GPIOConfig_t;

typedef enum // USART state.
{
  FW_USART_MODE_OFF,  // USART is not used.
  FW_USART_MODE_UART, // USART is in UART mode.
  FW_USART_MODE_SPI,  // USART is in SPI mode.
} FW_USARTState_t;

typedef struct // USART configuration.
{
  FW_USARTState_t state; // USART state.
  union                  // UART parameters.
  {
    UartMode uart; // UART parameters.
    SpiMasterMode spi; // SPI parameters.
  } mode;
} FW_USARTConfig_t; 

typedef union // Parameters.
{
  NodeLogicalType_t                  role;
  NodeCapability_t         nodeCapability;
  NodeLogicalAddr_t           logicalAddr;
  NWKAddr_t                       NWKAddr;
  IEEEAddr_t                      macAddr;
  PANID_t                           panID;
  PANID_t                        actPANID;
  uint8_t                         channel;
  uint32_t                    channelMask;
  FW_LQIParam_t                       lqi;
  int8_t                          txPower;
  NodeAddrInf_t            parentAddrInfo;
  FW_ChildrenAddrInfo_t  childrenAddrInfo;
  uint16_t                    autoNetwork;
  FW_PowerDuration_t        powerDuration;
  uint16_t                     syncPeriod;
  uint128_t                   securityKey;
  const char**                      ppStr;
  uint16_t            dataDeliveryTimeout;
  uint8_t                    dataRetryMax;
  FW_GPIOConfig_t*             gpioConfig;
  bool                            uartDTR;
  UartBaudRate                  uartSpeed;
  bool                             uartFC;
  uint8_t                       adcConfig;
  I2CMode_t                     i2cConfig;
  FW_USARTConfig_t*           usartConfig;
  uint16_t                   calibrPeriod;
} FW_ParamValue_t;

typedef struct // Parameter structure.
{
  FW_ParamID_t id;       // Parameter ID.
  FW_ParamValue_t value; // Parameter value.
} FW_Param_t;

typedef enum // Address mode for data transmission.
{
  NODE_NWK_ADDR_MODE,     // Network address is used.
  NODE_LOGICAL_ADDR_MODE, // Logical address is used.
} NodeAddrMode_t;

typedef struct // Possible network events.
{
  void (*joined)(void);               // Node is joined to network.
  void (*lost)(void);                 // Node lost network connection.
  void (*addNode)(const NodeAddrInf_t* addr);   // New child has been added.
  void (*deleteNode)(const NodeAddrInf_t* addr); // Child has been deleted.
} FW_NetworkEvents_t;

typedef struct // Node Logical Address information
{
  NodeLogicalAddr_t logicalAddr;
  NWKAddr_t         NWKAddr;
} NodeLogicalInf_t;

typedef struct // Network events concerned with logical addressing (acceptable for coordinator only)
{
  void (*addLogicalNodeAddr)(const NodeLogicalInf_t* addr);   // New node with logical address has joined to the network.
  void (*deleteLogicalNodeAddr)(const NodeLogicalInf_t* addr); // Existing node with logical address has left the network.
} FW_LogicalAddressEvents_t;

typedef enum // Data transmission status.
{
  FW_DATA_ACK_STATUS,   // Data transmission success.
  FW_DATA_NOACK_STATUS, // Data transmission failure.
} FW_DataStatus_t;

typedef struct // Request data transmission parameters.
{
  NodeLogicalAddr_t dstLogicalAddr; // Destination logical address.
  NWKAddr_t dstNWKAddr;             // Destination network address.
  NodeAddrMode_t addrMode;          // Destination address mode.
  uint8_t srcEndPoint;              // Source end-point.
  uint8_t dstEndPoint;              // Destination end-point.
  bool arq;                         // ARQ control.
  bool broadcast;                   // Broadcast flag.
  uint8_t* data;                    // Data buffer pointer.
  uint8_t length;                   // Data length.
  uint8_t handle;                   // Data handle.
} FW_DataRequest_t;

typedef struct // Indication data transmission parameters.
{
  NodeLogicalAddr_t srcLogicalAddr; // Source logical address.
  NWKAddr_t srcNWKAddr;             // Network logical address.
  bool isLogicalValid;              // Logical address valid flag.
  uint8_t srcEndPoint;              // Source end-point.
  uint8_t dstEndPoint;              // Destination end-point.
  uint8_t* data;                    // Data buffer pointer.
  uint8_t length;                   // Data length.
  bool broadcast;                   // Broadcast flag.
  uint8_t lqi;                      // LQI value.
  int8_t rssi;                      // RSSI value.
} FW_DataIndication_t;

/*******************************************************************************
  Description: Through this function control is passed to the user.

  Parameters: resetReason - reason of the reset which occured,
              isEepromCrcValid - TRUE if ParamServer EEPROM CRC status is ok.

  Returns: nothing.
*******************************************************************************/
void fw_userEntry(FW_ResetReason_t resetReason, bool isEepromCrcValid);

/*******************************************************************************
  Description: sets parameters of users main loop. This function is usually
               called at the end of fw_userEntry().

  Parameters: period - call interval in milliseconds.
              userLoop - users main loop periodically called.
                         If period is set to 0, then userLoop will be called
                         by Framework as soon as possible.

  Returns: nothing.
*******************************************************************************/
void fw_setUserLoop(uint32_t period, void (*userLoop)(void));

/*******************************************************************************
  Description: provides system time in milliseconds.

  Parameters: nothing.

  Returns: system time in milliseconds (passed from the last cold or warm reset).
*******************************************************************************/
uint32_t fw_getSystemTime(void);

/*******************************************************************************
  Description: performs warm reset. Warm reset procedure reinitializes hardware
               and software, excluding cleaning the EEPROM data. The node will
               fall to the same state as it was reset by the hardware RESET
               signal.

  Parameters: factoryDefaults - if TRUE, set all eZeeNet parameters, stored
              in the EEPROM, to their default values.

  Returns: nothing.
*******************************************************************************/
void fw_warmReset(bool factoryDefaults);

/*******************************************************************************
  Description: registers callbacks for notification that the Framework is ready
               to sleep and notification that the node is waken up.


  Parameters: fwReadyToSleep - pointer to the users function that is called
                               by eZeeNet when it decides to sleep. This is the
                               right place to prepare all users peripheral
                               resources for sleeping phase (to move them to
                               tri-state, etc.).
              wakeup - pointer to the users function that is called when
                       eZeeNet is waken up.

  Returns: nothing.
*******************************************************************************/
void fw_registerSleep(void (*fwReadyToSleep)(void), void (*wakeup)(void));

/*******************************************************************************
  Description: forces the node to get asleep.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void fw_forceToSleep(void);

/*******************************************************************************
  Description: forces the node to wake up before the end of sleep period.

  Parameters: 

  Returns: nothing.
*******************************************************************************/
void fw_forceWakeup(void);

/*******************************************************************************
  Description: indicates that users application is ready to get asleep.

  Parameters: nothing.

  Returns: BUSY if Framework cannot proceed the sleep procedure 
           (it means,that user calls the function, but fwReadyToSleep 
           has not been got yet) or SUCCESS, otherwise.
*******************************************************************************/
result_t fw_appReadyToSleep(void);

/*******************************************************************************
  Description: writes a block of bytes to EEPROM.

  Parameters: params  pointer to writing parameters.

  Returns: nothing.
*******************************************************************************/
void fw_eepromWrite(EEPROMParams_t *params);

/*******************************************************************************
  Description: reads a block of bytes from EEPROM.

  Parameters: params - pointer to writing parameters.

  Returns: nothing.
*******************************************************************************/
void fw_eepromRead(EEPROMParams_t *params);

/*******************************************************************************
  Description: sets eZeeNet parameter.

  Parameters: param - parameter description pointer.

  Returns: FAIL, if id in param is out of range or SUCCESS, otherwise.
*******************************************************************************/
result_t fw_setParam(const FW_Param_t *param);

/*******************************************************************************
  Description: gets eZeeNet parameter.

  Parameters: param - parameter description pointer.

  Returns: FAIL, if id in param is out of range or SUCCESS, otherwise.
*******************************************************************************/
result_t fw_getParam(FW_Param_t *param);

/*******************************************************************************
  Description: registers the network event handlers.

  Parameters: handlers - pointer to network event handlers set.

  Returns: nothing.
*******************************************************************************/
void fw_registerNetworkEvents(const FW_NetworkEvents_t *handlers);

/*******************************************************************************
  Description: registers the handlers for events concerned with logical
               addressing (for coordinator only).

  Parameters: handlers - pointer to the logical addressing events' handlers set.

  Returns: nothing.
*******************************************************************************/
void fw_registerLogicalAddressEvents(const FW_LogicalAddressEvents_t *handlers);

/*******************************************************************************
  Description: forces a node to start a network (if coordinator) or 
               join (if router or end-device) to the existing network.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void fw_joinNetwork(void);

/*******************************************************************************
  Description: force a node to leave network.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void fw_leaveNetwork(void);

/*******************************************************************************
  Description: requests a node for networking status.

  Parameters: nothing.

  Returns: TRUE if the node is in the network, FALSE  the node is not joined 
           to the network or it has been orphaned by its parent.
*******************************************************************************/
bool fw_isJoined(void);

/*******************************************************************************
  Description: registers the end-point for data receive.

  Parameters: endpoint - endpoint number wish to register.
                         Users application should use the endpoint range 
                         from 1 to 239. Endpoint 240 is used for Framework
                         purposes. The rest possible 8-bit values are reserved 
                         for network management.
              ind - data indication handler associated with the endpoint.

  Returns: FAIL, if endpoint number is out of range and SUCCESS, otherwise.
*******************************************************************************/
result_t fw_registerEndPoint(uint8_t endpoint, void (*ind)(const FW_DataIndication_t *params));

/*******************************************************************************
  Description: requests data transmission.

  Parameters: params - data transmission parameters. There is no need to keep
                       formed request in memory until corresponded 
                       acknowledgement will be received.
              conf - confirmation for data transmission.

  Returns: FAIL  if the data to be transmitted is too long. BUSY  if the data
           cannot be transmitted at the moment and transmission can be repeated
           later, SUCCESS  otherwise.
*******************************************************************************/
result_t fw_dataRequest(const FW_DataRequest_t *params,
                        void (*conf)(uint8_t handle, FW_DataStatus_t status));

/*******************************************************************************
  Description: manipulates by eZeeNet data indication flow.

  Parameters: endpoint - the number of the endpoint for which flow control is
                         changed. Value 0 is used for simultaneous control of
                         all endpoints.
              enable - the indication mode. If it is TRUE, received data will
                       be sent to user application, otherwise they will be stored
                       by eZeeNet until mode is TRUE. Atfer mode becames TRUE
                       stored frames will be indicated to user application.

  Returns: nothing.
*******************************************************************************/
void fw_dataIndicationControl(uint8_t endpoint, bool enable);

/*******************************************************************************
  Description: requests explicitly for the data buffered on the router.
               Requesting for the data which are possibly buffered on its router
               (see Figure 11) is required when the end-device should participate
               in two-way communications with other node. eZeeNet executes this
               request automatically on each wakeup, so this function has to be
               used in rare cases.

  Parameters: nothing.

  Returns: TRUE, if there is a data on the router and it was extracted 
           FALSE, otherwise.
*******************************************************************************/
bool fw_delayedDataRequest(void);

/*******************************************************************************
  Description: Restarts the eZeeNet stack. It is required in some cases, for
               instance when application software needs to apply the changes in
               some global settings.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void fw_stackRestart(void);

/*******************************************************************************
  Description: encrypts the 16-byte block with the 16-byte key using
               AES-128 algorithm.

  Parameters: key - cipher key with length 16 byte
              buf - plaintext with length 16 byte
              temp - array for allocation of key schedule with length 176 byte.

  Returns: nothing.
*******************************************************************************/
void aes_encrypt (const uint8_t key[], uint8_t buf[], uint8_t temp[]);

/*******************************************************************************
  Description: decrypts the 16-byte block with the 16-byte key using
               AES-128 algorithm.

  Parameters: key - cipher key with length 16 byte
              buf - ciphertext with length 16 byte
              temp - array for allocation of key schedule with length 176 byte.

  Returns: nothing.
*******************************************************************************/
void aes_decrypt (const uint8_t key[], uint8_t buf[], uint8_t temp[]);

#endif //FRAMEWORK_H_

// eof framework.h
