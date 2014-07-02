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
  mac_srv.c
  
  MAC server implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "mac_srv_cntrl_fxns.h"
#include "com_assert.h"

enum // Various constants.
{
  ACL_ENTRY = 0x08,
  
  PAN_DESCR_PARAM_MAX                = 11, // Number of parameters in PAN descriptor.
  PAN_DESCR_COORD_ADDR_MODE_PARAM_IND = 0, // Index of coordinator address mode parameter in PAN descriptor.
  PAN_DESCR_COORD_ADDR_PARAM_IND      = 2, // Index of coordinator address parameter in PAN descriptor.

  DATA_IND_PARAM_MAX               = 11, // Number of parameters in data indication.
  DATA_IND_SRC_ADDR_MODE_PARAM_IND = 0, // Index of source address mode parameter in data indication.
  DATA_IND_SRC_ADDR_PARAM_IND      = 2, // Index of source address parameter in data indication.
  DATA_IND_DST_ADDR_MODE_PARAM_IND = 3, // Index of destination address mode parameter in data indication.
  DATA_IND_DST_ADDR_PARAM_IND      = 5, // Index of destination address parameter in data indication.

  COMM_STATUS_IND_PARAM_MAX               = 6, // Number of parameters in comm status indication.
  COMM_STATUS_IND_SRC_ADDR_MODE_PARAM_IND = 1, // Index of source address mode parameter in comm status indication.
  COMM_STATUS_IND_SRC_ADDR_PARAM_IND      = 2, // Index of source address parameter in comm status indication.
  COMM_STATUS_IND_DST_ADDR_MODE_PARAM_IND = 3, // Index of destination address mode parameter in comm status indication.
  COMM_STATUS_IND_DST_ADDR_PARAM_IND      = 4, // Index of destination address parameter in comm status indication.
  
  BEACON_PEND_ADDR_LIST_MAX_SIZE = 7,
};

enum // MAC server messages codes.
{
  MAC_MSG_ID_DATA_REQ          = 0x40,
  MAC_MSG_ID_DATA_CONF         = 0x41,
  MAC_MSG_ID_DATA_IND          = 0x42,
  MAC_MSG_ID_PURGE_REQ         = 0x43,
  MAC_MSG_ID_PURGE_CONF        = 0x44,
  MAC_MSG_ID_ASSOC_REQ         = 0x45,
  MAC_MSG_ID_ASSOC_CONF        = 0x46,
  MAC_MSG_ID_ASSOC_IND         = 0x47,
  MAC_MSG_ID_ASSOC_RESP        = 0x48,
  MAC_MSG_ID_DISASSOC_REQ      = 0x49,
  MAC_MSG_ID_DISASSOC_CONF     = 0x4A,
  MAC_MSG_ID_DISASSOC_IND      = 0x4B,
  MAC_MSG_ID_BEACON_NOTIFY_IND = 0x4C,
  MAC_MSG_ID_GET_REQ           = 0x4D,
  MAC_MSG_ID_GET_CONF          = 0x4E,
  MAC_MSG_ID_GTS_REQ           = 0x4F,
  MAC_MSG_ID_GTS_CONF          = 0x50,
  MAC_MSG_ID_GTS_IND           = 0x51,
  MAC_MSG_ID_ORPHAN_IND        = 0x52,
  MAC_MSG_ID_ORPHAN_RESP       = 0x53,
  MAC_MSG_ID_RESET_REQ         = 0x54,
  MAC_MSG_ID_RESET_CONF        = 0x55,
  MAC_MSG_ID_RX_ENABLE_REQ     = 0x56,
  MAC_MSG_ID_RX_ENABLE_CONF    = 0x57,
  MAC_MSG_ID_SCAN_REQ          = 0x58,
  MAC_MSG_ID_SCAN_CONF         = 0x59,
  MAC_MSG_ID_COMM_STATUS_IND   = 0x5A,
  MAC_MSG_ID_SET_REQ           = 0x5B,
  MAC_MSG_ID_SET_CONF          = 0x5C,
  MAC_MSG_ID_START_REQ         = 0x5D,
  MAC_MSG_ID_START_CONF        = 0x5E,
  MAC_MSG_ID_SYNC_REQ          = 0x5F,
  MAC_MSG_ID_SYNC_LOSS_IND     = 0x60,
  MAC_MSG_ID_POLL_REQ          = 0x61,
  MAC_MSG_ID_POLL_CONF         = 0x62,
  MAC_MSG_ID_POLL_IND          = 0x63,
};

// Puts command to MAC.
static void putCmd(struct MacSrv*, MsgId, MacSrvCmdPoolItem*);

// Scans data request message.
static void scanDataReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans purge request message.
static void scanPurgeReq(const MsgSrvFxns*, struct MacCmdDescr*);

