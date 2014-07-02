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
  buff_descr.c
  
  Buffer descriptor capability implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "buff_descr.h"
#include "com_assert.h"
#include <string.h>

/*******************************************************************************
  Description: inits buffer descriptor with buffer reference and size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              buffer - buffer reference.
              size - buffer size.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_init(BuffDescr* pBuffDescr, Uint8* buffer, Uint16 size)
{
  ASSERT((0 != pBuffDescr) && buffer && size);
  pBuffDescr->head = buffer;
  pBuffDescr->data  = buffer;
  pBuffDescr->end  = buffer + size;
  pBuffDescr->tail = buffer;
  pBuffDescr->busy = false;
  pBuffDescr->listItem.next = 0;
  pBuffDescr->listItem.prev = 0;
}

/*******************************************************************************
  Description: sets header and footer.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              headerSize - header reference.
              footerSize - footer size.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_setRoom(BuffDescr* pBuffDescr, Uint16 headerSize, Uint16 footerSize)
{
  ASSERT(0 != pBuffDescr);
  {
    Uint8* newDataPtr = pBuffDescr->head + headerSize;
    Uint8* newTailPtr = pBuffDescr->end - footerSize;
    
    ASSERT((newDataPtr >= pBuffDescr->head) &&
           (newTailPtr <= pBuffDescr->end) &&
           (newDataPtr <= newTailPtr));
    pBuffDescr->data = newDataPtr;
    pBuffDescr->tail = newTailPtr;
  }
}

/*******************************************************************************
  Description: increments header to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - header size increment.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_push(BuffDescr* pBuffDescr, Uint16 size)
{
  ASSERT(0 != pBuffDescr);
  {
    Uint8* newDataPtr = pBuffDescr->data - size;
    
    ASSERT(size &&
           (pBuffDescr->data > newDataPtr) &&
           (newDataPtr >= pBuffDescr->head));
    pBuffDescr->data = newDataPtr;
  }
}

/*******************************************************************************
  Description: decrements header to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - header size decrement.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_pull(BuffDescr* pBuffDescr, Uint16 size)
{
  ASSERT(0 != pBuffDescr);
  {
    Uint8* newDataPtr = pBuffDescr->data + size;
    
    ASSERT(size && (newDataPtr <= pBuffDescr->tail));
    pBuffDescr->data = newDataPtr;
  }
}

/*******************************************************************************
  Description: increments footer to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - footer size increment.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_put(BuffDescr* pBuffDescr, Uint16 size)
{
  ASSERT(0 != pBuffDescr);
  {
    Uint8* newTailPtr = pBuffDescr->tail + size;
    
    ASSERT(size && (newTailPtr <= pBuffDescr->end));
    pBuffDescr->tail = newTailPtr;
  }
}

/*******************************************************************************
  Description: decrements footer to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - footer size decrement.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_trim(BuffDescr* pBuffDescr, Uint16 size)
{
  ASSERT(0 != pBuffDescr);
  {
    Uint8* newTailPtr = pBuffDescr->tail - size;
    
    ASSERT(size &&
           (pBuffDescr->tail > newTailPtr) &&
           (newTailPtr >= pBuffDescr->data));
    pBuffDescr->tail = newTailPtr;
  }
}

/*******************************************************************************
  Description: fills buffer with data. Provides detection data area violation,
               i.e. area between data and tail pointers. Violation is indicated
               by ASSERT function.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              dst - destination buffer reference. It is reference in buffer that
                    is described by pBuffDescr.
              src - source buffer reference.
              length - data length.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_putData(BuffDescr* pBuffDescr, Uint8* dst, Uint8* src, Uint16 length)
{
  ASSERT((0 != pBuffDescr) && dst && src && length && (dst >= pBuffDescr->data) &&
                                               ((dst + length) <= pBuffDescr->tail));
  memcpy(dst, src, length);
}

/*******************************************************************************
  Description: gets data from buffer.  Provides detection data area violation,
               i.e. area between data and tail pointers. Violation is indicated
               by ASSERT function.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              dst - destination buffer reference.
              src - source buffer reference.  It is reference in buffer that
                    is described by pBuffDescr.
              length - data length to get.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_getData(BuffDescr* pBuffDescr, Uint8* dst, Uint8* src, Uint16 length)
{
  ASSERT((0 != pBuffDescr) && dst && src && length && (src >= pBuffDescr->data) &&
                                               ((src + length) <= pBuffDescr->tail));
  memcpy(dst, src, length);
}

// eof buff_descr.c
