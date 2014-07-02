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
  zigtst_cntrl_fxns.h
  
  ZigTest control interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_channel_fxns.h"
#include "byte_channel_fxns.h"

#ifndef ZIGTST_CNTRL_FXNS_H_
#define ZIGTST_CNTRL_FXNS_H_

struct ZigTstChannel
{
  ByteChannelFxns byteChannelFxns; // Byte channel reference.

  List txPendBuffList; // Descriptor list for outgoing frames.
  List rxPendBuffList; // Descriptor list for incoming frames.
                       // They are not read yet by higher layer.

  BuffDescr* activeTxBuffDescr; // Reference for buffer descriptor being transmitted.
  Uint8* txBuffPtr;             // Current position in TX buffer.
  Uint8 currTxByte; 

  Uint16 rxBuffSize; // RX buffer size.
  Uint16 rxBuffMax;  // Number of RX buffers.
  BuffDescr rxBuffDescr[ZIGTST_RX_BUFF_MAX];
  Uint8 rxBuff[ZIGTST_RX_BUFF_SIZE * ZIGTST_RX_BUFF_MAX];
  BuffDescr* activeRxBuffDescr; // Reference for buffer descriptor being accepted
                                // data from byte channel. 
  Uint8* rxBuffPtr;             // Current position in RX buffer.
  Uint16 rxLength;

  enum
  {
    ZIGTST_RXSTATE_INVALID,
    ZIGTST_RXSTATE_IDLE,
    ZIGTST_RXSTATE_DATA,
  } rxState;
  
  enum
  {
    ZIGTST_TXSTATE_INVALID,
    ZIGTST_TXSTATE_IDLE,   // There is no transmission.
    ZIGTST_TXSTATE_TX,     // Transmission is in progress.
  } txState;
};

/*******************************************************************************
  Description: provides reference to ZigTest channel interface.
  
  Parameters: pMsgChannelFxns - ZigTest channel reference.
  
  Returns: nothing.
*******************************************************************************/
void zigTstChannel_getFxns(MsgChannelFxns* pMsgChannelFxns);

/*******************************************************************************
  Description: inits ZigTest channel.
  
               Attention: rxBuffSize patrameter must be initialized with
                          ZIGTST_RX_BUFF_SIZE macros.
                          rxBuffMax patrameter must be initialized with
                          ZIGTST_RX_BUFF_MAX macros.
               
  Parameters: pZigTstChannel - ZigTest channel reference.
              pByteChannelFxns - byte channel interface reference.
              rxBuffSize - size of RX buffer.
              rxBuffMax - number of RX buffers.
  
  Returns: nothing.
*******************************************************************************/
void zigTstChannel_init(struct ZigTstChannel* pZigTstChannel,
                        const ByteChannelFxns* pByteChannelFxns,
                        Uint16 rxBuffSize,
                        Uint16 rxBuffMax);
                       
/*******************************************************************************
  Description: serves frames transmission and receive.
  
  Parameters: pZigTstChannel - ZigTest channel reference.
  
  Returns: nothing.
*******************************************************************************/
void zigTstChannel_process(struct ZigTstChannel* pZigTstChannel);

#endif

// eof zigtst_cntrl_fxns.h