// Scans associate request message.
static void scanAssocReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans assiciate response message.
static void scanAssocResp(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans disassociate request message.
static void scanDisassocReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans get request message.
static void scanGetReq(const MsgSrvFxns*, struct MacCmdDescr*);

// Scans orphan response message.
static void scanOrphanResp(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans reset request message.
static void scanResetReq(const MsgSrvFxns*, struct MacCmdDescr*);

// Scans rx enable request message.
static void scanRxEnableReq(const MsgSrvFxns*, struct MacCmdDescr*);

// Scans scan request message.
static void scanScanReq(struct MacSrv*, MacSrvCmdPoolItem*);

// Scans set request message.
static void scanSetReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans start request message.
static void scanStartReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Scans poll request message.
static void scanPollReq(const MsgSrvFxns*, struct MacCmdDescr*, BuffDescr*);

// Finds free buffer for event printing.
static BuffDescr* findFreeEventBuff(BuffDescr*, Uint8);

// Prints data confirm.
static Bool printDataConf(struct MacSrv*, struct MacCmdDescr*);

// Prints purge confirm.
static Bool printPurgeConf(struct MacSrv*, struct MacCmdDescr*);

// Prints association confirm.
static Bool printAssocConf(struct MacSrv*, struct MacCmdDescr*);

// Prints disassociation confirm.
static Bool printDisassocConf(struct MacSrv*, struct MacCmdDescr*);

// Prints get confirm.
static Bool printGetConf(struct MacSrv*, struct MacCmdDescr*);

// Prints set confirm.
static Bool printSetConf(struct MacSrv*, struct MacCmdDescr*);

// Prints reset confirm.
static Bool printResetConf(struct MacSrv*, struct MacCmdDescr*);

// Prints rx enable confirm.
static Bool printRxEnableConf(struct MacSrv*, struct MacCmdDescr*);

// Prints start confirm.
static Bool printStartConf(struct MacSrv*, struct MacCmdDescr*);

// Prints scan confirm.
static Bool printScanConf(struct MacSrv*, struct MacCmdDescr*);

// Prints poll confirm.
static Bool printPollConf(struct MacSrv*, struct MacCmdDescr*);

// Prints comm status indication.
static Bool printCommStatusInd(struct MacSrv*, struct MacCmdDescr*);

// Prints data indication.
static Bool printDataInd(struct MacSrv*, struct MacIndDescr*);

// Prints associate indication.
static Bool printAssocInd(struct MacSrv*, struct MacIndDescr*);

// Prints disassociate indication.
static Bool printDisassocInd(struct MacSrv*, struct MacIndDescr*);

// Prints orphan indication.
static Bool printOrphanInd(struct MacSrv*, struct MacIndDescr*);

// Prints poll indication.
static Bool printPollInd(struct MacSrv*, struct MacIndDescr*);

// Prints beacon notify indication.
static Bool printBeaconNotifyInd(struct MacSrv*, struct MacIndDescr*);

/*******************************************************************************
  Description: prints data confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - data request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void dataConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printDataConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints purge confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - purge request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void purgeConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printPurgeConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints associate confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void assocConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printAssocConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints disassociate confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void disassocConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printDisassocConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints set confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void setConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printSetConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints comm status indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void commStatusInd(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printCommStatusInd(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints reset confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void resetConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printResetConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints rx enable confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void rxEnableConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printRxEnableConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints scan confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void scanConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printScanConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints start confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void startConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printStartConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}

/*******************************************************************************
  Description: prints poll confirm.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void pollConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  if (!printPollConf(pMacSrv, pCmdDescr))
    list_queueTail(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
}
  
/*******************************************************************************
  Description: prints data indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void dataInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printDataInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: prints associate indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void assocInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printAssocInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: prints disassociate indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void disassocInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printDisassocInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: prints orphan indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void orphanInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printOrphanInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: prints poll indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void pollInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printPollInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: prints beacon notify indication.

  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - associate request descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void beaconNotifyInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  if (!printBeaconNotifyInd(pMacSrv, pIndDescr))
    list_queueTail(&pMacSrv->indPendList, (ListItem*)pIndDescr);
}

/*******************************************************************************
  Description: returns reference to MAC event interface.

  Parameters: pMacEventFxns - reference to MAC event interface.
  
  Returns: nothing.
*******************************************************************************/
void macSrv_getFxns(MacEventFxns* pMacEventFxns)
{
  pMacEventFxns->dataConf = (void (*)(struct Nwk*, struct MacCmdDescr*))dataConf;
  pMacEventFxns->purgeConf = (void (*)(struct Nwk*, struct MacCmdDescr*))purgeConf;
  pMacEventFxns->assocConf = (void (*)(struct Nwk*, struct MacCmdDescr*))assocConf;
  pMacEventFxns->disassocConf = (void (*)(struct Nwk*, struct MacCmdDescr*))disassocConf;
  pMacEventFxns->setConf = (void (*)(struct Nwk*, struct MacCmdDescr*))setConf;
  pMacEventFxns->commStatusInd = (void (*)(struct Nwk*, struct MacCmdDescr*))commStatusInd;
  pMacEventFxns->resetConf = (void (*)(struct Nwk*, struct MacCmdDescr*))resetConf;
  pMacEventFxns->rxEnableConf = (void (*)(struct Nwk*, struct MacCmdDescr*))rxEnableConf;
  pMacEventFxns->scanConf = (void (*)(struct Nwk*, struct MacCmdDescr*))scanConf;
  pMacEventFxns->startConf = (void (*)(struct Nwk*, struct MacCmdDescr*))startConf;
  pMacEventFxns->pollConf = (void (*)(struct Nwk*, struct MacCmdDescr*))pollConf;
  
  pMacEventFxns->dataInd = (void (*)(struct Nwk*, struct MacIndDescr*))dataInd;
  pMacEventFxns->assocInd = (void (*)(struct Nwk*, struct MacIndDescr*))assocInd;
  pMacEventFxns->disassocInd = (void (*)(struct Nwk*, struct MacIndDescr*))disassocInd;
  pMacEventFxns->orphanInd = (void (*)(struct Nwk*, struct MacIndDescr*))orphanInd;
  pMacEventFxns->pollInd = (void (*)(struct Nwk*, struct MacIndDescr*))pollInd;
  pMacEventFxns->beaconNotifyInd = (void (*)(struct Nwk*, struct MacIndDescr*))beaconNotifyInd;
}

/*******************************************************************************
  Description: inits MAC server.

  Parameters: pMacSrv       - MAC server reference.
              pMsgSrvFxns   - message server interface reference.
              pMacCmdFxns   - MAC command interface reference.
              pLedsFxns           - LEDs interface reference.
              cmdPoolSize - number of MAC command descriptors. It must be
                            initialized with MAC_SRV_CMD_POOL_SIZE macros.
              eventBuffSize - size of MAC event buffer. It must be initialized
                              with MAC_SRV_EVENT_BUFF_SIZE macros.
              eventBuffMax  - number of MAC event buffer.  It must be initialized
                              with MAC_SRV_EVENT_BUFF_MAX macros.
              beaconBuffSize - size of beacon buffer. It must be initialized
                               with MAC_BEACON_BUFF_SIZE macros.
              scanEnergyMax  - number of energy values in ED scan. It must be
                               initialized with MAC_SRV_SCAN_ENERGY_MAX macros.
              scanPanDescrMax - number of PAN descriptors in active or passive scan.
                                It must be initialized with
                                MAC_SRV_SCAN_PAN_DESCR_MAX macros.
  
  Returns: nothing.
*******************************************************************************/
void macSrv_init(struct MacSrv* pMacSrv,
                const MsgSrvFxns* pMsgSrvFxns,
                const MacCmdFxns* pMacCmdFxns,
                const LedsFxns* pLedsFxns,
                Uint8 cmdPoolSize,
                Uint8 eventBuffSize,
                Uint8 eventBuffMax,
                Uint8 beaconBuffSize,
                Uint8 scanEnergyMax,
                Uint8 scanPanDescrMax)
{
  // Init used interfaces.
  pMacSrv->msgSrvFxns = *pMsgSrvFxns;
  pMacSrv->macCmdFxns = *pMacCmdFxns;
  pMacSrv->ledsFxns = *pLedsFxns;
  
  // Init beacon buffer stuff.
  pMacSrv->beaconBuffSize = beaconBuffSize;
  buffDescr_init(&pMacSrv->beaconBuffDescr,
                 pMacSrv->beaconBuff,
                 beaconBuffSize);
  
  // Init command pool.
  pMacSrv->cmdPoolSize = cmdPoolSize;
  {
    Uint8 cmdPoolItemInd;
    
    for (cmdPoolItemInd = 0; cmdPoolItemInd < cmdPoolSize; cmdPoolItemInd++)
    {
      buffDescr_init(&pMacSrv->cmdPool[cmdPoolItemInd].buffDescr, 
                     pMacSrv->cmdPool[cmdPoolItemInd].buff,
                     PHY_FRAME_MAX_SIZE);
      pMacSrv->cmdPool[cmdPoolItemInd].busy = false;
    }
  }
  
  // Init event buffers parameters, event buffer descriptors.
  pMacSrv->eventBuffSize = eventBuffSize;
  pMacSrv->eventBuffMax = eventBuffMax;
  {
    Uint8 buffInd;
    
    for (buffInd = 0; buffInd < eventBuffMax; buffInd++)
    {
      buffDescr_init(&pMacSrv->eventBuffDescr[buffInd],
                     pMacSrv->eventBuff + eventBuffSize * buffInd,
                     eventBuffSize);
    }
  }
  
  // Init scan results limits.
  pMacSrv->scanEnergyMax = scanEnergyMax;
  pMacSrv->scanPanDescrMax = scanPanDescrMax;
  pMacSrv->scanBusy = false;
  
  // Init confirms and indications pending lists. 
  list_init(&pMacSrv->confPendList);
  list_init(&pMacSrv->indPendList);
}

/*******************************************************************************
  Description: gets events from MAC and prints them to message server.
               Gets commands from message server and passes them to MAC.
    
  Parameters: pMacSrv - MAC server reference.
  
  Returns: nothing.
*******************************************************************************/
void macSrv_process(struct MacSrv* pMacSrv)
{
  // MAC command processing.
  {
    MsgId msgId;
  
    if (pMacSrv->msgSrvFxns.isReadyToScanf(pMacSrv->msgSrvFxns.pMsgSrv, &msgId))
    {
      // New command is available.
      Uint8 cmdItemInd;
    
      // Find free command pool item.
      for (cmdItemInd = 0; cmdItemInd < pMacSrv->cmdPoolSize; cmdItemInd++)
      {
        if (!pMacSrv->cmdPool[cmdItemInd].busy)
        {
          // There is free command descriptor.
          if ((MAC_MSG_ID_SCAN_REQ != msgId) || !pMacSrv->scanBusy )
          {
            // Only one scan request is allowed.
            MacSrvCmdPoolItem* pCmdPoolItem = &pMacSrv->cmdPool[cmdItemInd];
                
            pCmdPoolItem->cmdDescr.handle = cmdItemInd;
            pCmdPoolItem->busy = true;
            putCmd(pMacSrv, msgId, pCmdPoolItem);
            break;
          }
        }
      }
    } 
  }
 
  // Try printing confirm from pending list.
  {
    struct MacCmdDescr* pCmdDescr;
    
    pCmdDescr = (struct MacCmdDescr*)list_peekHead(&pMacSrv->confPendList);
    if (pCmdDescr)
    {
      Bool res = false;
      
      switch (pCmdDescr->cmdId)
      {
        case MAC_CMD_ID_DATA_REQ:
             res = printDataConf(pMacSrv, pCmdDescr);
             break;
          
        case MAC_CMD_ID_PURGE_REQ:
             res = printPurgeConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_ASSOC_REQ:
             res = printAssocConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_ASSOC_RESP:
        case MAC_CMD_ID_ORPHAN_RESP:
             res = printCommStatusInd(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_DISASSOC_REQ:
             res = printDisassocConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_GET_REQ:
             res = printGetConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_SET_REQ:
             res = printSetConf(pMacSrv, pCmdDescr);
             break;
                     
        case MAC_CMD_ID_RESET_REQ:
             res = printResetConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_RX_ENABLE_REQ:
             res = printRxEnableConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_START_REQ:
             res = printStartConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_SCAN_REQ:
             res = printScanConf(pMacSrv, pCmdDescr);
             break;
             
        case MAC_CMD_ID_POLL_REQ:
             res = printPollConf(pMacSrv, pCmdDescr);
             break;
        
        default:
             ASSERT(false);
      }
      if (res)
        list_unlink(&pMacSrv->confPendList, (ListItem*)pCmdDescr);
    }
  }
  
  // Try printing MAC indication from pending list.
  {
    struct MacIndDescr* pIndDescr;
    
    pIndDescr = (struct MacIndDescr*)list_peekHead(&pMacSrv->indPendList);
    if (pIndDescr)
    {
      Bool res = false;
      
      switch (pIndDescr->indId)
      {
                     
        case MAC_IND_ID_DATA:
             res = printDataInd(pMacSrv, pIndDescr);
             break;
        
        case MAC_IND_ID_ASSOC:
             res = printAssocInd(pMacSrv, pIndDescr);
             break;
             
        case MAC_IND_ID_DISASSOC:
             res = printDisassocInd(pMacSrv, pIndDescr);
             break;

        case MAC_IND_ID_ORPHAN:
             res = printOrphanInd(pMacSrv, pIndDescr);
             break;

        case MAC_IND_ID_POLL:
             res = printPollInd(pMacSrv, pIndDescr);
             break;

        default:
             ASSERT(false);
      }
      if (res)
        list_unlink(&pMacSrv->indPendList, (ListItem*)pIndDescr);
    }
  }
}

/*******************************************************************************
  Description: scans and puts command to MAC.

  Parameters: pMacSrv      - MAC server reference.
              msgId        - parsed message ID.
              pCmdPoolItem - command pool item.
  
  Returns: nothing.
*******************************************************************************/
static void putCmd(struct MacSrv* pMacSrv, MsgId msgId, MacSrvCmdPoolItem* pCmdPoolItem)
{
  switch (msgId)
  {
    case MAC_MSG_ID_DATA_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_DATA_REQ;
         scanDataReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.dataReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
          
    case MAC_MSG_ID_PURGE_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_PURGE_REQ;
         scanPurgeReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr);
         pMacSrv->macCmdFxns.purgeReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_ASSOC_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_ASSOC_REQ;
         scanAssocReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.assocReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_ASSOC_RESP:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_ASSOC_RESP;
         scanAssocResp(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.assocResp(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_DISASSOC_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_DISASSOC_REQ;
         scanDisassocReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.disassocReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_ORPHAN_RESP:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_ORPHAN_RESP;
         scanOrphanResp(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.orphanResp(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_RESET_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_RESET_REQ;
         scanResetReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr);
         pMacSrv->macCmdFxns.resetReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_RX_ENABLE_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_RX_ENABLE_REQ;
         scanRxEnableReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr);
         pMacSrv->macCmdFxns.rxEnableReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_SCAN_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_SCAN_REQ;
         scanScanReq(pMacSrv, pCmdPoolItem);
         pMacSrv->macCmdFxns.scanReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_SET_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_SET_REQ;
         scanSetReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pMacSrv->beaconBuffDescr);
         pMacSrv->macCmdFxns.setReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
         
    case MAC_MSG_ID_GET_REQ:
         // Immediate return.
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_GET_REQ;
         scanGetReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr);
         pMacSrv->macCmdFxns.getReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         if (!printGetConf(pMacSrv, &pCmdPoolItem->cmdDescr))
           list_queueTail(&pMacSrv->confPendList, (ListItem*)&pCmdPoolItem->cmdDescr);
         break;     
             
    case MAC_MSG_ID_START_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_START_REQ;
         scanStartReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.startReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
    case MAC_MSG_ID_POLL_REQ:
         pCmdPoolItem->cmdDescr.cmdId = MAC_CMD_ID_POLL_REQ;
         scanPollReq(&pMacSrv->msgSrvFxns, &pCmdPoolItem->cmdDescr, &pCmdPoolItem->buffDescr);
         pMacSrv->macCmdFxns.pollReq(pMacSrv->macCmdFxns.pMac, &pCmdPoolItem->cmdDescr);
         break;
             
        default:
             ASSERT(false);
  }
}

/*******************************************************************************
  Description: scans data request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanDataReq(const MsgSrvFxns* pMsgSrvFxns,
                        struct MacCmdDescr* pMacCmdDescr,
                        BuffDescr* pPsduBuffDescr)
{
  // Scan source address information.
  {
    Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.dataReq.srcAddrMode,
                        (Uint8*)&pMacCmdDescr->params.cmd.dataReq.srcPanId};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16,                  {0}, MSG_PARAM_FLAG_DEFAULT}};
    
    // Scan up to not included source address.
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 2);
    
    // Scan source address.
    format[0].id = MSG_PARAM_ID_UINT16;
    if (MAC_ADDR_MODE_EXT == *pParams[0])
      format[0].id = MSG_PARAM_ID_UINT64;
    pParams[0] = (Uint8*)&pMacCmdDescr->params.cmd.dataReq.srcAddr;
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 1);
  }
  
  // Scan destination address information.
  {
    Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.dataReq.dstAddrMode,
                        (Uint8*)&pMacCmdDescr->params.cmd.dataReq.dstPanId};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16,                  {0}, MSG_PARAM_FLAG_DEFAULT}};
    
    // Scan up to not included destination address.
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 2);
    
    // Scan destination address.
    format[0].id = MSG_PARAM_ID_UINT16;
    if (MAC_ADDR_MODE_EXT == *pParams[0])
      format[0].id = MSG_PARAM_ID_UINT64;
    pParams[0] = (Uint8*)&pMacCmdDescr->params.cmd.dataReq.dstAddr;
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 1);
  }
  
  // Scan msdu length.
  {
    Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.dataReq.msduLength};
                      
    const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};
    
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 1);
  }
  
  // Scan msdu, msdu handle and transmission options.
  {
  	Uint8 msduLength = pMacCmdDescr->params.cmd.dataReq.msduLength;
  	
    // Allocate msdu in end of PSDU buffer.
    buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE - msduLength, 0);

    // Set mpdu buffer descriptor reference.
    pMacCmdDescr->params.cmd.dataReq.pPsduBuffDescr = pPsduBuffDescr;
  
    // Scan.
    {
      Uint8* pParams[] = {(Uint8*)pMacCmdDescr->params.cmd.dataReq.pPsduBuffDescr->data,
                          (Uint8*)&pMacCmdDescr->params.cmd.dataReq.msduHandle,
                          (Uint8*)&pMacCmdDescr->params.cmd.dataReq.txOptions};
      const MsgParamDescr format[] = {{MSG_PARAM_ID_AUINT8,          {msduLength}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_UINT8,                    {0}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_ENUM8, {sizeof(MacTxOptions)}, MSG_PARAM_FLAG_DEFAULT}};
  
      pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);
    }
  }
}

/*******************************************************************************
  Description: scans purge request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanPurgeReq(const MsgSrvFxns* pMsgSrvFxns,
                         struct MacCmdDescr* pMacCmdDescr)
{
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.purgeReq.msduHandle};
  const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};
 
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 1);
}

/*******************************************************************************
  Description: scans associate request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanAssocReq(const MsgSrvFxns* pMsgSrvFxns,
                         struct MacCmdDescr* pMacCmdDescr,
                         BuffDescr* pPsduBuffDescr)
{
  Bool securityEnable;
  
  // Scan channel and coordinator address information.
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.assocReq.channel,
                      (Uint8*)&pMacCmdDescr->params.cmd.assocReq.coordAddrMode,
                      (Uint8*)&pMacCmdDescr->params.cmd.assocReq.coordPanId};
                      
  MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8,                   {0}, MSG_PARAM_FLAG_DEFAULT},
                            {MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                            {MSG_PARAM_ID_UINT16,                  {0}, MSG_PARAM_FLAG_DEFAULT}};
    
  // Scan up to not included coordinator address.
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);
    
  // Scan coordinator address and capability.
  format[0].id = MSG_PARAM_ID_UINT16;
  if (MAC_ADDR_MODE_EXT == pMacCmdDescr->params.cmd.assocReq.coordAddrMode)
    format[0].id = MSG_PARAM_ID_UINT64;
  pParams[0] = (Uint8*)&pMacCmdDescr->params.cmd.assocReq.coordAddr;
    
  format[1].id = MSG_PARAM_ID_UINT8;
  pParams[1] = (Uint8*)&pMacCmdDescr->params.cmd.assocReq.capability;
  
  format[2].id = MSG_PARAM_ID_ENUM8;
  format[2].length.enum8 =  sizeof(Bool);
  pParams[2] = (Uint8*)&securityEnable;
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);
  
  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.assocReq.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: scans assiciate response message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanAssocResp(const MsgSrvFxns* pMsgSrvFxns,
                          struct MacCmdDescr* pMacCmdDescr,
                          BuffDescr* pPsduBuffDescr)
{
  Bool securityEnable;
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.assocResp.extAddr,
                      (Uint8*)&pMacCmdDescr->params.cmd.assocResp.shortAddr,
                      (Uint8*)&pMacCmdDescr->params.cmd.assocResp.status,
                      (Uint8*)&securityEnable};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64,                {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT16,                {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_ENUM8,      {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 4);

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.assocResp.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: scans disassociate request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanDisassocReq(const MsgSrvFxns* pMsgSrvFxns,
                            struct MacCmdDescr* pMacCmdDescr,
                            BuffDescr* pPsduBuffDescr)
{
  Bool securityEnable;
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.disassocReq.extAddr,
                      (Uint8*)&pMacCmdDescr->params.cmd.disassocReq.reason,
                      (Uint8*)&securityEnable};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64,                        {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_ENUM8, {sizeof(MacDisassocReason)}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.disassocReq.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: scans get request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanGetReq(const MsgSrvFxns* pMsgSrvFxns,
                       struct MacCmdDescr* pMacCmdDescr)
{
  Uint8* pParams = (Uint8*)&pMacCmdDescr->params.cmd.getReq.attrId;
  const MsgParamDescr format = {MSG_PARAM_ID_ENUM8, {sizeof(MacPibAttrId)}, MSG_PARAM_FLAG_DEFAULT};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, &format, &pParams, 1);
}

/*******************************************************************************
  Description: scans orphan response message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanOrphanResp(const MsgSrvFxns* pMsgSrvFxns,
                           struct MacCmdDescr* pMacCmdDescr,
                           BuffDescr* pPsduBuffDescr)
{
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.orphanResp.extAddr,
                      (Uint8*)&pMacCmdDescr->params.cmd.orphanResp.shortAddr,
                      (Uint8*)&pMacCmdDescr->params.cmd.orphanResp.isAssociated};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64,           {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT16,           {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.orphanResp.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: scans reset request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanResetReq(const MsgSrvFxns* pMsgSrvFxns,
                         struct MacCmdDescr* pMacCmdDescr)
{
  Uint8* pParams = (Uint8*)&pMacCmdDescr->params.cmd.resetReq.setDefault;
  const MsgParamDescr format = {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, &format, &pParams, 1);
}

/*******************************************************************************
  Description: scans rx enable request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanRxEnableReq(const MsgSrvFxns* pMsgSrvFxns,
                            struct MacCmdDescr* pMacCmdDescr)
{
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.rxEnableReq.deferPermit,
                      (Uint8*)&pMacCmdDescr->params.cmd.rxEnableReq.rxOnTime,
                      (Uint8*)&pMacCmdDescr->params.cmd.rxEnableReq.rxOnDuration};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT24,           {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT24,           {0}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);
}

/*******************************************************************************
  Description: scans scan request message.

  Parameters: pMacSrv      - MAC server reference.
              pCmdPoolItem - command pool item reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanScanReq(struct MacSrv* pMacSrv, MacSrvCmdPoolItem* pCmdPoolItem)
{	
  Uint8* pParams[] = {(Uint8*)&pCmdPoolItem->cmdDescr.params.cmd.scanReq.type,
                      (Uint8*)&pCmdPoolItem->cmdDescr.params.cmd.scanReq.channels,
                      (Uint8*)&pCmdPoolItem->cmdDescr.params.cmd.scanReq.duration};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacScanType)}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT32,                  {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT8,                   {0}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMacSrv->msgSrvFxns.scanf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 3);

  // Set scan result stuff.
  switch (pCmdPoolItem->cmdDescr.params.cmd.scanReq.type)
  {
    case MAC_SCAN_ED:
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.result.energy = pMacSrv->scanResult.energy;
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.maxResultSize = pMacSrv->scanEnergyMax;
         break;
      
    case MAC_SCAN_ACTIVE:
    case MAC_SCAN_PASSIVE:
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.result.panDescr = pMacSrv->scanResult.panDescr;
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.maxResultSize = pMacSrv->scanPanDescrMax;
         break;
         
    case MAC_SCAN_ORPHAN:
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.result.panDescr = 0;
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.result.energy = 0;
         pCmdPoolItem->cmdDescr.params.cmd.scanReq.maxResultSize = 0;
         break;
      
    default: 
         ASSERT(false);
  }
  
  pMacSrv->scanBusy = true;

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(&pCmdPoolItem->buffDescr, PHY_FRAME_MAX_SIZE, 0);
  pCmdPoolItem->cmdDescr.params.cmd.scanReq.pPsduBuffDescr = &pCmdPoolItem->buffDescr;
}

/*******************************************************************************
  Description: scans set request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pBeaconBuffDescr - beacon buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanSetReq(const MsgSrvFxns* pMsgSrvFxns,
                       struct MacCmdDescr* pMacCmdDescr,
                       BuffDescr* pBeaconBuffDescr)
{
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.setReq.attrId};
  MsgParamDescr format = {MSG_PARAM_ID_ENUM8, {sizeof(MacPibAttrId)}, MSG_PARAM_FLAG_DEFAULT};

  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, &format, pParams, 1);
  pParams[0] = (Uint8*)&pMacCmdDescr->params.cmd.setReq.attr;
  switch (pMacCmdDescr->params.cmd.setReq.attrId)
  {
    case MAC_PIB_ATTR_ID_ASSOCIATION_PERMIT:
    case MAC_PIB_ATTR_ID_AUTO_REQUEST:
    case MAC_PIB_ATTR_ID_BATT_LIFE_EXT:
    case MAC_PIB_ATTR_ID_GTS_PERMIT:
    case MAC_PIB_ATTR_ID_PROMISCUOUS_MODE:
    case MAC_PIB_ATTR_ID_RX_ON_WHEN_IDLE:
         format.id = MSG_PARAM_ID_ENUM8;
         format.length.enum8 = sizeof(Bool);
         break;
    
    case MAC_PIB_ATTR_ID_ACK_WAIT_DURATION:
    case MAC_PIB_ATTR_ID_BATT_LIFE_EXT_PERIODS:
    case MAC_PIB_ATTR_ID_BEACON_ORDER:
    case MAC_PIB_ATTR_ID_BSN:
    case MAC_PIB_ATTR_ID_DSN:
    case MAC_PIB_ATTR_ID_MAX_CSMA_BACKOFFS:
    case MAC_PIB_ATTR_ID_MIN_BE:
    case MAC_PIB_ATTR_ID_SUPERFRAME_ORDER:
    case MAC_PIB_ATTR_ID_BEACON_PAYLOAD_LENGTH:
         format.id = MSG_PARAM_ID_UINT8;
         break;

    case MAC_PIB_ATTR_ID_BEACON_TX_TIME:
         format.id = MSG_PARAM_ID_UINT24;
         break;
        
    case MAC_PIB_ATTR_ID_COORD_EXT_ADDR:
    case MAC_PIB_ATTR_ID_EXT_ADDR:
         format.id = MSG_PARAM_ID_UINT64;
         break;
      
    case MAC_PIB_ATTR_ID_COORD_SHORT_ADDR:
    case MAC_PIB_ATTR_ID_PANID:
    case MAC_PIB_ATTR_ID_SHORT_ADDR:
    case MAC_PIB_ATTR_ID_TRANSACTION_PERSISTENCE_TIME:
         format.id = MSG_PARAM_ID_UINT16;
         break;
    
    case MAC_PIB_ATTR_ID_BEACON_PAYLOAD:
         {
           Uint8 length;
           
           // Read beacon length.
           pParams[0] = (Uint8*)&length;
           pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, &format, pParams, 1);
         
           format.id = MSG_PARAM_ID_AUINT8;
           format.length.array = length;
           buffDescr_setRoom(pBeaconBuffDescr, (pBeaconBuffDescr->end - pBeaconBuffDescr->head - length), 0);
           pMacCmdDescr->params.cmd.setReq.attr.pBeaconBuffDescr = pBeaconBuffDescr;
           pParams[0] = (Uint8*)pBeaconBuffDescr->data;
         }
         break;
         
    default:
         ASSERT(false);
  } 
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, &format, pParams, 1);
}

/*******************************************************************************
  Description: scans start request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanStartReq(const MsgSrvFxns* pMsgSrvFxns,
                         struct MacCmdDescr* pMacCmdDescr,
                         BuffDescr* pPsduBuffDescr)
{
  Uint8 orders;
  Uint8 flags;
	
  Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.startReq.panId,
                      (Uint8*)&pMacCmdDescr->params.cmd.startReq.channel,
                      (Uint8*)&orders,
                      (Uint8*)&flags};
                      
  const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                  {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};
    
  pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 4);

  pMacCmdDescr->params.cmd.startReq.beaconOrder = orders & 0x0F;
  pMacCmdDescr->params.cmd.startReq.superframeOrder = orders >> 4;
  pMacCmdDescr->params.cmd.startReq.panCoordinator = (flags & 1) ? true : false;
  pMacCmdDescr->params.cmd.startReq.batteryLifeExt = (flags & 2) ? true : false;
  pMacCmdDescr->params.cmd.startReq.coordRealignment = (flags & 4) ? true : false;

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.startReq.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: scans poll request message.

  Parameters: pMsgSrvFxns    - message server reference. 
              pMacCmdDescr   - command descriptor reference.
              pPsduBuffDescr - PSDU buffer descriptor reference.
  
  Returns: nothing.
*******************************************************************************/
static void scanPollReq(const MsgSrvFxns* pMsgSrvFxns,
                        struct MacCmdDescr* pMacCmdDescr,
                        BuffDescr* pPsduBuffDescr)
{
  Bool extAddrMode = false;
  
  // Scan coordinator address mode.  
  {	
    Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.pollReq.coordAddrMode};

    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT}};	
	
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 1);
    if (MAC_ADDR_MODE_EXT == pMacCmdDescr->params.cmd.pollReq.coordAddrMode)
      extAddrMode = true;
  }	
	
  // Scan rest address info and security flag. 
  {	
    Bool securityEnable;
    Uint8* pParams[] = {(Uint8*)&pMacCmdDescr->params.cmd.pollReq.coordPanId,
                        (Uint8*)&pMacCmdDescr->params.cmd.pollReq.coordAddr,
                        (Uint8*)&securityEnable};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
  
    if (extAddrMode)
      format[1].id = MSG_PARAM_ID_UINT64;
    pMsgSrvFxns->scanf(pMsgSrvFxns->pMsgSrv, format, pParams, 3);
  }

  // Prepare PSDU buffer descriptor and set reference to it.
  buffDescr_setRoom(pPsduBuffDescr, PHY_FRAME_MAX_SIZE, 0);
  pMacCmdDescr->params.cmd.pollReq.pPsduBuffDescr = pPsduBuffDescr;
}

