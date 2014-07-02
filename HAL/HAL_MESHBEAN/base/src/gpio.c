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

/*===============================================================
The implementation of GPIO interface.
=================================================================*/

#include <avr/io.h>
#include <gpio.h>
#include <avr/pgmspace.h>

#define GPIO_0_PORT PORTB
#define GPIO_1_PORT PORTB
#define GPIO_2_PORT PORTB
#define GPIO_3_PORT PORTG
#define GPIO_4_PORT PORTG
#define GPIO_5_PORT PORTG
#define GPIO_6_PORT PORTD
#define GPIO_7_PORT PORTD
#define GPIO_8_PORT PORTE
#define GPIO_9_PORT PORTD
#define GPIO_10_PORT PORTD
#define GPIO_11_PORT PORTD
#define GPIO_12_PORT PORTD
#define GPIO_13_PORT PORTD
#define GPIO_14_PORT PORTD
#define GPIO_15_PORT PORTF
#define GPIO_16_PORT PORTF
#define GPIO_17_PORT PORTF
#define GPIO_18_PORT PORTF
#define GPIO_19_PORT PORTG
#define GPIO_20_PORT PORTE
#define GPIO_21_PORT PORTE
#define GPIO_22_PORT PORTE
#define GPIO_23_PORT PORTE
#define GPIO_24_PORT PORTE
#define GPIO_25_PORT PORTE

#define GPIO_0_PIN 5
#define GPIO_1_PIN 6
#define GPIO_2_PIN 7
#define GPIO_3_PIN 0
#define GPIO_4_PIN 1
#define GPIO_5_PIN 2
#define GPIO_6_PIN 6
#define GPIO_7_PIN 7
#define GPIO_8_PIN 3
#define GPIO_9_PIN 0
#define GPIO_10_PIN 1
#define GPIO_11_PIN 2
#define GPIO_12_PIN 3
#define GPIO_13_PIN 4
#define GPIO_14_PIN 5
#define GPIO_15_PIN 3
#define GPIO_16_PIN 2
#define GPIO_17_PIN 1
#define GPIO_18_PIN 0
#define GPIO_19_PIN 5
#define GPIO_20_PIN 4
#define GPIO_21_PIN 0
#define GPIO_22_PIN 1
#define GPIO_23_PIN 2
#define GPIO_24_PIN 6
#define GPIO_25_PIN 7

#define GPIOCONFIGSIZE (GPIO_IRQ_7 + 1)
typedef struct 
{
  volatile uint8_t *port;
  uint8_t pin;
} gpio_config_t;

const gpio_config_t gpio_config[] PROGMEM =
{
  {
    .port = &GPIO_0_PORT,
    .pin = GPIO_0_PIN
  },
  {
    .port = &GPIO_1_PORT,
    .pin = GPIO_1_PIN
  },
  {
    .port = &GPIO_2_PORT,
    .pin = GPIO_2_PIN
  },
  {
    .port = &GPIO_3_PORT,
    .pin = GPIO_3_PIN
  },
  {
    .port = &GPIO_4_PORT,
    .pin = GPIO_4_PIN
  },
  {
    .port = &GPIO_5_PORT,
    .pin = GPIO_5_PIN
  },
  {
    .port = &GPIO_6_PORT,
    .pin = GPIO_6_PIN
  },
  {
    .port = &GPIO_7_PORT,
    .pin = GPIO_7_PIN
  },
  {
    .port = &GPIO_8_PORT,
    .pin = GPIO_8_PIN
  },
  {
    .port = &GPIO_9_PORT,
    .pin = GPIO_9_PIN
  },
  {
    .port = &GPIO_10_PORT,
    .pin = GPIO_10_PIN
  },
  {
    .port = &GPIO_11_PORT,
    .pin = GPIO_11_PIN
  },
  {
    .port = &GPIO_12_PORT,
    .pin = GPIO_12_PIN
  },
  {
    .port = &GPIO_13_PORT,
    .pin = GPIO_13_PIN
  },
  {
    .port = &GPIO_14_PORT,
    .pin = GPIO_14_PIN
  },
  {
    .port = &GPIO_15_PORT,
    .pin = GPIO_15_PIN
  },
  {
    .port = &GPIO_16_PORT,
    .pin = GPIO_16_PIN
  },
  {
    .port = &GPIO_17_PORT,
    .pin = GPIO_17_PIN
  },
  {
    .port = &GPIO_18_PORT,
    .pin = GPIO_18_PIN
  },
  {
    .port = &GPIO_19_PORT,
    .pin = GPIO_19_PIN
  },
  {
    .port = &GPIO_20_PORT,
    .pin = GPIO_20_PIN
  },
  {
    .port = &GPIO_21_PORT,
    .pin = GPIO_21_PIN
  },
  {
    .port = &GPIO_22_PORT,
    .pin = GPIO_22_PIN
  },
  {
    .port = &GPIO_23_PORT,
    .pin = GPIO_23_PIN
  },
  {
    .port = &GPIO_24_PORT,
    .pin = GPIO_24_PIN
  },
  {
    .port = &GPIO_25_PORT,
    .pin = GPIO_25_PIN
  }
};

