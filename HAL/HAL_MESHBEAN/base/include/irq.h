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

#ifndef IRQ_H_
#define IRQ_H_

#include <tos.h>

#define IRQ_0 0
#define IRQ_1 1
#define IRQ_2 2
#define IRQ_3 3
#define IRQ_4 4
#define IRQ_5 5
#define IRQ_6 6  // Valid for use
#define IRQ_7 7  // Valid for use

// interrupt activation condition.
typedef enum
{
  IRQ_LOW_LEVEL,     // The low level generates an interrupt request.
  IRQ_ANY_EDGE,      // Any edge generates an interrupt request.
  IRQ_FALLING_EDGE,  // Falling edge generates an interrupt request.
  IRQ_RISING_EDGE    // Rising edge generates an interrupt request.
} irqMode_t;

/*******************************************************************************
  Description: Registers user's irqNumber interrupt.

  Parameters: irqNumber - IRQ number.
              irqMode - Interrupt sence control.
              f - user's interrupt handler.

  Returns: FAIL - if irqNumber is out of range or such interrupt has 
           been already registered.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_register(uint8_t irqNumber, irqMode_t irqMode, void (*f)(void));

/*******************************************************************************
  Description: Enables irqNumber interrupt.

  Parameters: irqNumber - IRQ number.

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_enable(uint8_t irqNumber);

/*******************************************************************************
  Description: Disables irqNumber interrupt.

  Parameters: irqNumber - IRQ number

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_disable(uint8_t irqNumber);

/*******************************************************************************
  Description: Unregisters user's irqNumber interrupt.

  Parameters: irqNumber - IRQ number.

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_unregister(uint8_t irqNumber);

#endif /* IRQ_H_ */