/*******************************************************************************
  Description: finds and takes free buffer for event printing.
    
  Parameters: pEventBuffDescr - first event buffer descriptor reference.
              eventBuffMax - max number of event buffer.
  
  Returns:  free buffer reference if it has been found and 0, otherwise.
*******************************************************************************/
static BuffDescr* findFreeEventBuff(BuffDescr* pEventBuffDescr,
                                    Uint8 eventBuffMax)
{
  Uint8 buffInd;
  BuffDescr* pBuffDescr = 0;
  
  for (buffInd = 0; buffInd < eventBuffMax; buffInd++)
  {
    if (!pEventBuffDescr[buffInd].busy)
    {
      pBuffDescr = &pEventBuffDescr[buffInd];
      buffDescr_setRoom(pBuffDescr, 0, (pBuffDescr->end - pBuffDescr->head));
      pBuffDescr->busy = true;
      break;
    }
  }
  return pBuffDescr;
}

/*******************************************************************************
  Description: prints data confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printDataConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.data.msduHandle,
                        (Uint8*)&pCmdDescr->params.conf.data.status};
    
    const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8,                 {0}, MSG_PARAM_FLAG_DEFAULT},
                                    {MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_DATA_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints purge confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printPurgeConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.purge.msduHandle,
                        (Uint8*)&pCmdDescr->params.conf.purge.status};
                        
    const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8,                 {0}, MSG_PARAM_FLAG_DEFAULT},
                                    {MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_PURGE_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints association confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printAssocConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.assoc.shortAddr,
                        (Uint8*)&pCmdDescr->params.conf.assoc.status};
                        
    const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT16,                {0}, MSG_PARAM_FLAG_DEFAULT},
                                    {MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_ASSOC_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints disassociation confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printDisassocConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.disassoc.status};
    const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
   
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_DISASSOC_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints get confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printGetConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.get.status,
                        (Uint8*)&pCmdDescr->params.conf.get.attrId};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacPibAttrId)}, MSG_PARAM_FLAG_DEFAULT}};
  
    // Print status and attribute ID.
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_GET_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
  
    // Print attribute.
    pParams[0] = (Uint8*)&pCmdDescr->params.conf.get.attr;
    switch (pCmdDescr->params.conf.get.attrId)
    {
      case MAC_PIB_ATTR_ID_ASSOCIATION_PERMIT:
      case MAC_PIB_ATTR_ID_AUTO_REQUEST:
      case MAC_PIB_ATTR_ID_BATT_LIFE_EXT:
      case MAC_PIB_ATTR_ID_GTS_PERMIT:
      case MAC_PIB_ATTR_ID_PROMISCUOUS_MODE:
      case MAC_PIB_ATTR_ID_RX_ON_WHEN_IDLE:
           format[0].id = MSG_PARAM_ID_ENUM8;
           format[0].length.enum8 = sizeof(sizeof(MacPibAttrId));
           break;
    	
      case MAC_PIB_ATTR_ID_ACK_WAIT_DURATION:
      case MAC_PIB_ATTR_ID_BATT_LIFE_EXT_PERIODS:
      case MAC_PIB_ATTR_ID_BEACON_ORDER:
      case MAC_PIB_ATTR_ID_BSN:
      case MAC_PIB_ATTR_ID_DSN:
      case MAC_PIB_ATTR_ID_MAX_CSMA_BACKOFFS:
      case MAC_PIB_ATTR_ID_MIN_BE:
      case MAC_PIB_ATTR_ID_SUPERFRAME_ORDER:
      case MAC_PIB_ATTR_ID_BEACON_PAYLOAD_LENGTH:
           format[0].id = MSG_PARAM_ID_UINT8;
           break;

      case MAC_PIB_ATTR_ID_BEACON_TX_TIME:
           format[0].id = MSG_PARAM_ID_UINT24;
           break;
        
      case MAC_PIB_ATTR_ID_COORD_EXT_ADDR:
      case MAC_PIB_ATTR_ID_EXT_ADDR:
           format[0].id = MSG_PARAM_ID_UINT64;
           break;
      
      case MAC_PIB_ATTR_ID_COORD_SHORT_ADDR:
      case MAC_PIB_ATTR_ID_PANID:
      case MAC_PIB_ATTR_ID_SHORT_ADDR:
      case MAC_PIB_ATTR_ID_TRANSACTION_PERSISTENCE_TIME:
           format[0].id = MSG_PARAM_ID_UINT16;
           break;
    
      case MAC_PIB_ATTR_ID_BEACON_PAYLOAD:
           {
             Uint8 beaconLength = 0;
            
             if (pCmdDescr->params.conf.get.attr.pBeaconBuffDescr)
             {
           	   beaconLength = pCmdDescr->params.conf.get.attr.pBeaconBuffDescr->tail -
                              pCmdDescr->params.conf.get.attr.pBeaconBuffDescr->data;
             }
           	
           	 // Print beacon payload length.
           	 format[0].id = MSG_PARAM_ID_UINT8;
           	 pParams[0] = (Uint8*)&beaconLength;
             pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
             
             // Prepare for beacon payload printing.
             format[0].id = MSG_PARAM_ID_AUINT8;
             format[0].length.array = beaconLength;
             pParams[0] = (Uint8*)pCmdDescr->params.conf.get.attr.pBeaconBuffDescr->data;
           }
           break;
         
      default:
           ASSERT(false);
    }
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints set confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printSetConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.set.status,
                        (Uint8*)&pCmdDescr->params.conf.set.attrId};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacPibAttrId)}, MSG_PARAM_FLAG_DEFAULT}};
  
    // Print status and attribute ID.
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_SET_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
  
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints reset confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printResetConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.reset.status};
    const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
   
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_RESET_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints rx enable confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printRxEnableConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.rxEnable.status};
    const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_RX_ENABLE_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints start confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printStartConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.start.status};
    const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_START_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints scan confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printScanConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_SCAN_CONF, pPrintBuffDescr);
  
    // Print parameters but scan results.
    {
      Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.scan.status,
                          (Uint8*)&pCmdDescr->params.conf.scan.type,
                          (Uint8*)&pCmdDescr->params.conf.scan.unscannedChannels,
                          (Uint8*)&pCmdDescr->params.conf.scan.resultSize};
      const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_ENUM8, {sizeof(MacScanType)}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_UINT32, {0}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};
    
      pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 4);
    }
  
    // Print scan results.
    {
      switch (pCmdDescr->params.conf.scan.type)
      {
        case MAC_ED_SCAN:
             {
               Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.scan.result.energy};
               
               const MsgParamDescr format[] = {{MSG_PARAM_ID_AUINT8,
                                                {pCmdDescr->params.conf.scan.resultSize},
                                                MSG_PARAM_FLAG_DEFAULT}};
            
               pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
             }
             break;
           
        case MAC_ACTIVE_SCAN:
        case MAC_PASSIVE_SCAN:
             {
               Uint8 resultInd;
               Bool securityUse = false;
               Uint8 aclEntry = 0x08;
               Bool securityFail = false;
               Uint8* pParams[PAN_DESCR_PARAM_MAX];
               MsgParamDescr format[PAN_DESCR_PARAM_MAX] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT24, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                            {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
                                      
               for (resultInd = 0; resultInd < pCmdDescr->params.conf.scan.resultSize; resultInd++)
               {
                 pParams[PAN_DESCR_COORD_ADDR_MODE_PARAM_IND] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].coordAddrMode;
                 pParams[1] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].coordPanId;
                 pParams[PAN_DESCR_COORD_ADDR_PARAM_IND] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].coordAddr;
                 pParams[3] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].channel;
                 pParams[4] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].superframeSpec;
                 pParams[5] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].gtsPermit;
                 pParams[6] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].lqi;
                 pParams[7] = (Uint8*)&pCmdDescr->params.conf.scan.result.panDescr[resultInd].timeStamp;
                 pParams[8] = (Uint8*)&securityUse;
                 pParams[9] = (Uint8*)&aclEntry;
                 pParams[10] = (Uint8*)&securityFail;
               
                 if (MAC_EXT_ADDR_MODE == *pParams[PAN_DESCR_COORD_ADDR_MODE_PARAM_IND])
                   format[PAN_DESCR_COORD_ADDR_PARAM_IND].id = MSG_PARAM_ID_UINT64;
               
                 pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, PAN_DESCR_PARAM_MAX);
               }
             }
             break;
        
        case MAC_ORPHAN_SCAN:
             break;
        
        default:
             ASSERT(false);
      }
    }

    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor and scan operation stuff.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    pMacSrv->scanBusy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints poll confirm.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if confirm has been printed and false, otherwise.
*******************************************************************************/
static Bool printPollConf(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.poll.status};
    const MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};
  
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_POLL_CONF, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
   
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints comm status indication.
    
  Parameters: pMacSrv   - MAC server reference.
              pCmdDescr - command descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printCommStatusInd(struct MacSrv* pMacSrv, struct MacCmdDescr* pCmdDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
    
  // Find free buffer for confirm printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    Uint8* pParams[] = {(Uint8*)&pCmdDescr->params.conf.commStatusInd.panId,
                        (Uint8*)&pCmdDescr->params.conf.commStatusInd.srcAddrMode,
                        (Uint8*)&pCmdDescr->params.conf.commStatusInd.srcAddr,
                        (Uint8*)&pCmdDescr->params.conf.commStatusInd.dstAddrMode,
                        (Uint8*)&pCmdDescr->params.conf.commStatusInd.dstAddr,
                        (Uint8*)&pCmdDescr->params.conf.commStatusInd.status};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacStatus)}, MSG_PARAM_FLAG_DEFAULT}};

    if (MAC_EXT_ADDR_MODE == *pParams[COMM_STATUS_IND_SRC_ADDR_MODE_PARAM_IND])
      format[COMM_STATUS_IND_SRC_ADDR_PARAM_IND].id = MSG_PARAM_ID_UINT64;

    if (MAC_EXT_ADDR_MODE == *pParams[COMM_STATUS_IND_DST_ADDR_MODE_PARAM_IND])
      format[COMM_STATUS_IND_DST_ADDR_PARAM_IND].id = MSG_PARAM_ID_UINT64;

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_COMM_STATUS_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, COMM_STATUS_IND_PARAM_MAX);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release command descriptor.
    pMacSrv->cmdPool[pCmdDescr->handle].busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints data indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printDataInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    Uint8 aclEntry = ACL_ENTRY;
    Bool securityUse = false;
    Uint8* pParams[] = {(Uint8*)&pIndDescr->params.data.srcAddrMode,
                        (Uint8*)&pIndDescr->params.data.srcPanId,
                        (Uint8*)&pIndDescr->params.data.srcAddr,
                        (Uint8*)&pIndDescr->params.data.dstAddrMode,
                        (Uint8*)&pIndDescr->params.data.dstPanId,
                        (Uint8*)&pIndDescr->params.data.dstAddr,
                        (Uint8*)&pIndDescr->params.data.msduLength,
                        (Uint8*)&pIndDescr->params.data.pMsduBuffDescr->data,
                        (Uint8*)&pIndDescr->params.data.lqi,
                        (Uint8*)&securityUse,
                        (Uint8*)&aclEntry};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_AUINT8, {pIndDescr->params.data.msduLength}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

    if (MAC_EXT_ADDR_MODE == *pParams[DATA_IND_SRC_ADDR_MODE_PARAM_IND])
      format[DATA_IND_SRC_ADDR_MODE_PARAM_IND].id = MSG_PARAM_ID_UINT64;

    if (MAC_EXT_ADDR_MODE == *pParams[DATA_IND_DST_ADDR_MODE_PARAM_IND])
      format[DATA_IND_DST_ADDR_PARAM_IND].id = MSG_PARAM_ID_UINT64;

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_DATA_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, DATA_IND_PARAM_MAX);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints associate indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printAssocInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    Uint8 aclEntry = ACL_ENTRY;
    Bool securityUse = false;
    Uint8* pParams[] = {(Uint8*)&pIndDescr->params.assoc.extAddr,
                        (Uint8*)&pIndDescr->params.assoc.capability,
                        (Uint8*)&securityUse,
                        (Uint8*)&aclEntry};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_ASSOC_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 4);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints disassociate indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printDisassocInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    Uint8 aclEntry = ACL_ENTRY;
    Bool securityUse = false;
    Uint8* pParams[] = {(Uint8*)&pIndDescr->params.disassoc.extAddr,
                        (Uint8*)&pIndDescr->params.disassoc.reason,
                        (Uint8*)&securityUse,
                        (Uint8*)&aclEntry};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(MacDisassocReason)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_DISASSOC_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 4);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints orphan indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printOrphanInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    Uint8 aclEntry = ACL_ENTRY;
    Bool securityUse = false;
    Uint8* pParams[] = {(Uint8*)&pIndDescr->params.orphan.extAddr,
                        (Uint8*)&securityUse,
                        (Uint8*)&aclEntry};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT64, {0}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                              {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_ORPHAN_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 3);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints poll indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printPollInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    Uint8* pParams[] = {(Uint8*)&pIndDescr->params.poll.shortAddr};
                      
    MsgParamDescr format[] = {{MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT}};

    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_POLL_IND, pPrintBuffDescr);
    pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

