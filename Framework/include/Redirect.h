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
**  Redirect.h
**  
**  eZeeNet framework: logical addressing module header file
*******************************************************************************/

#ifndef _REDIRECT_H_
#define _REDIRECT_H_

#include "stack.h"

#define COMMAND_ID_ADD_NODE              1
#define COMMAND_ID_ADD_NODE_RSP_OK       2
#define COMMAND_ID_ADD_NODE_RSP_ERROR    3
#define COMMAND_ID_DELETE_NODE           4
#define COMMAND_ID_DELETE_NODE_RSP_OK    5
#define COMMAND_ID_DELETE_NODE_RSP_ERROR 6
#define COMMAND_ID_NODE_ID_REQ           7
#define COMMAND_ID_NODE_FOUND_RSP        8
#define COMMAND_ID_NODE_NOT_FOUND_RSP    9
#define COMMAND_ID_REDIRECT              10
#define COMMAND_ID_DATA                  11
#define COMMAND_ID_HEARTBEAT             12
#define NODE_FOUND_RSP_TIMEOUT           2000

typedef uint16_t NodeId_t; 

result_t sendAddressRequest (NodeId_t node_id, int arq);
result_t sendAddressInfo (NodeId_t node_id, NWKAddr_t nwk_addr);
result_t Redirect_Init(void);
void Redirect_Start(void);
void dataTransferDone(void);
result_t APSDE_Control_Indication (APSIndication_t* ind);
bool isRedirectIdle(void);

typedef enum
{
  DTL_IDLE,
  DTL_IDREQ_SENT,
  DTL_RESP_FOUND,
  DTL_RESP_NOT_FOUND,
  DTL_WAITING_FOR_RESP,
  DTL_DATA_SENT
} DTLState_t; // State for data transfer by logical address.

#endif//_REDIRECT_H_

// eof Redirect.h
