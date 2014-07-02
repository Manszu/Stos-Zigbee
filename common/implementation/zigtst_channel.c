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
  zigtst_channel.c
  
  ZigTest channel implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "msg_channel_fxns.h"
#include "zigtst_cntrl_fxns.h"
#include "com_assert.h"

// Tries to get RX buffer.
static void getRxBuff(struct ZigTstChannel*);
// Serves incoming frame.
static void srvIncomingFrame(struct ZigTstChannel*);
// Serves outgoing frame.
static void srvOutgoingFrame(struct ZigTstChannel*);

/*******************************************************************************
  Description: writes the message to the channel.
    
  Parameters: pZigTstChannel - channel reference.
              pBuffDescr     - buffer descriptor for message.
  
  Returns: nothing.
*******************************************************************************/
static void write(struct ZigTstChannel* pZigTstChannel, BuffDescr* pBuffDescr)
{
  list_queueTail(&pZigTstChannel->txPendBuffList, (ListItem*)pBuffDescr);
}

/*******************************************************************************
  Description: reads incoming message from the channel.
    
  Parameters: pZigTstChannel - channel reference.
  
  Returns: message buffer descriptor reference if there is a message 
           and 0, otherwise.
*******************************************************************************/
static BuffDescr* read(struct ZigTstChannel* pZigTstChannel)
{
  BuffDescr* pRxBuffDescr;
  
  pRxBuffDescr = (BuffDescr*)list_dequeueHead(&pZigTstChannel->rxPendBuffList);
  return pRxBuffDescr;
}

/*******************************************************************************
  Description: returns reference to message channel interface.
    
  Parameters: pMsgChannelFxns - message channel reference.
  
  Returns: nothing.
*******************************************************************************/
void zigTstChannel_getFxns(MsgChannelFxns* pMsgChannelFxns)
{
  pMsgChannelFxns->write = (void (*)(struct MsgChannel*, BuffDescr*))write;
  pMsgChannelFxns->read = (BuffDescr* (*)(struct MsgChannel*))read;
}

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
                       Uint16 rxBuffMax)
{
  ASSERT((0 != pZigTstChannel) && (0 != pByteChannelFxns) &&
         (0 != rxBuffSize) && (0 != rxBuffMax));
  
  // Save byte channel interface reference.
  pZigTstChannel->byteChannelFxns = *pByteChannelFxns;
  
  {
    // Init RX buffers descriptors.
    Uint8 buffInd;
    
    pZigTstChannel->rxBuffSize = rxBuffSize;
    pZigTstChannel->rxBuffMax = rxBuffMax;
    for (buffInd = 0; buffInd < rxBuffMax; buffInd++)
    {
      buffDescr_init(&pZigTstChannel->rxBuffDescr[buffInd],
                     pZigTstChannel->rxBuff + rxBuffSize * buffInd,
                     rxBuffSize);
    }
  }
  
  // Clear active RX and TX descriptors, states.
  pZigTstChannel->activeRxBuffDescr = 0;
  pZigTstChannel->activeTxBuffDescr = 0;
  pZigTstChannel->rxState = ZIGTST_RXSTATE_IDLE;
  pZigTstChannel->txState = ZIGTST_TXSTATE_IDLE;
}

/*******************************************************************************
  Description: ZigTest channel main loop.
    
  Parameters: pZigTstChannel - ZigTest channel reference.
  
  Returns: nothing.
*******************************************************************************/
void zigTstChannel_process(struct ZigTstChannel* pZigTstChannel)
{
  ASSERT(0 != pZigTstChannel);
  // Serve transmission.
  if (0 == pZigTstChannel->activeTxBuffDescr)
    // There is no a frame to transmit.
    // Check outgoing frames queue.
    pZigTstChannel->activeTxBuffDescr = (BuffDescr*)list_dequeueHead(&pZigTstChannel->txPendBuffList);
  if (0 != pZigTstChannel->activeTxBuffDescr)
    // There is a frame to transmit.
    // Serve outgoing frame.
    srvOutgoingFrame(pZigTstChannel);
  // Serve receive.
  if (0 == pZigTstChannel->activeRxBuffDescr)
    // Try to get RX buffer.
    getRxBuff(pZigTstChannel);
  if (0 != pZigTstChannel->activeRxBuffDescr)
    // There is buffer for receive.
    // Serve incoming frame.
    srvIncomingFrame(pZigTstChannel);
}

