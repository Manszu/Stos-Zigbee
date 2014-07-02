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
**  frameint.h
**  
**  eZeeNet framework: internal control funtions header for framework.c
*******************************************************************************/
#ifndef _FRAMEINT_H_DEFINED_
#define _FRAMEINT_H_DEFINED_

#define ACTIVE_UNIT         10ul
#define SLEEP_UNIT          100ul
#define SYNC_UNIT           100ul   
#define DTR_POLL_TIME       (SLEEP_UNIT * 10)
#define POLL_PERIOD         20ul
#define LIFETIME_FACTOR     4
#define MSEC_IN_MINUTE      60000ul

#define DELAY_BEFORE_CALIBR 1000 //Delay before calibration in msec

typedef enum
{
  SYNC_NO_DATA          = 0,
  SYNC_REQUESTED        = 1,
  SYNC_DATA_MAY_PRESENT = 2,
} FWSyncState_t;

extern uint8_t apiBlocked;
extern uint8_t pendingConfirm;
extern int userLoopTimerDesc;
extern volatile uint8_t staticNetworkOpened;
extern int calibrTimerDesc;
extern uint32_t lastCalibrTime;

void initPowerManagement ();
void startPowerManagement ();
void stopPowerManagement ();
void rxBufferEmpty();
void userLoopTimerFired();
void calibrTimerFired();

#endif

// eof frameint.h
