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
  bsc_channel.c
  
  BSC channel implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "bsc_cntrl_fxns.h"
#include "com_assert.h"

enum
{
  DLE = 0x10,
  STX = 0x02,
  ETX = 0x03,
};

// Tries to get RX buffer.
static void getRxBuff(struct BscChannel*);
// Serves incoming frame.
static void srvIncomingFrame(struct BscChannel*);
// Serves outgoing frame.
static void srvOutgoingFrame(struct BscChannel*);

/*******************************************************************************
  Description: writes outgoing frame to BSC channel.
  
  Parameters: pBscChannel - BSC channel reference.
              pBuffDescr - outgoing frame buffer descriptor.
  
  Returns: nothing.
*******************************************************************************/
static void write(struct BscChannel* pBscChannel, BuffDescr* pBuffDescr)
{
  ASSERT((0 != pBscChannel) && (0 != pBuffDescr));
  list_queueTail(&pBscChannel->txPendBuffList, (ListItem*)pBuffDescr);
}

/*******************************************************************************
  Description: reads incoming frame from BSC channel.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: incoming frame buffer desriptor reference if there is an incoming frame
           and 0, otherwise.
*******************************************************************************/
static BuffDescr* read(struct BscChannel* pBscChannel)
{
  ListItem* pListItem;
  
  ASSERT(0 != pBscChannel);
  pListItem = list_dequeueHead(&pBscChannel->rxPendBuffList);
  return (BuffDescr*)pListItem;
}

