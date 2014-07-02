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
 
 /********************************************************************************
  Id.h
  
  Constants for parametrization in NWKC.nc.
  *******************************************************************************/
#ifndef _ID_H_
#define _ID_H_
typedef enum
{
  CTRL_NIBCTL = 0x00,

  IRT_RERRPACKET = 0x01,
  IRT_DATARM = 0x02,

  // NLME_FORMATION
  FORM_ED_SCAN = 0x03,
  FORM_ACTIVE_SCAN = 0x04,
  FORM_PROMISC_SET = 0x05,
  FORM_SHORT_SET = 0x06,
  FORM_PLEN_SET = 0x07,
  FORM_PAYLD_SET = 0x08,
  FORM_RXON_SET = 0x09,
  FORM_PROMISC_SET_OFF = 0x0A,
  FORM_START = 0x0B,
  FORM_RX = 0x0C,
  FORM_NWK_CTRL,

  // NLME_DISCOVERY
  DISC,
  DISC_SCAN,
  DISC_PROMISC_SET,
  DISC_PROMISC_SET_OFF,
  DISC_SHORT_SET,
  DISC_GET,
  DISC_RX,                  //20
  DISC_NWK_CTRL,

  // NLME_JOIN request
  JOINR,
  JOINR_SCAN,
  JOINR_RX,
  JOINR_BO_SET,
  JOINR_SO_SET,
  JOINR_PANID,
  JOINR_OWN_SO_SET,
  JOINR_SHORT_SET,
  JOINR_RX_SET,            //30
  JOINR_PAYLD_SET,
  JOINR_PLEN_SET,
  JOINR_GET_JEXT,
  JOINR_GET_PANID,
  JOINR_NWK_CTRL,

  // NLME_LEAVE
  LEAVE,
  LEAVE_PARENT,
  LEAVE_REPAIR,
  LEAVE_SPEC,
  LEAVE_FAIL_CHILD,                 //40
  LEAVE_NWK_CTRL,

  // NLME_JOIN indication
  JOINI_COMM,

  // NLME_RESET
  RESET_NWK_RESET,
  RESET_NWK_CTRL,      
  RESET_NIBCTL,
  RESET_NWK_LEAVE,
  RESET_SET_EXT,
  RESET_SET_TRANS,

  // NLME_PERMIT
  PERMIT_SET,

  // MLME_ORPHAN indication
  ORPHAN_COMM,                      //50

  // NLDE_DATA
  DATA_UC,
  DATA_IND,
  DATA_BC,
  REPAIR_DISC,
  REPAIR_JOINR,        

  // Routing
  ROUTING_NWK_CTRL,
  NWK_CMD,
  RREQ_CMD,
  RREP_CMD,
  RERR_CMD,            //60

  ROUTE_SUCCESS,
  ROUTE_FAIL,
  ROUTE_UNDERWAY,

  // Broadcast
  BCAST_PACKET,
  BC_SPREAD,
  BC_ARROW,

  // NLME_START
  START_START,
  START_RXON,
  START_RX,
  STARTR_NWK_CTRL,         //70

  // NLME_SET
  TRANS_TIME,

  //NIBM 
  NIB_PERMIT_SET,

  POLL,

  // No module. Used to protect from doing 
  // something if we receive confirmation from a 
  // non-existing module
  NO_MODULE,               //74
  APL_DISABLE_RX
} nwkId_t;

#endif

// eof Id.h
