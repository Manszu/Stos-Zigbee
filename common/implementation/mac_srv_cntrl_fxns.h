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
  mac_srv_cntrl_fxns.h
  
  MAC server control interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_srv_fxns.h"
#include "mac_fxns.h"
#include "leds_fxns.h"
#include "mac_cntrl_fxns.h"

#ifndef MAC_SRV_CNTRL_FXNS_H_
#define MAC_SRV_CNTRL_FXNS_H_

typedef struct
{
  struct MacCmdDescr cmdDescr;
  BuffDescr buffDescr;
  Uint8 buff[PHY_FRAME_MAX_SIZE];
  Bool busy;
} MacSrvCmdPoolItem;

struct MacSrv
{
  MsgSrvFxns msgSrvFxns;
  MacCmdFxns macCmdFxns;
  LedsFxns ledsFxns;
  
  Uint8 beaconBuffSize;
  Uint8 beaconBuff[MAC_SRV_BEACON_BUFF_SIZE];
  BuffDescr beaconBuffDescr;
  
  Uint8 eventBuffSize;
  Uint8 eventBuffMax;
  BuffDescr eventBuffDescr[MAC_SRV_EVENT_BUFF_MAX];
  Uint8 eventBuff[MAC_SRV_EVENT_BUFF_SIZE * MAC_SRV_EVENT_BUFF_MAX];
  
  List confPendList;
  List indPendList;
   
  Uint8 cmdPoolSize;
  MacSrvCmdPoolItem cmdPool[MAC_SRV_CMD_POOL_SIZE];
  
  // Stuff for scan operation.
  Uint8 scanEnergyMax;
  Uint8 scanPanDescrMax;
  Bool scanBusy;         // Only one scan operation is allowed.
  union
  {
    PhyEnergyLevel energy[MAC_SRV_SCAN_ENERGY_MAX];
    MacPanDescr panDescr[MAC_SRV_SCAN_PAN_DESCR_MAX];
  } scanResult;
};

/*******************************************************************************
  Description: returns reference to MAC event interface.

  Parameters: pMacEventFxns - reference to MAC event interface.
  
  Returns: nothing.
*******************************************************************************/
void macSrv_getFxns(MacEventFxns* pMacEventFxns);

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
                Uint8 scanPanDescrMax);

/*******************************************************************************
  Description: gets events from MAC and prints them to message server.
               Gets commands from message server and passes them to MAC.
    
  Parameters: pMacSrv - MAC server reference.
  
  Returns: nothing.
*******************************************************************************/
void macSrv_process(struct MacSrv* pMacSrv);

#endif

// eof mac_srv_cntrl_fxns.h
