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
  msg_srv_fxns.h
  
  Message server interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef MSG_SRV_FXNS_H_
#define MSG_SRV_FXNS_H_

#include "com_types.h"
#include "buff_descr.h"

struct MsgSrv;

typedef Uint8 MsgId;

typedef enum
{
  MSG_PARAM_ID_INVALID,
  MSG_PARAM_ID_UINT8,
  MSG_PARAM_ID_ENUM8, // For special types, ENUMs that have 8 bits in length in messages.
  MSG_PARAM_ID_UINT16,
  MSG_PARAM_ID_UINT24,
  MSG_PARAM_ID_UINT32,
  MSG_PARAM_ID_UINT64,
  MSG_PARAM_ID_UINT128,
  MSG_PARAM_ID_AUINT8,  // Array of UINT8.
} MsgParamId;

typedef enum 
{
  MSG_PARAM_FLAG_DEFAULT = 0x00,
  MSG_PARAM_FLAG_SKIP_SCAN = 0x01, // Parameter is skipped when scan. Argument list should not contain
                                   // argument for this parameter.
} MsgParamFlag;

typedef struct
{
  MsgParamId id;
  union // Special fild. Makes sence only for some parameters.
  {
    Uint8 array; // Array length (for AUIN8). 
    Uint8 enum8; // Real ENUM length (for ENUM8).
  } length;
  MsgParamFlag flags;
} MsgParamDescr;

typedef struct
{
  struct MsgSrv* pMsgSrv;
  
  Bool (*isReadyToScanf)(struct MsgSrv* pMsgSrv, MsgId* pMsgId);
  void (*scanf)(struct MsgSrv* pMsgSrv, const MsgParamDescr* format, Uint8** pParams, Uint8 paramMax);
  void (*startPrintf)(struct MsgSrv* pMsgSrv, MsgId msgId, BuffDescr* pBuffDescr);
  void (*printf)(struct MsgSrv* pMsgSrv, const MsgParamDescr* format, Uint8** pParams, Uint8 paramMax);
  void (*endPrintf)(struct MsgSrv* pMsgSrv);
} MsgSrvFxns;

#endif

// eof msg_srv_fxns.h
