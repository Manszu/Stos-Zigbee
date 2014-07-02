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
**  debugemu.h
**  
**  debug macros for eZeeNet framework 
*******************************************************************************/
#ifndef _DEBUGEMU_H_DEFINED_
#define _DEBUGEMU_H_DEFINED_

// #define DEBUG_EMU_SLEEPTIMER
// #define DEBUG_DONT_GO_TO_SLEEP
// #define DEBUG_SIGNAL_WAKEUP 
// #define DEBUG_GPIO_0
// #define DEBUG_NO_ZDOM_SLEEP

#define DEBUG_SLEEP

#ifdef DEBUG_SIGNAL_WAKEUP
#define DEBUG_SHOW_AWAKE gpio_setState (DEBUG_SIGNAL_WAKEUP, 1)
#define DEBUG_SHOW_SLEEP gpio_setState (DEBUG_SIGNAL_WAKEUP, 0)
#else
#define DEBUG_SHOW_AWAKE
#define DEBUG_SHOW_SLEEP
#endif

#if 0
#define DEBUG_SLEEP_FIRED gpio_setState (GPIO_1,1)
#define DEBUG_SLEEP_START gpio_setState (GPIO_2,1)
#else 
#define DEBUG_SLEEP_FIRED
#define DEBUG_SLEEP_START
#endif

#define TRAP() { /*for(;;);*/ }

#define ASSERT_FW_NULL_CALLBACK()          TRAP()
#define ASSERT_FW_UNKNOWN_DST_ENDPOINT()   TRAP()
#define ASSERT_FW_SBUF_ERROR()             TRAP()
#define ASSERT_FW_APSDE_OPEN_FAILED()      TRAP()
#define ASSERT_FW_APSDE_RXCONTROL_FAILED() TRAP()

#endif  

// eof debugemu.h

