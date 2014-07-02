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
** net_data_fxns.h
**  
** 
*******************************************************************************/

#ifndef NET_DATA_FXNS_ 
#define NET_DATA_FXNS_ 

#include "net_types.h"

#define NET_BROADCAST_ENDPOINT 0xFF
#define NET_DATA_MAX_LENGTH    84
#define NET_BOADCAST_ADDR 0xFFFF 

struct Net;

/*******************************************************************************/
typedef struct
{
  NetAddr       dstAddr;
  NetEndPoint   dstEndPoint;
  NetEndPoint   srcEndPoint;
  Bool          arq;
  Uint8*        pData;                    // Data buffer pointer.
  Uint8         length;                   // Data length.
  NetDataHandle handle;                   // Data handle.
  Uint8         radius;
} NetDataReqParams;

/*******************************************************************************/

/*******************************************************************************/
typedef struct
{
  NetDataHandle     handle;                   // Data handle.
  NetStatus status;
  
} NetDataConfParams;

/*******************************************************************************/
typedef struct
{
  NetAllAddr  srcAddr;
  NetEndPoint srcEndPoint;
  NetEndPoint dstEndPoint;
  Uint8*      pData;                    // Data buffer pointer.
  Uint8       length;                   // Data length.
  Bool        isBroadcast;            // Packet sent with broadcast address.
  Uint8       lqi;                      // LQI value.
  Int8        rssi;                      // RSSI value.
} NetDataIndParams;

/*******************************************************************************/
typedef struct 
{
  struct Net* pNet;

/*******************************************************************************
 Description: register endpoint for transfering data. 
  	
 Parameters:  pNet - reference to the Net object,
              ep   - number of endpoint. 
                     Must be in the range of 1 - 239 to recieve 
                     data intended for particular endpoint, 
                     or NET_BROADCAST_ENDPOINT to recieve data intended for all 
                     unregistered endpoints in the range 1 - 239.
 
 Returns:     true - if registered succesfully,
              false - if endpoint is already registered or 
                      endpoint number Is out of range.
*******************************************************************************/
  Bool (*openEndPoint)(struct Net* pNet, NetEndPoint ep);

/*******************************************************************************
 Description: unregister endpoint for transfering data.

 Parameters:  pNet - reference to the Net object,
              ep   - number of endpoint (which used for openEndpoint).

 Returns:     true - if unregistered succesfully,
              false - if endpoint has not been registered or
                      endpoint number Is out of range..
*******************************************************************************/
  Bool (*closeEndPoint)(struct Net* pNet, NetEndPoint ep);


  Bool (*dataReq)(struct Net* pNet, const NetDataReqParams *pParams);
  Bool (*getConf)(struct Net* pNet, NetEndPoint ep, NetDataConfParams *conf);
  Bool (*getInd)(struct Net* pNet, NetEndPoint ep, NetDataIndParams *ind);
  
} NetDataFxns;

#endif // NET_DATA_FXNS_
//eof net_data_fxns.h