/*******************************************************************************
  Description: Configure particular pin of ZigBit module to the specified mode.

  Parameters: pin - GPIO pin sequence number.
              mode - GPIO mode.

  Returns:  FAIL - if there is no such GPIO pin sequence number.
            SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_setConfig(uint8_t pin, GPIOMode_t mode)
{
  gpio_config_t i;
  if( pin < GPIOCONFIGSIZE )
  {
    memcpy_P(&i, &gpio_config[ pin ], sizeof(gpio_config_t));    
    // Setup DDRxn
    if(mode == GPIO_OUTPUT) *(i.port - 1) |= (1 << i.pin);
    else *(i.port - 1) &= ~(1 << i.pin);
    // Setup PORTxn
    if(mode == GPIO_INPUT_PULLUP_ON) *i.port |= (1 << i.pin);
    else *i.port &= ~(1 << i.pin);
   
    return SUCCESS;
  }
  else
  {
    return FAIL;
  }
}

/*******************************************************************************
  Description: Read configuration for particular pin.
               Configuration is returned in mode.

  Parameters: pin - GPIO pin sequence number.
              mode - GPIO mode.

  Returns:  FAIL - if there is no such GPIO pin sequence number.
            SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_getConfig(uint8_t pin, GPIOMode_t *mode)
{
  gpio_config_t i;
  uint8_t state;
  if( pin < GPIOCONFIGSIZE )
  {
    memcpy_P(&i, &gpio_config[ pin ], sizeof(gpio_config_t));
    // Read PORTxn
    state = (*i.port >> i.pin) & 0x01;
    // Read DDRxn
    state |= ((*(i.port - 1) >> i.pin) & 0x01) << 1;
    switch (state) 
    {
      case 0: *mode = GPIO_Z; break;
      case 1: *mode = GPIO_INPUT_PULLUP_ON; break;
      case 2: 
      case 3: *mode = GPIO_OUTPUT; break;
    }
    return SUCCESS;
  }
  else
  {
    return FAIL;
  }
}

/*******************************************************************************
  Description: Get state of pin into value.

  Parameters: pin - GPIO pin sequence number.
              value - value of GPIO pin (0 or 1).

  Returns: FAIL - if there is no such GPIO pin sequence number.
           SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_getState(uint8_t pin, uint8_t *value)
{
  gpio_config_t i;
  if( pin < GPIOCONFIGSIZE )
  {
    memcpy_P(&i, &gpio_config[ pin ], sizeof(gpio_config_t));
    *value = (*(i.port - 2) >> i.pin) & 0x01;
    return SUCCESS;
  }
  else
  {
    return FAIL;
  }
}

/*******************************************************************************
  Description: Sets state of pin into value.

  Parameters: pin - GPIO pin sequence number.
              value - value of GPIO pin (0 or 1).

  Returns: FAIL - if there is no such GPIO pin sequence number.
           SUCCESS - otherwise.
*******************************************************************************/
result_t gpio_setState(uint8_t pin, uint8_t value)
{
  gpio_config_t i;
  GPIOMode_t mode;
  if( pin < GPIOCONFIGSIZE )
  {
    memcpy_P(&i, &gpio_config[ pin ], sizeof(gpio_config_t));
    gpio_getConfig(pin, &mode);
    // Write PORTxn
    if( mode == GPIO_OUTPUT ) // Sets PORTxn of GPIO a pin if a pin is configurated as output
    {
      *i.port = (*i.port & (~(1 << i.pin))) | ((value & 0x01) << i.pin);
      return SUCCESS;
    }
    else
    {
      return FAIL;
    }
  }
  else
  {
    return FAIL;
  }
}

// eof gpio.c
