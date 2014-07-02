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
  buff_descr.h
  
  Buffer descriptor capability declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef BUFF_DESCR_H_
#define BUFF_DESCR_H_

#include "com_types.h"
#include "com_list.h"

/*******************************************************************************
  General description.
  
  Buffer descriptor is intended for providing support of buffers using.
  
  General use can be presented in the following way.
  
  Allocation and initialization.
  At first supplier provides buffer and buffer description allocation. Then
  supplier inits buffer descriptor (buffDescr_init() method). head and end
  fileds of BuffDescr structure point to begin and end of buffer accordingly.
  
  Data area.
  It occupies space between limits pointed by data and tail fields of 
  BuffDescr structure. After initialization they both point to begin of
  buffer. Data area can be changed (as length as location) by means of 
  buffDescr_setRoom(), buffDescr_push(), buffDescr_pull(), buffDescr_put(),
  buffDescr_trim() methods. BuffDescr structure doesn't contain data area
  size explicitly as it is defined by data and tail fileds of BuffDescr
  structure.
  
  Data saving.
  For relatively safe means of saving data in buffer buffDescr_putData() 
  method is used. 
  
  Data getting.
  For relatively safe means of getting data from buffer buffDescr_getData() 
  method is used.
  
  Buffer occupation and release.
  After data saving supplier sets busy flag in true and passes buffer
  possession by pass the reference to user. After that supplier waits for
  buffer release by check busy flag. It should be set by user to false. After
  that user may not work with the buffer (and buffer descriptor too). Usually
  supplier has buffers and buffers descriptor organized in array whereas user
  uses buffer descriptors organized in list.
  
  Notes:
    busy flag is only one field of BuffDescr struct that is manipulated directly
    by user or supplier. Other fileds are changed indirectly by methods. 
*******************************************************************************/

typedef struct // Buffer descriptor declaration.
{
  ListItem listItem; // Must be the first! Provides buffer descriptors
                     // connectivity in list.
  Uint8* head;       // Buffer head.
  Uint8* data;       // User's data head in buffer.
  Uint8* tail;       // User's data end in buffer.
  Uint8* end;        // Buffer end.
  Bool busy;         // Descripted buffer as well this descriptor are in use.
} BuffDescr;

/*******************************************************************************
  Description: inits buffer descriptor with buffer reference and size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              buffer - buffer reference.
              size - buffer size.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_init(BuffDescr* pBuffDescr, Uint8* buffer, Uint16 size);

/*******************************************************************************
  Description: sets header and footer.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              headerSize - header reference.
              footerSize - footer size.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_setRoom(BuffDescr* pBuffDescr, Uint16 headerSize, Uint16 footerSize);

/*******************************************************************************
  Description: increments header to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - header size increment.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_push(BuffDescr* pBuffDescr, Uint16 size);

/*******************************************************************************
  Description: decrements header to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - header size decrement.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_pull(BuffDescr* pBuffDescr, Uint16 size);

/*******************************************************************************
  Description: increments footer to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - footer size increment.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_put(BuffDescr* pBuffDescr, Uint16 size);

/*******************************************************************************
  Description: decrements footer to size.
    
  Parameters: pBuffDescr - buffer descriptor reference.
              size - footer size decrement.
  
  Returns: nothing.
*******************************************************************************/
void buffDescr_trim(BuffDescr* pBuffDescr, Uint16 size);

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
void buffDescr_putData(BuffDescr* pBuffDescr, Uint8* dst, Uint8* src, Uint16 length);

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
void buffDescr_getData(BuffDescr* pBuffDescr, Uint8* dst, Uint8* src, Uint16 length);

#endif

// buff_descr.h
