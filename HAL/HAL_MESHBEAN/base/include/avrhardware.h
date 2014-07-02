/*									tab:4
 *
 *
 * "Copyright (c) 2000-2002 The Regents of the University  of California.  
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software and its
 * documentation for any purpose, without fee, and without written agreement is
 * hereby granted, provided that the above copyright notice, the following
 * two paragraphs and the author appear in all copies of this software.
 * 
 * IN NO EVENT SHALL THE UNIVERSITY OF CALIFORNIA BE LIABLE TO ANY PARTY FOR
 * DIRECT, INDIRECT, SPECIAL, INCIDENTAL, OR CONSEQUENTIAL DAMAGES ARISING OUT
 * OF THE USE OF THIS SOFTWARE AND ITS DOCUMENTATION, EVEN IF THE UNIVERSITY OF
 * CALIFORNIA HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * THE UNIVERSITY OF CALIFORNIA SPECIFICALLY DISCLAIMS ANY WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS FOR A PARTICULAR PURPOSE.  THE SOFTWARE PROVIDED HEREUNDER IS
 * ON AN "AS IS" BASIS, AND THE UNIVERSITY OF CALIFORNIA HAS NO OBLIGATION TO
 * PROVIDE MAINTENANCE, SUPPORT, UPDATES, ENHANCEMENTS, OR MODIFICATIONS."
 *
 */
/*									tab:4
 *  IMPORTANT: READ BEFORE DOWNLOADING, COPYING, INSTALLING OR USING.  By
 *  downloading, copying, installing or using the software you agree to
 *  this license.  If you do not agree to this license, do not download,
 *  install, copy or use the software.
 *
 *  Intel Open Source License 
 *
 *  Copyright (c) 2002 Intel Corporation 
 *  All rights reserved. 
 *  Redistribution and use in source and binary forms, with or without
 *  modification, are permitted provided that the following conditions are
 *  met:
 * 
 *	Redistributions of source code must retain the above copyright
 *  notice, this list of conditions and the following disclaimer.
 *	Redistributions in binary form must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in the
 *  documentation and/or other materials provided with the distribution.
 *      Neither the name of the Intel Corporation nor the names of its
 *  contributors may be used to endorse or promote products derived from
 *  this software without specific prior written permission.
 *  
 *  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 *  ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 *  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 *  PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE INTEL OR ITS
 *  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 *  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 *  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 *  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 *  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 *  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 *  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 * 
 */
/*
 *
 * Authors:             Jason Hill, Philip Levis, Nelson Lee
 *
 *
 */

#ifndef TOSH_AVRHARDWARE_H_
#define TOSH_AVRHARDWARE_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/pgmspace.h>

#include <stdio.h>
#include <avr/sfr_defs.h>

// defines for bits manipulation 
#define TOSH_ASSIGN_PIN(name, port, bit) \
static inline void TOSH_SET_##name##_PIN() {PORT##port |= (1 << bit);} \
static inline void TOSH_CLR_##name##_PIN() {PORT##port &= ~(1 << bit);} \
static inline char TOSH_READ_##name##_PIN() \
  {return (PIN##port & (1 << bit)) != 0;} \
static inline void TOSH_MAKE_##name##_OUTPUT() {DDR##port |= (1 << bit);} \
static inline void TOSH_MAKE_##name##_INPUT()  {DDR##port &= ~(1 << bit);} \
static inline void TOSH_MAKE_##name##_PULLUP() {PORT##port |= (1 << bit);} 

#define TOSH_ASSIGN_OUTPUT_ONLY_PIN(name, port, bit) \
static inline void TOSH_SET_##name##_PIN() {PORT##port |= (1 << bit);} \
static inline void TOSH_CLR_##name##_PIN() {PORT##port &= ~(1 << bit);} \
static inline void TOSH_MAKE_##name##_OUTPUT() {;} 

#define TOSH_ALIAS_OUTPUT_ONLY_PIN(alias, connector)\
static inline void TOSH_SET_##alias##_PIN() {TOSH_SET_##connector##_PIN();} \
static inline void TOSH_CLR_##alias##_PIN() {TOSH_CLR_##connector##_PIN();} \
static inline void TOSH_MAKE_##alias##_OUTPUT() {} \

#define TOSH_ALIAS_PIN(alias, connector) \
static inline void TOSH_SET_##alias##_PIN() {TOSH_SET_##connector##_PIN();} \
static inline void TOSH_CLR_##alias##_PIN() {TOSH_CLR_##connector##_PIN();} \
static inline char TOSH_READ_##alias##_PIN() {return TOSH_READ_##connector##_PIN();} \
static inline void TOSH_MAKE_##alias##_OUTPUT() {TOSH_MAKE_##connector##_OUTPUT();} \
static inline void TOSH_MAKE_##alias##_INPUT()  {TOSH_MAKE_##connector##_INPUT();} 


/*******************************************************************************
  Description: Delay.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void TOSH_wait();

/*******************************************************************************
  Description: Idle mode.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void TOSH_sleep();

// atomic statement runtime support
typedef uint8_t __nesc_atomic_t;

/*******************************************************************************
  Description: Atomic statement runtime support.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
inline __nesc_atomic_t __nesc_atomic_start(void);

/*******************************************************************************
  Description: Atomic statement runtime support.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
inline void __nesc_atomic_end(__nesc_atomic_t oldSreg);


/**************************************************
 * Endian-dependent byte swapping functions
 *************************************************/

/*******************************************************************************
  Description: Converts uint16 (uint32, uint64) from platform-specific endian format to low-endian format.

  Parameters: fromU16 (fromU32, fromU64) - uint16 (uint32, uint64) value in host endian format.

  Returns: uint16 (uint32, uint64) value in low-endian format.
*******************************************************************************/
inline uint16_t htole_u16(uint16_t fromU16);
inline uint32_t htole_u32(uint32_t fromU32);
inline uint64_t htole_u64(uint64_t fromU64);

/*******************************************************************************
  Description: Converts uint16 (uint32, uint64) from low-endian format to platform-specific endian format.

  Parameters: fromU16 (fromU32, fromU64) - uint16 (uint32, uint64) value in low-endian format.

  Returns:  uint16 (uint32, uint64) value in host endian format.
*******************************************************************************/
inline uint16_t letoh_u16(uint16_t fromU16);
inline uint32_t letoh_u32(uint32_t fromU32);
inline uint64_t letoh_u64(uint64_t fromU64);

/*
 * Byte-swapping function aliases defined below should be further placed into
 * the corresponding component headers, but now they 
 * are here to simplify and speed up patching process for Endian independence.
 */
// Use for ZigBee fields access within the MAC data buffer (host <-> zigbee)
#define htoz_u16 htole_u16
#define htoz_u32 htole_u32
#define htoz_u64 htole_u64
#define ztoh_u16 letoh_u16
#define ztoh_u32 letoh_u32
#define ztoh_u64 letoh_u64
// Use for GUI and AM protocol fields (host <-> pc)
#define htop_u16 htole_u16
#define htop_u32 htole_u32
#define htop_u64 htole_u64
#define ptoh_u16 letoh_u16
#define ptoh_u32 letoh_u32
#define ptoh_u64 letoh_u64
/**************************************************/
/**************************************************/

#endif //TOSH_AVRHARDWARE_H_
// eof avrhardware.h
