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
  msg_srv.h
  
  Message server implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_srv_cntrl_fxns.h"
#include "com_assert.h"
#include "nbo.h"
#include <string.h>

static Uint8 getParamLength(const MsgParamDescr*); // Returns parameter length.

/*******************************************************************************
  Description:  returns parameter length.
    
  Parameters: pParamDescr - parameter descriptor reference.
  
  Returns: parameter length.
*******************************************************************************/
static Uint8 getParamLength(const MsgParamDescr* pParamDescr)
{
  Uint8 paramLength = 0;
  
  switch (pParamDescr->id)
  {
    case MSG_PARAM_ID_UINT8:
    case MSG_PARAM_ID_ENUM8:
         paramLength = 1;
         break;
           
    case MSG_PARAM_ID_UINT16:
         paramLength = 2;
         break;
    
    case MSG_PARAM_ID_UINT24:
         paramLength = 3;
         break;
           
    case MSG_PARAM_ID_UINT32:
         paramLength = 4;
         break;
           
    case MSG_PARAM_ID_UINT64:
         paramLength = 8;
         break;
           
    case MSG_PARAM_ID_AUINT8:
         paramLength = pParamDescr->length.array;
         break;
           
    default:
         ASSERT(false);
  }
  return paramLength;
}

/*******************************************************************************
  Description: tells that caller now begins to scan the message.
    
  Parameters: pMsgSrv - message server reference.
              msgId   - ID of received message. Valid only if message is ready
                        to scan.
  
  Returns: true, if message is ready to scan and false, otherwise.
*******************************************************************************/
static Bool isReadyToScanf(struct MsgSrv* pMsgSrv, MsgId* pMsgId)
{
  Bool res = false;
  
  ASSERT((0 != pMsgSrv) && ((MSG_SRV_SCAN_STATE_IDLE == pMsgSrv->scanState) ||
                            (MSG_SRV_SCAN_STATE_READY == pMsgSrv->scanState)));
  if (MSG_SRV_SCAN_STATE_IDLE == pMsgSrv->scanState)
  {
    // Message has not been read yet.
    pMsgSrv->pScanBuffDescr = pMsgSrv->msgChannelFxns.read(pMsgSrv->msgChannelFxns.pMsgChannel);
    if (0 != pMsgSrv->pScanBuffDescr)
    {
      pMsgSrv->pScanBuff = pMsgSrv->pScanBuffDescr->data;
      *pMsgId = *pMsgSrv->pScanBuff;
      pMsgSrv->scanState = MSG_SRV_SCAN_STATE_READY;
      res = true; 
    }
  }
  else
  {
    // Message has been read in the past.
    *pMsgId = *pMsgSrv->pScanBuff;
    res = true;
  }
  return res;
}

/*******************************************************************************
  Description: scans part (or entire) of message. Due to MSG_PARAM_SKIP_FLAG
               format parameter can contain more descriptors than pParams 
               parameter can contain pointer to parameters.  
    
  Parameters: pMsgSrv - message server reference.
              format - format string, describes parameters to scan.
              pParams - pointer to array with pointers to parameters to scan.
              paramMax - number of parameters to scan.
  
  Returns: nothing.
*******************************************************************************/
static void lscanf(struct MsgSrv* pMsgSrv,
                   const MsgParamDescr* format,
                   Uint8** pParams,
                   Uint8 paramMax)
{
  Uint8 formatInd = 0;
  Uint8 paramInd = 0;
  
  ASSERT((0 != pMsgSrv) && (0 != format) && (0 != pParams) && (0 != paramMax) &&
         ((MSG_SRV_SCAN_STATE_READY == pMsgSrv->scanState) ||
          (MSG_SRV_SCAN_STATE_SCAN == pMsgSrv->scanState)));
          
  if (MSG_SRV_SCAN_STATE_READY == pMsgSrv->scanState)
  {
    pMsgSrv->scanState = MSG_SRV_SCAN_STATE_SCAN;
    pMsgSrv->pScanBuff += sizeof(MsgId);
  }
  while (paramInd < paramMax)
  {
    Uint8 paramLength = getParamLength(&format[formatInd]);
    
    if (0 == (MSG_PARAM_FLAG_SKIP_SCAN & format[formatInd].flags))
    {
      // Parameter is not skipped.
      if (MSG_PARAM_ID_ENUM8 == format[formatInd].id)
      {
      	// First special case. ENUM 8 bits in length.
      	switch (format[formatInd].length.enum8)
      	{
      	  case 1:
   	       	   *pParams[paramInd] = *pMsgSrv->pScanBuff;
      	       break;
      	  
      	  case 2:
      	       *(Uint16*)pParams[paramInd] = *pMsgSrv->pScanBuff;
      	       break;
      	  
      	  case 4:
   	       	   *(Uint32*)pParams[paramInd] = *pMsgSrv->pScanBuff;
      	       break;
      	  
      	  default:	
      	       // Too long ENUM.
      	       ASSERT(false);
      	       break;
      	}
      }
      else
      {
      	if (MSG_PARAM_ID_AUINT8 == format[formatInd].id)
      	  // Second special case. Array of UINT8 is in unchanged byte order.
      	  memcpy(pParams[paramInd], pMsgSrv->pScanBuff, paramLength);
      	else
      	  // All other (nominal) cases.
      	  nbo_letoh(pParams[paramInd], pMsgSrv->pScanBuff, paramLength);
      }
      paramInd++;
    }
    pMsgSrv->pScanBuff += paramLength;
    if (pMsgSrv->pScanBuff > pMsgSrv->pScanBuffDescr->tail)
      ASSERT(false);
    formatInd++;
  };
  if (pMsgSrv->pScanBuff == pMsgSrv->pScanBuffDescr->tail)
  {
    pMsgSrv->scanState = MSG_SRV_SCAN_STATE_IDLE;
    pMsgSrv->pScanBuffDescr->busy = false;
  }
}

