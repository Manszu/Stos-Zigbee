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
  sbuf_code.c

  Data buffer descriptor (Sbuf_t structure) handling functions - implementation.
********************************************************************************/

/*******************************************************************************
  Description: init sbuf structure and adjust it to the buffer

  Parameters: buf    -  pointer to sbuf structure,
              buffer -  buffer pointer,
              len    -  buffer length.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(buf,init)(Sbuf_t* sb, uint8_t* buffer, uint8_t len)
  {
    if (NULL == sb || NULL == buffer)
      return FAIL;
    sb->head = buffer;
    sb->data  = buffer;
    sb->end  = buffer + len;
    sb->tail = buffer + len;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: set header and footer size for sbuf

  Parameters: buf    -  pointer to sbuf structure,
              header - header size,
              footer - footer size.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(buf,setRoom)(Sbuf_t* sb, uint8_t header, uint8_t footer)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->head + header >= sb->end - footer)
      return FAIL;
    sb->data = sb->head + header;
    sb->tail = sb->end - footer;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: increase header size

  Parameters: buf    -  pointer to sbuf structure,
              header - number of bytes to increase.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(header,pop)(Sbuf_t* sb, uint8_t header)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->data + header >= sb->tail)
      return FAIL;
    sb->data = sb->data + header;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: decrease header size

  Parameters: buf    -  pointer to sbuf structure,
              header - number of bytes to decrease.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(header,push)(Sbuf_t* sb, uint8_t header)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->data - header < sb->head)
      return FAIL;
    sb->data = sb->data - header;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: increase footer size

  Parameters: buf    -  pointer to sbuf structure,
              footer - number of bytes to increase.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(footer,pop)(Sbuf_t* sb, uint8_t footer)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->tail - footer < sb->data)
      return FAIL;
    sb->tail = sb->tail - footer;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: decrease footer size

  Parameters: buf    -  pointer to sbuf structure,
              footer - number of bytes to decrease.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(footer,push)(Sbuf_t* sb, uint8_t footer)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->tail + footer > sb->end)
      return FAIL;
    sb->tail = sb->tail + footer;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: put data into the data section, bounds check is performed.

  Parameters: dst - pointer to the buffer to copy to (must be inside data room), 
              src - pointer to the buffer to copy from,
              len - number of bytes to copy.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(data,put)(Sbuf_t* sb, uint8_t* dst, uint8_t* src, uint8_t len)
  {
    if (NULL == sb || NULL == dst || NULL == src)
      return FAIL;
   if (dst < sb->data || dst + len > sb->tail)
      return FAIL;
    memcpy(dst, src, len);
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: put byte into the data section, bounds check is performed.

  Parameters: dst  - pointer to the buffer to copy to (must be inside data room),
              byte - byte to put.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(data,putByte)(Sbuf_t* sb, uint8_t* dst, uint8_t byte)
  {
    if (NULL == sb || NULL == dst)
      return FAIL;
    if (dst < sb->data || dst + 1 > sb->tail)
      return FAIL;
    *dst = byte;
    return SUCCESS;
  }
  
/*******************************************************************************
  Description: set data section lenght (increasing footer size).

  Parameters: dst - pointer to the buffer to copy to (must be inside data room),
              len - data section length to set.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
  SBUF_OP(data,setLength)(Sbuf_t* sb, uint8_t len)
  {
    if (NULL == sb)
      return FAIL;
    if (sb->data + len > sb->end)
      return FAIL;
    sb->tail = sb->data + len;
    return SUCCESS;
  }

//eof

