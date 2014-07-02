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
**  NetworkMgmt.h
**  
**  eZeeNet framework: network management module header file
*******************************************************************************/

#ifndef _NETWORK_MGMT_
#define _NETWORK_MGMT_

#include "stack.h"
#include "ParamServer.h"
#include "apptimer.h"

#define AUTONETWORK_DELAY_UNIT 1000ul // 1s

result_t NETWORK_MANAGER_Init(void);
bool     NETWORK_MANAGER_join(void);
uint8_t  NETWORK_getNwkState(void);
result_t NETWORK_MANAGER_checkParent(void);
void NETWORK_MANAGER_startAutoNet(void);
void NETWORK_MANAGER_stopAutoNet(void);
void stackStartDone(void);
bool stopNetwork(void);

typedef enum _NetworkManagerState_t_
{
  NO_STATE,
  STACK_INITING,
  STACK_INITED,
  STACK_STARTING,
  STACK_STARTED,
  NETWORK_OPENING,
  NETWORK_OPENED,
  NETWORK_STARTING,
  NETWORK_JOINED,
  NETWORK_FROZEN,
  NETWORK_LOST,
  NETWORK_STOPPING,
  NETWORK_STOPPED,
  STACK_STOPPING,
  STACK_STOPED
} NetworkManagerState_t;

typedef enum _CommandState_t_
{
  NO_COMMAND,
  COMMAND_PROCESSING
} CommandState_t;

#endif//_NETWORK_MGMT_

// eof NetworkMgmt.h