/*******************************************************************************
  Description: starts printing new message.
    
  Parameters: pMsgSrv    - message server reference.
              msgId      - message ID to start printing.
              pBuffDescr - buffer descriptor reference. The buffer descriptor
                           describes buffer allocated by caller. Buffer is used
                           to transmit the message.
  
  Returns: nothing.
*******************************************************************************/
static void startPrintf(struct MsgSrv* pMsgSrv, MsgId msgId, BuffDescr* pBuffDescr)
{
  ASSERT((0 != pMsgSrv) && (0 != pBuffDescr) &&
         (MSG_SRV_PRINT_STATE_IDLE == pMsgSrv->printState));
  // Save buffer descriptor reference.
  pMsgSrv->pPrintBuffDescr = pBuffDescr;
  // Init buffer pointer.
  pMsgSrv->pPrintBuff = pMsgSrv->pPrintBuffDescr->data;
  // Save message ID.
  buffDescr_put(pMsgSrv->pPrintBuffDescr, sizeof(msgId));
  nbo_htole(pMsgSrv->pPrintBuff, (Uint8*)&msgId, sizeof(msgId));
  pMsgSrv->pPrintBuff += sizeof(msgId);
  pMsgSrv->printState = MSG_SRV_PRINT_STATE_PRINT;
}

/*******************************************************************************
  Description: prints part (or entire) of message.
    
  Parameters: pMsgSrv - message server reference.
              format - format string, describes parameters to print.
              pParams - pointer to array with pointers to parameters to print.
              paramMax - number of parameters to print.
  
  Returns: nothing.
*******************************************************************************/
static void lprintf(struct MsgSrv* pMsgSrv,
                    const MsgParamDescr* format,
                    Uint8** pParams,
                    Uint8 paramMax)
{
  Uint8 paramInd;
  
  ASSERT((0 != pMsgSrv) && (0 != format) && (0 != pParams) && (0 != paramMax));
  
  for (paramInd = 0; paramInd < paramMax; paramInd++)
  {
    Uint8 paramLength = getParamLength(&format[paramInd]);
    
    if (paramLength)
    {
      buffDescr_put(pMsgSrv->pPrintBuffDescr, paramLength);
      if (MSG_PARAM_ID_ENUM8 == format[paramInd].id)
      {
      	// First special case. ENUM 8 bits in length.
      	switch (format[paramInd].length.enum8)
      	{
      	  case 1:
               *pMsgSrv->pPrintBuff = *pParams[paramInd]; 
      	       break;
      	  
      	  case 2:
               *pMsgSrv->pPrintBuff = *(Uint16*)pParams[paramInd];
      	       break;
      	  
      	  case 4:
               *pMsgSrv->pPrintBuff = *(Uint32*)pParams[paramInd];
      	       break;
      	  
      	  default:	
      	       // Too long ENUM.
      	       ASSERT(false);
      	       break;
      	}
      }
      else
      {
      	if (MSG_PARAM_ID_AUINT8 == format[paramInd].id)
      	  // Second special case. Array of UINT8 is in unchanged byte order.
      	  memcpy(pMsgSrv->pPrintBuff, pParams[paramInd], paramLength);
      	else
      	  // All other (nominal) cases.
      	  nbo_htole(pMsgSrv->pPrintBuff, pParams[paramInd], paramLength);
      }
      pMsgSrv->pPrintBuff += paramLength;
    }
  }
}

/*******************************************************************************
  Description: writes printed message to message channel.
    
  Parameters: pMsgSrv - message server reference.
  
  Returns: nothing.
*******************************************************************************/
static void endPrintf(struct MsgSrv* pMsgSrv)
{
  // Write message to message channel.
  pMsgSrv->msgChannelFxns.write(pMsgSrv->msgChannelFxns.pMsgChannel, 
                                pMsgSrv->pPrintBuffDescr);
  // Get ready to print the next message.
  pMsgSrv->printState = MSG_SRV_PRINT_STATE_IDLE;
}

/*******************************************************************************
  Description: provides message server interface reference.
    
  Parameters: pMsgSrvFxns - message server interface reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_getFxns(MsgSrvFxns* pMsgSrvFxns)
{
  pMsgSrvFxns->isReadyToScanf = isReadyToScanf;
  pMsgSrvFxns->scanf = lscanf;
  pMsgSrvFxns->startPrintf = startPrintf;
  pMsgSrvFxns->printf = lprintf;
  pMsgSrvFxns->endPrintf = endPrintf;
}

/*******************************************************************************
  Description: inits message server.
    
  Parameters: pMsgSrv - message server reference.
              pMsgChannelFxns - message channel reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_init(struct MsgSrv* pMsgSrv, const MsgChannelFxns* pMsgChannelFxns)
{
  ASSERT((0 != pMsgSrv) && (0 != pMsgChannelFxns));
  pMsgSrv->msgChannelFxns = *pMsgChannelFxns;
  pMsgSrv->printState = MSG_SRV_PRINT_STATE_IDLE;
  pMsgSrv->scanState = MSG_SRV_SCAN_STATE_IDLE;
}

/*******************************************************************************
  Description: message server control loop. Reads incoming messages from 
               message channel, parses them in reply of external request.
               Writes outgoing messages to message channel.
    
  Parameters: pMsgSrv - message server reference.
  
  Returns: nothing.
*******************************************************************************/
void msgSrv_process(struct MsgSrv* pMsgSrv)
{
  
}

// eof msg_srv.c