/*******************************************************************************
  Description: provides reference to BSC channel interface.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_getFxns(MsgChannelFxns* pMsgChannelFxns)
{
  pMsgChannelFxns->read = (BuffDescr* (*)(struct MsgChannel*))read;
  pMsgChannelFxns->write = (void (*)(struct MsgChannel*, BuffDescr*))write;
}

/*******************************************************************************
  Description: inits BSC channel.
  
               Attention: rxBuffSize patrameter must be initialized with
               BSC_RX_BUFFER_SIZE macros.
               rxBuffMax patrameter must be initialized with
               BSC_RX_BUFFER_MAX macros.
  
  Parameters: pBscChannel - BSC channel reference.
              pByteChannelFxns - byte channel interface reference.
              rxBuffSize - size of RX buffer.
              rxBuffMax - number of RX buffers.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_init(struct BscChannel* pBscChannel,
                    const ByteChannelFxns* pByteChannelFxns,
                    Uint16 rxBuffSize,
                    Uint16 rxBuffMax)
{
  ASSERT((0 != pBscChannel) && (0 != pByteChannelFxns) &&
         (0 != rxBuffSize) && (0 != rxBuffMax));
  // Save byte channel interface reference.
  pBscChannel->byteChannelFxns = *pByteChannelFxns;
  {
    // Init RX buffers descriptors.
    Uint8 buffInd;
    
    pBscChannel->rxBuffSize = rxBuffSize;
    pBscChannel->rxBuffMax = rxBuffMax;
    for (buffInd = 0; buffInd < rxBuffMax; buffInd++)
    {
      buffDescr_init(&pBscChannel->rxBuffDescr[buffInd],
                     pBscChannel->rxBuff + rxBuffSize * buffInd,
                     rxBuffSize);
    }
  }
  // Clear active RX and TX descriptors, states.
  pBscChannel->activeRxBuffDescr = 0;
  pBscChannel->activeTxBuffDescr = 0;
  pBscChannel->rxState = BSC_IDLE_RXSTATE;
  pBscChannel->txState = BSC_IDLE_TXSTATE;
}

/*******************************************************************************
  Description: serves frames transmission and receive.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
void bscChannel_process(struct BscChannel* pBscChannel)
{
  ASSERT(0 != pBscChannel);
  // Serve transmission.
  if (0 == pBscChannel->activeTxBuffDescr)
    // There is no a frame to transmit.
    // Check outgoing frames queue.
    pBscChannel->activeTxBuffDescr = (BuffDescr*)list_dequeueHead(&pBscChannel->txPendBuffList);
  if (0 != pBscChannel->activeTxBuffDescr)
    // There is a frame to transmit.
    // Serve outgoing frame.
    srvOutgoingFrame(pBscChannel);
  // Serve receive.
  if (0 == pBscChannel->activeRxBuffDescr)
    // Try to get RX buffer.
    getRxBuff(pBscChannel);
  if (0 != pBscChannel->activeRxBuffDescr)
    // There is buffer for receive.
    // Serve incoming frame.
    srvIncomingFrame(pBscChannel);
}

/*******************************************************************************
  Description: tries to get free RX buffer. Can change active RX buffer 
               descriptor value.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void getRxBuff(struct BscChannel* pBscChannel)
{
  Uint8 ind;
    
  for (ind = 0; ind < pBscChannel->rxBuffMax; ind++)
  {
    if (!pBscChannel->rxBuffDescr[ind].busy)
    {
      pBscChannel->activeRxBuffDescr = &pBscChannel->rxBuffDescr[ind];
      break;
    }
  }
}

/*******************************************************************************
  Description: serves incoming frame.
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void srvIncomingFrame(struct BscChannel* pBscChannel)
{
  Uint8 data;
    
  ASSERT(0 != pBscChannel->activeRxBuffDescr);
  while (0 != (data = pBscChannel->byteChannelFxns.read(pBscChannel->byteChannelFxns.pByteChannel, &data, 1)))
  {
    switch (pBscChannel->rxState)
    {
      case BSC_IDLE_RXSTATE:
           if (DLE == data)
           {
             // Start frame receive.
             pBscChannel->rxCrc = data;
             pBscChannel->rxState = BSC_STX_RXSTATE;
           }
           break;
             
      case BSC_STX_RXSTATE:
           if (STX == data)
           {
             pBscChannel->rxCrc += data;                // Correct CRC.
             pBscChannel->rxState = BSC_DATA_RXSTATE;   // Header has been received. Ready to get data.
             // Init buffer descriptor and data pointer.
             buffDescr_setRoom(pBscChannel->activeRxBuffDescr, 0, pBscChannel->rxBuffSize);
             pBscChannel->activeRxBuffPtr = pBscChannel->activeRxBuffDescr->data;
           }
           else
             // Invalid byte. Revert to header waiting. 
             pBscChannel->rxState = BSC_IDLE_RXSTATE;
           break;
        
      case BSC_DATA_RXSTATE:
           pBscChannel->rxCrc += data; // Correct CRC.
           if (DLE == data)
           {
             // DLE in user data.
             pBscChannel->rxState = BSC_DLE_DATA_RXSTATE;
           }
           else
           {
             // There is no DLE.
             // // Reserve room for 1 byte in buffer and save it.
             buffDescr_put(pBscChannel->activeRxBuffDescr, 1); 
             *pBscChannel->activeRxBuffPtr = data;
             pBscChannel->activeRxBuffPtr++;

           }
           break;
             
      case BSC_DLE_DATA_RXSTATE:
           pBscChannel->rxCrc += data; // Correct CRC.
           if (DLE == data)
           {
             // // Reserve room for 1 byte in buffer and save it.
             buffDescr_put(pBscChannel->activeRxBuffDescr, 1);
             *pBscChannel->activeRxBuffPtr = data;
             pBscChannel->activeRxBuffPtr++;
             pBscChannel->rxState = BSC_DATA_RXSTATE; // Again ready to get data.
           }
           else
             if (ETX == data)
               pBscChannel->rxState = BSC_ETX_RXSTATE;
             else
               pBscChannel->rxState = BSC_IDLE_RXSTATE;
           break;
             
      case BSC_ETX_RXSTATE:
           pBscChannel->rxState = BSC_IDLE_RXSTATE;
           if (data == pBscChannel->rxCrc)
           {
             // CRC is correct.
             // Mark buffer as busy.
             pBscChannel->activeRxBuffDescr->busy = true;
             // Add to RX pending list.
             list_queueTail(&pBscChannel->rxPendBuffList, (ListItem*)pBscChannel->activeRxBuffDescr);
             // Reset active RX buff descriptor.
             pBscChannel->activeRxBuffDescr = 0;
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
  
  Parameters: pBscChannel - BSC channel reference.
  
  Returns: nothing.
*******************************************************************************/
static void srvOutgoingFrame(struct BscChannel* pBscChannel)
{
  ASSERT(0 != pBscChannel->activeTxBuffDescr);
  if (BSC_IDLE_TXSTATE == pBscChannel->txState)
  {
    // Set start position in TX buffer.
    pBscChannel->txBuffPtr = pBscChannel->activeTxBuffDescr->data;
    pBscChannel->currTxByte = DLE;
    pBscChannel->txCrc = 0;
    pBscChannel->txState = BSC_DLE_STX_TXSTATE;
  }
  while (0 != pBscChannel->byteChannelFxns.write(pBscChannel->byteChannelFxns.pByteChannel,
                                                 &pBscChannel->currTxByte,
                                                 1))
  {
    pBscChannel->txCrc += pBscChannel->currTxByte;
    switch (pBscChannel->txState)
    {
      case BSC_DLE_STX_TXSTATE:
           pBscChannel->currTxByte = STX;
           pBscChannel->txState = BSC_STX_TXSTATE;
           break;
         
      case BSC_STX_TXSTATE:
      case BSC_DATA_TXSTATE:
           if (pBscChannel->txBuffPtr < pBscChannel->activeTxBuffDescr->tail)
           {
             pBscChannel->currTxByte = *pBscChannel->txBuffPtr;
             if (DLE == pBscChannel->currTxByte)
               pBscChannel->txState = BSC_DLE_DATA_TXSTATE;
             else
               pBscChannel->txState = BSC_DATA_TXSTATE; 
             pBscChannel->txBuffPtr++;
           }
           else
           {
             pBscChannel->currTxByte = DLE;
             pBscChannel->txState = BSC_DLE_ETX_TXSTATE;
           }
           break;
         
      case BSC_DLE_DATA_TXSTATE:
           pBscChannel->txState = BSC_DATA_TXSTATE; 
           break;
         
      case BSC_DLE_ETX_TXSTATE:
           pBscChannel->currTxByte = ETX;
           pBscChannel->txState = BSC_ETX_TXSTATE;
           break;
      
      case BSC_ETX_TXSTATE:
           pBscChannel->currTxByte = pBscChannel->txCrc;
           pBscChannel->txState = BSC_CRC_TXSTATE;
           break;
         
      case BSC_CRC_TXSTATE:
           pBscChannel->activeTxBuffDescr->busy = false;
           pBscChannel->activeTxBuffDescr = 0;
           return;
         
      default: 
           ASSERT(false);
    }
  }
}

// eof bsc_channel.c
