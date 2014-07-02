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

/*=============================================================
The implementation of IRQ interface.
===============================================================*/

#include <avr/interrupt.h>
#include <avr/io.h>
#include <irq.h>

#ifndef NUM_IRQ_LINES
#define NUM_IRQ_LINES 2
#endif

#ifndef FIRST_VALID_IRQ
#define FIRST_VALID_IRQ IRQ_6
#endif

typedef void (*IRQ_callback)(void);
IRQ_callback IRQ_callback_list[NUM_IRQ_LINES] = {[0 ... (NUM_IRQ_LINES - 1)] = NULL};

/*******************************************************************************
  Description: Registers user's irqNumber interrupt.

  Parameters: irqNumber - IRQ number.
              irqMode - Interrupt sence control.
              f - user's interrupt handler.

  Returns: FAIL - if irqNumber is out of range or such interrupt has 
           been already registered.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_register(uint8_t irqNumber, irqMode_t irqMode, void (*f)(void))
{
  uint8_t irqOffsetNumber = irqNumber - FIRST_VALID_IRQ;
  // irqNumber is out of range
  if (irqOffsetNumber >= NUM_IRQ_LINES)
    return FAIL;
  // Such interrupt has been already register
  if (IRQ_callback_list[irqOffsetNumber] != NULL)
    return FAIL;
  // IRQ pin is input
  DDRE &= ~(1 << irqNumber);
  PORTE |= (1 << irqNumber);
  uint8_t ui8ShiftCount = (irqNumber - IRQ_4) << 1;
  // Clear previous settings of corresponding interrupt sense control
  EICRB &= ~(3 << ui8ShiftCount);
  // Setup corresponding interrupt sence control
  EICRB |= (irqMode & 0x03) << ui8ShiftCount;
  // Clear the INTn interrupt flag
  EIFR &= ~(1 << irqNumber);
  IRQ_callback_list[irqOffsetNumber] = f;
  return SUCCESS;
}

/*******************************************************************************
  Description: Enables irqNumber interrupt.

  Parameters: irqNumber - IRQ number.

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_enable(uint8_t irqNumber)
{
  uint8_t irqOffsetNumber = irqNumber - FIRST_VALID_IRQ;
  // irqNumber is out of range
  if (irqOffsetNumber >= NUM_IRQ_LINES)
    return FAIL;
  // Interrupt has not been opened yet
  if (IRQ_callback_list[irqOffsetNumber] == NULL)
    return FAIL;
  // Enable external interrupt request
  EIMSK |= (1 << irqNumber);
  return SUCCESS;
}

/*******************************************************************************
  Description: Disables irqNumber interrupt.

  Parameters: irqNumber - IRQ number

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_disable(uint8_t irqNumber)
{
  uint8_t irqOffsetNumber = irqNumber - FIRST_VALID_IRQ;
  // irqNumber is out of range
  if (irqOffsetNumber >= NUM_IRQ_LINES)
    return FAIL;
  // Interrupt has not been opened yet
  if (IRQ_callback_list[irqOffsetNumber] == NULL)
    return FAIL;
  // Disable external interrupt request
  EIMSK &= ~(1 << irqNumber);
  return SUCCESS;
}

/*******************************************************************************
  Description: Unregisters user's irqNumber interrupt.

  Parameters: irqNumber - IRQ number.

  Returns: FAIL - if irqNumber is out of range or has not been registered yet.
           SUCCESS - otherwise.
*******************************************************************************/
result_t irq_unregister(uint8_t irqNumber)
{
  uint8_t irqOffsetNumber = irqNumber - FIRST_VALID_IRQ;
  // irqNumber is out of range
  if (irqOffsetNumber >= NUM_IRQ_LINES)
    return FAIL;
  // Interrupt has not been opened yet
  if (IRQ_callback_list[irqOffsetNumber] == NULL)
    return FAIL;
  // Disable external interrupt request
  EIMSK &= ~(1 << irqNumber);
  IRQ_callback_list[irqOffsetNumber] = NULL;
  // IRQ pin is tri-state
  DDRE &= ~(1 << irqNumber);
  PORTE &= ~(1 << irqNumber);
  return SUCCESS;
}

/*******************************************************************************
  Description: External interrupt 6 handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
SIGNAL(SIG_INTERRUPT6)
{
  IRQ_callback_list[IRQ_6 - FIRST_VALID_IRQ]();
}

/*******************************************************************************
  Description: External interrupt 7 handler.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
SIGNAL(SIG_INTERRUPT7)
{
  IRQ_callback_list[IRQ_7 - FIRST_VALID_IRQ]();
}

// eof irq.c
