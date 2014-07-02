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

#ifndef GPIO_H_
#define GPIO_H_

#include <tos.h>

#define GPIO_0 0              //GPIO0 ZigBit pin name 
#define GPIO_1 1              //GPIO1 ZigBit pin name 
#define GPIO_2 2              //GPIO2 ZigBit pin name  
#define GPIO_3 3              //GPIO3 ZigBit pin name    
#define GPIO_4 4              //GPIO4 ZigBit pin name  
#define GPIO_5 5              //GPIO5 ZigBit pin name 
#define GPIO_6 6              //GPIO6 ZigBit pin name 
#define GPIO_7 7              //GPIO7 ZigBit pin name 
#define GPIO_8 8              //GPIO8 ZigBit pin name 
#define GPIO_I2C_CLK 9        //I2C_CLK ZigBit pin name  
#define GPIO_I2C_DATA 10      //I2C_DATA ZigBit pin name 
#define GPIO_UART_TXD 11      //UART_TXD ZigBit pin name 
#define GPIO_UART_RXD 12      //UART_RXD ZigBit pin name 
#define GPIO_UART_RTS 13      //UART_RTS ZigBit pin name 
#define GPIO_UART_CTS 14      //UART_CTS ZigBit pin name 
#define GPIO_ADC_INPUT_3 15   //ADC_INPUT_3 ZigBit pin name 
#define GPIO_ADC_INPUT_2 16   //ADC_INPUT_2 ZigBit pin name 
#define GPIO_ADC_INPUT_1 17   //ADC_INPUT_1 ZigBit pin name 
#define GPIO_BAT 18           //BAT ZigBit pin name 
#define GPIO_1WR 19           //1WR ZigBit pin name 
#define GPIO_UART_DTR 20      //UART_DTR ZigBit pin name 
#define GPIO_USART0_RXD 21    //USART0_RXD ZigBit pin name 
#define GPIO_USART0_TXD 22    //USART0_TXD ZigBit pin name 
#define GPIO_USART0_EXTCLK 23 //UART0_EXTCLK ZigBit pin name 
#define GPIO_IRQ_6 24         //IRQ_6 ZigBit pin name 
#define GPIO_IRQ_7 25         //IRQ_7 ZigBit pin name 


typedef enum
{
  GPIO_INPUT_PULLUP_OFF, // input, pullup is off
  GPIO_INPUT_PULLUP_ON,  // input, pullup is on
  GPIO_Z,                // tri-state
  GPIO_OUTPUT            // output
} GPIOMode_t;

/*******************************************************************************
  Description: Configure particular pin of ZigBit module to the specified mode.

  Parameters: pin - GPIO pin sequence number.
              mode - GPIO mode.

  Returns:  FAIL - if there is no such GPIO pin sequence number.
            SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_setConfig(uint8_t pin, GPIOMode_t mode);

/*******************************************************************************
  Description: Read configuration for particular pin.
               Configuration is returned in mode.

  Parameters: pin - GPIO pin sequence number.
              mode - GPIO mode.

  Returns:  FAIL - if there is no such GPIO pin sequence number.
            SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_getConfig(uint8_t pin, GPIOMode_t *mode);

/*******************************************************************************
  Description: Get state of pin into value.

  Parameters: pin - GPIO pin sequence number.
              value - value of GPIO pin (0 or 1).

  Returns: FAIL - if there is no such GPIO pin sequence number.
           SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_getState(uint8_t pin, uint8_t *value);

/*******************************************************************************
  Description: Sets state of pin into value.

  Parameters: pin - GPIO pin sequence number.
              value - value of GPIO pin (0 or 1).

  Returns: FAIL - if there is no such GPIO pin sequence number.
           SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_setState(uint8_t pin, uint8_t value);

#endif /* GPIO_H_ */

// eof gpio.h