/*******************************************************************************
  Description: tries to get free RX buffer. Can change active RX buffer 
               descriptor value.
  
  Parameters: pZigTstChannel - ZigTest channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void getRxBuff(struct ZigTstChannel* pZigTstChannel)
{
  Uint8 ind;
    
  for (ind = 0; ind < pZigTstChannel->rxBuffMax; ind++)
  {
    if (!pZigTstChannel->rxBuffDescr[ind].busy)
    {
      pZigTstChannel->activeRxBuffDescr = &pZigTstChannel->rxBuffDescr[ind];
      break;
    }
  }
}

/*******************************************************************************
  Description: serves incoming frame.
  
  Parameters: pZigTstChannel - ZigTestgraph display msgChannel at (384, 320) now or when in appClock__Clock__HandleFire
   channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void srvIncomingFrame(struct ZigTstChannel* pZigTstChannel)
{
  Uint8 data;
    
  ASSERT(0 != pZigTstChannel->activeRxBuffDescr);
  while (0 != (pZigTstChannel->byteChannelFxns.read(pZigTstChannel->byteChannelFxns.pByteChannel, &data, 1)))
  {
    switch (pZigTstChannel->rxState)
    {
      case ZIGTST_RXSTATE_IDLE:
           // Start frame receive.
           pZigTstChannel->rxLength = data;
           // Init buffer descriptor and data pointer.
           buffDescr_setRoom(pZigTstChannel->activeRxBuffDescr, 0, pZigTstChannel->rxBuffSize - data);
           pZigTstChannel->rxBuffPtr = pZigTstChannel->activeRxBuffDescr->data;
           pZigTstChannel->rxState = ZIGTST_RXSTATE_DATA;
           break;

      case ZIGTST_RXSTATE_DATA:
           *pZigTstChannel->rxBuffPtr = data;
           pZigTstChannel->rxBuffPtr++;
           pZigTstChannel->rxLength--;
           if (0 == pZigTstChannel->rxLength)
           {
             // Mark buffer as busy.
             pZigTstChannel->activeRxBuffDescr->busy = true;
             // Add to RX pending list.
             list_queueTail(&pZigTstChannel->rxPendBuffList, (ListItem*)pZigTstChannel->activeRxBuffDescr);
             // Reset active RX buff descriptor.
             pZigTstChannel->activeRxBuffDescr = 0;
             pZigTstChannel->rxState = ZIGTST_RXSTATE_IDLE;
             return;
           }
           break;

      default:
           ASSERT(false);
    }
  }
}

/*******************************************************************************
  Description: serves outgoing frame.
  
  Parameters: pZigTstChannel - ZigTest channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void srvOutgoingFrame(struct ZigTstChannel* pZigTstChannel)
{
  ASSERT(0 != pZigTstChannel->activeTxBuffDescr);
  if (ZIGTST_TXSTATE_IDLE == pZigTstChannel->txState)
  {
    // Set start position in TX buffer.
    pZigTstChannel->txBuffPtr = pZigTstChannel->activeTxBuffDescr->data;
    // First byte - message length.
    pZigTstChannel->currTxByte = pZigTstChannel->activeTxBuffDescr->tail - 
                                pZigTstChannel->activeTxBuffDescr->data;
    pZigTstChannel->txState = ZIGTST_TXSTATE_TX;
  }
  while (0 != pZigTstChannel->byteChannelFxns.write(pZigTstChannel->byteChannelFxns.pByteChannel,
                                                    &pZigTstChannel->currTxByte,
                                                    1))
  {
    if (pZigTstChannel->txBuffPtr < pZigTstChannel->activeTxBuffDescr->tail)
    {
      pZigTstChannel->currTxByte = *pZigTstChannel->txBuffPtr;
      pZigTstChannel->txBuffPtr++;
    }
    else
    {
      pZigTstChannel->activeTxBuffDescr->busy = false;
      pZigTstChannel->activeTxBuffDescr = 0;
      pZigTstChannel->txState = ZIGTST_TXSTATE_IDLE;
      return;
    }      
  }
}

// eof zigtst_channel.c
