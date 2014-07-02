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

/******************************************************************************
The implementation of some TOS functions.
******************************************************************************/

#include <avrhardware.h>
#include <hardware.h>
#include <stdio.h>

/*******************************************************************************
  Description: Delay.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void TOSH_wait()
{
  asm volatile("nop");
  asm volatile("nop");
}

/*******************************************************************************
  Description: Idle mode.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void TOSH_sleep()
{
  SMCR = 0x1;
  asm volatile ("sleep");
  SMCR = 0;
}

/*******************************************************************************
  Description: Atomic statement runtime support.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
inline __nesc_atomic_t __nesc_atomic_start(void)
{
  __nesc_atomic_t result = SREG;
  cli();
  #ifdef DEBUG_PROFILE_ATOMIC
    DDRB |= 1<<PIN6; //yellow led on
    PORTB |= 1<<PIN6;
  #endif
  return result;
}

/*******************************************************************************
  Description: Atomic statement runtime support.

  Parameters: oldSreg.

  Returns: nothing.
*******************************************************************************/
inline void __nesc_atomic_end(__nesc_atomic_t oldSreg)
{
  SREG = oldSreg;
  #ifdef DEBUG_PROFILE_ATOMIC
    PORTB &= ~(1<<PIN6); //yellow led off
  #endif
}

/**************************************************
 * Endian-dependent byte swapping functions
 *************************************************/

/*******************************************************************************
  Description: Converts uint16 (uint32, uint64) from platform-specific endian format to low-endian format.

  Parameters: fromU16 (fromU32, fromU64) - uint16 (uint32, uint64) value in host endian format.

  Returns: uint16 (uint32, uint64) value in low-endian format.
*******************************************************************************/
inline uint16_t htole_u16(uint16_t fromU16) 
{ 
  return fromU16; 
}

/*******************************************************************************/
inline uint32_t htole_u32(uint32_t fromU32) 
{ 
  return fromU32; 
}

/*******************************************************************************/
inline uint64_t htole_u64(uint64_t fromU64) 
{ 
  return fromU64; 
}

/*******************************************************************************
  Description: Converts uint16 (uint32, uint64) from low-endian format to platform-specific endian format.

  Parameters: fromU16 (fromU32, fromU64) - uint16 (uint32, uint64) value in low-endian format.

  Returns:  uint16 (uint32, uint64) value in host endian format.
*******************************************************************************/
inline uint16_t letoh_u16(uint16_t fromU16) 
{ 
  return fromU16; 
}

/*******************************************************************************/
inline uint32_t letoh_u32(uint32_t fromU32) 
{ 
  return fromU32; 
}
 
/*******************************************************************************/
inline uint64_t letoh_u64(uint64_t fromU64) 
{ 
  return fromU64; 
} 

// eof tos.c
