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
  sbuf.h
  
  Data buffer descriptor (Sbuf_t structure) handling functions - declaration.
********************************************************************************/
 

#ifndef _SBUF_H_
#define _SBUF_H_

#include "tos.h"

typedef struct 
{
  uint8_t * head;
  uint8_t * data;     
  uint8_t * tail;     
  uint8_t * end;      
  uint8_t   handler;  
  bool      busy;     
} Sbuf_t;

/*******************************************************************************
  Description: init sbuf structure and adjust it to the buffer

  Parameters: buf    -  pointer to sbuf structure,
              buffer -  buffer pointer,
              len    -  buffer length.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__buf__init(Sbuf_t* buf, uint8_t* buffer, uint8_t len);

/*******************************************************************************
  Description: set header and footer size for sbuf

  Parameters: buf    -  pointer to sbuf structure,
              header - header size,
              footer - footer size.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__buf__setRoom(Sbuf_t* sb, uint8_t header, uint8_t footer);

/*******************************************************************************
  Description: increase header size

  Parameters: buf    -  pointer to sbuf structure,
              header - number of bytes to increase.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__header__pop(Sbuf_t* sb, uint8_t header);

/*******************************************************************************
  Description: decrease header size

  Parameters: buf    -  pointer to sbuf structure,
              header - number of bytes to decrease.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__header__push(Sbuf_t* sb, uint8_t header);

/*******************************************************************************
  Description: increase footer size

  Parameters: buf    -  pointer to sbuf structure,
              footer - number of bytes to increase.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__footer__pop(Sbuf_t* sb, uint8_t footer);

/*******************************************************************************
  Description: decrease footer size

  Parameters: buf    -  pointer to sbuf structure,
              footer - number of bytes to decrease.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__footer__push(Sbuf_t* sb, uint8_t footer);

/*******************************************************************************
  Description: put data into the data section, bounds check is performed.

  Parameters: dst - pointer to the buffer to copy to (must be inside data room), 
              src - pointer to the buffer to copy from,
              len - number of bytes to copy.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__data__put(Sbuf_t* sb, uint8_t* dst, uint8_t* src, uint8_t len);

/*******************************************************************************
  Description: put byte into the data section, bounds check is performed.

  Parameters: dst  - pointer to the buffer to copy to (must be inside data room),
              byte - byte to put.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__data__putByte(Sbuf_t* sb, uint8_t* dst, uint8_t byte);

/*******************************************************************************
  Description: set data section lenght (increasing footer size).

  Parameters: dst - pointer to the buffer to copy to (must be inside data room),
              len - data section length to set.

  Returns: FAIL if illegal parameters passed, SUCCESS othervise.
*******************************************************************************/
result_t sbuf__data__setLength(Sbuf_t* sb, uint8_t len);

#endif // _SBUF_H_
