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

/***********************************************************************
  circ_buf.h
  
  Macros for working with circular buffers
  
  Written by M.Balakhno
***********************************************************************/


#ifndef CIRCULAR_BUFFER_H
#define CIRCULAR_BUFFER_H


/*******************************************************************************
  Description:  Read next item from circular buffer. Buffer MUST NOT be empty.
                This macro returns item currently in tail and increments tail.
				Tail is wrapped if necessary.

  Parameters:   buf - buffer array
                tail - buffer tail variable
				len - total buffer size
				acc - acceptor of extracted value
  
  Returns:      Tail value
*******************************************************************************/
#define CIRC_BUFFER_READ(buf, tail, len, acc)	\
			( (acc) = (buf)[tail], (tail) = (((len)-1) == (tail)) ? (0) : ((tail) + 1) )

/*******************************************************************************
  Description:  Write item into buffer, placing it at head position. Buffer MUST NOT be full. 
                This macro writes *val* into buffer at position specified by *head* parameter
				and increments head. Head is wrapped if necessary.

  Parameters:   buf - buffer array
                head - buffer head variable
				len - total buffer size
				val - value to store in buffer
  
  Returns:      Nothing
*******************************************************************************/
#define CIRC_BUFFER_WRITE(buf, head, len, val)	\
			( (buf)[head] = (val), head = (((len)-1) == head) ? (0) : (head + 1) )

/*******************************************************************************
  Description:  Check if buffer is empty

  Parameters:   head - buffer head variable
				tail - buffer tail variable
				len - total buffer size

  Returns:      true if empty, false otherwise
*******************************************************************************/
#define CIRC_BUFFER_EMPTY(head, tail, len) 	\
            ( (head) == (tail) )

/*******************************************************************************
  Description:  Check if buffer is full

  Parameters:   head - buffer head variable
				tail - buffer tail variable
				len - total buffer size

  Returns:      true if full, false otherwise
*******************************************************************************/
#define CIRC_BUFFER_FULL(head, tail, len) 	\
            ( (tail == 0) ? (head == ((len)-1)) : ( head == (tail-1)) )

/*******************************************************************************
  Description:  Init circular buffer head and tail pointers

  Parameters:   head - buffer head variable
				tail - buffer tail variable

  Returns:      Nothing
*******************************************************************************/
#define CIRC_BUFFER_INIT(head, tail)	\
			(head = 0, tail = 0)


/*******************************************************************************
  Description:  Returns item count currently in buffer

  Parameters:   head - buffer head variable
				tail - buffer tail variable
				len - total buffer size

  Returns:      Nothing
*******************************************************************************/
#define CIRC_BUFFER_COUNT(head, tail, len)	\
			( (head >= tail) ? (head - tail) : ((len) - tail + head) )
#endif

// eof circ_buf.h
