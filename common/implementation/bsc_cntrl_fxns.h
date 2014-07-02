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
  bsc_cntrl_fxns.h
  
  BSC control interface declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_channel_fxns.h"
#include "byte_channel_fxns.h"

#ifndef BSC_CNTRL_FXNS_H_
#define BSC_CNTRL_FXNS_H_

struct BscChannel
{
  ByteChannelFxns byteChannelFxns; // Byte channel reference.
  
  List txPendBuffList; // Descriptor list for outgoing frames.
  List rxPendBuffList; // Descriptor list for incoming frames.
                       // They are not read yet by higher layer.
  
  BuffDescr* activeTxBuffDescr; // Reference for buffer descriptor being transmitted.
  Uint8* txBuffPtr;             // Current position in TX buffer.
  Uint8 currTxByte; 
  Uint8  txCrc;                 // CRC for outgoing frame.
                   
  Uint16 rxBuffSize; // RX buffer size.
  Uint16 rxBuffMax;  // Number of RX buffers.
  
  BuffDescr rxBuffDescr[BSC_RX_BUFF_MAX];
  Uint8 rxBuff[BSC_RX_BUFF_SIZE * BSC_RX_BUFF_MAX];
  BuffDescr* activeRxBuffDescr; // Reference for buffer descriptor being accepted
                                // data from byte channel. 
  Uint8* activeRxBuffPtr;       // Current position in RX buffer.
  Uint8  rxCrc;                 // CRC for incoming frame.
                                
  enum
  {
    BSC_INVALID_RXSTATE,
    BSC_IDLE_RXSTATE,
    BSC_STX_RXSTATE,
    BSC_DATA_RXSTATE,
    BSC_DLE_DATA_RXSTATE,
    BSC_ETX_RXSTATE,
  } rxState;
  
  enum
  {
    BSC_INVALID_TXSTATE,
    BSC_IDLE_TXSTATE,     // There is no transmission.
    BSC_DLE_STX_TXSTATE,  // DLE before STX is been transmitting.
    BSC_STX_TXSTATE,      // STX is been transmitting.
    BSC_DATA_TXSTATE,     // Data is been transmitting.
    BSC_DLE_DATA_TXSTATE, // DLE before data is been transmitting.
    BSC_DLE_ETX_TXSTATE,  // DLE before STX is been transmitting.
    BSC_ETX_TXSTATE,      // ETX is been transmitting.
    BSC_CRC_TXSTATE,      // CRC is been transmitting.
  } txState;
};

/*******************************************************************************
  Description: provides reference to BSC channel interface.
  
  Parameters: pMsgChannelFxns - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_getFxns(MsgChannelFxns* pMsgChannelFxns);

/*******************************************************************************
  Description: inits BSC channel.
  
               Attention: rxBuffSize patrameter must be initialized with
                          BSC_RX_BUFF_SIZE macros.
                          rxBuffMax patrameter must be initialized with
                          BSC_RX_BUFF_MAX macros.
               
  Parameters: pBscChannel - BSC channel reference.
              pByteChannelFxns - byte channel interface reference.
              rxBuffSize - size of RX buffer.
              rxBuffMax - number of RX buffers.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_init(struct BscChannel* pBscChannel,
                    const ByteChannelFxns* pByteChannelFxns,
                    Uint16 rxBuffSize,
                    Uint16 rxBuffMax);

/*******************************************************************************
  Description: serves frames transmission and receive.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_process(struct BscChannel* pBscChannel);

#endif

// eof bsc_cntrl_fxns.h
