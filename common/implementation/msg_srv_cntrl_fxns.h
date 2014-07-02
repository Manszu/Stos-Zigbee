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
  msg_srv_cntrl_fxns.h
  
  Message server control interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_srv_fxns.h"
#include "msg_channel_fxns.h"

#ifndef MSG_SRV_CNTRL_FXNS_H_
#define MSG_SRV_CNTRL_FXNS_H_

struct MsgSrv
{
  MsgChannelFxns msgChannelFxns;
  
  enum
  {
    MSG_SRV_SCAN_STATE_INVALID,
    MSG_SRV_SCAN_STATE_IDLE,
    MSG_SRV_SCAN_STATE_READY,
    MSG_SRV_SCAN_STATE_SCAN,
  } scanState;
  BuffDescr* pScanBuffDescr;
  Uint8* pScanBuff;
  
  enum
  {
    MSG_SRV_PRINT_STATE_INVALID,
    MSG_SRV_PRINT_STATE_IDLE,
    MSG_SRV_PRINT_STATE_PRINT,
  } printState;
  BuffDescr* pPrintBuffDescr;
  Uint8* pPrintBuff;
};

/*******************************************************************************
  Description: provides message server interface reference.
    
  Parameters: pMsgSrvFxns - message server interface reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_getFxns(MsgSrvFxns*);

/*******************************************************************************
  Description: inits message server.
    
  Parameters: pMsgSrv - message server reference.
              pMsgChannelFxns - message channel reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_init(struct MsgSrv*, const MsgChannelFxns*);

/*******************************************************************************
  Description: message server control loop. Reads incoming messages from 
               message channel, parses them in reply of external request.
               Writes outgoing messages to message channel.
    
  Parameters: pMsgSrv - message server reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_process(struct MsgSrv*);

#endif

// eof msg_srv_cntrl_fxns.h