/*******************************************************************************
  Description: prints beacon notify indication.
    
  Parameters: pMacSrv - MAC server reference. 
              pIndDescr  - indication descriptor reference.
  
  Returns:  true if indication has been printed and false, otherwise.
*******************************************************************************/
static Bool printBeaconNotifyInd(struct MacSrv* pMacSrv, struct MacIndDescr* pIndDescr)
{
  BuffDescr* pPrintBuffDescr = 0;
  
  // Find free buffer for indication printing.
  pPrintBuffDescr = findFreeEventBuff(pMacSrv->eventBuffDescr, pMacSrv->eventBuffMax);
  if (0 != pPrintBuffDescr)
  {
    // There is a free buffer for indication printing.
    pMacSrv->msgSrvFxns.startPrintf(pMacSrv->msgSrvFxns.pMsgSrv, MAC_MSG_ID_BEACON_NOTIFY_IND, pPrintBuffDescr);
  
    // Print BSN.
    {
      Uint8* pParams[] = {(Uint8*)&pIndDescr->params.beaconNotify.bsn};
      const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

      pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    }
  
    // Print PAN descriptor.
    {
      Bool securityUse = false;
      Uint8 aclEntry = 0x08;
      Bool securityFail = false;
      Uint8* pParams[PAN_DESCR_PARAM_MAX];
      MsgParamDescr format[PAN_DESCR_PARAM_MAX] = {{MSG_PARAM_ID_ENUM8, {sizeof(MacAddrMode)}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT24, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT},
                                                   {MSG_PARAM_ID_ENUM8, {sizeof(Bool)}, MSG_PARAM_FLAG_DEFAULT}};
                                      
      pParams[PAN_DESCR_COORD_ADDR_MODE_PARAM_IND] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.coordAddrMode;
      pParams[1] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.coordPanId;
      pParams[PAN_DESCR_COORD_ADDR_PARAM_IND] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.coordAddr;
      pParams[3] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.channel;
      pParams[4] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.superframeSpec;
      pParams[5] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.gtsPermit;
      pParams[6] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.lqi;
      pParams[7] = (Uint8*)&pIndDescr->params.beaconNotify.panDescr.timeStamp;
      pParams[8] = (Uint8*)&securityUse;
      pParams[9] = (Uint8*)&aclEntry;
      pParams[10] = (Uint8*)&securityFail;
               
      if (MAC_EXT_ADDR_MODE == *pParams[PAN_DESCR_COORD_ADDR_MODE_PARAM_IND])
        format[PAN_DESCR_COORD_ADDR_PARAM_IND].id = MSG_PARAM_ID_UINT64;
               
      pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, PAN_DESCR_PARAM_MAX);
    }
  
    // Print pending address specification.
    {
      Uint8* pParams[] = {(Uint8*)&pIndDescr->params.beaconNotify.pendAddrSpec};
      const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8, {0}, MSG_PARAM_FLAG_DEFAULT}};

      pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 1);
    }
  
    // Print pending address list.
    {
      Uint8* pParams;
      MsgParamDescr format = {MSG_PARAM_ID_UINT16, {0}, MSG_PARAM_FLAG_DEFAULT};
      Uint8 addrCount;
      Uint8 addrInd;

      // Print short addresses.
      for (addrCount = 0; addrCount < pIndDescr->params.beaconNotify.pendAddrSpec.shortAddr; addrCount++)
      {
        pParams = (Uint8*)&pIndDescr->params.beaconNotify.pendAddrList[addrCount];
        pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, &format, &pParams, 1);
      }
    
      // Print extended addresses.
      format.id = MSG_PARAM_ID_UINT64;
      addrInd = 0;
      while ((addrInd++ < pIndDescr->params.beaconNotify.pendAddrSpec.extAddr) && (addrCount++ < BEACON_PEND_ADDR_LIST_MAX_SIZE))
      {
        pParams = (Uint8*)&pIndDescr->params.beaconNotify.pendAddrList[addrCount];
        pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, &format, &pParams, 1);
      }
    }
  
    // Print msdu length and msdu.
    {
      Uint8 msduLength = pIndDescr->params.beaconNotify.pMsduBuffDescr->tail - 
                         pIndDescr->params.beaconNotify.pMsduBuffDescr->data;
        
      Uint8* pParams[] = {(Uint8*)&msduLength,
                          (Uint8*)pIndDescr->params.beaconNotify.pMsduBuffDescr->data};
                      
      const MsgParamDescr format[] = {{MSG_PARAM_ID_UINT8,           {0}, MSG_PARAM_FLAG_DEFAULT},
                                      {MSG_PARAM_ID_AUINT8, {msduLength}, MSG_PARAM_FLAG_DEFAULT}}; 
                                      
      pMacSrv->msgSrvFxns.printf(pMacSrv->msgSrvFxns.pMsgSrv, format, pParams, 2);
    }
  
    pMacSrv->msgSrvFxns.endPrintf(pMacSrv->msgSrvFxns.pMsgSrv);
    
    // Release indication descriptor.
    pIndDescr->busy = false;
    return true;
  }
  return false;
}

// eof mac_srv.c
