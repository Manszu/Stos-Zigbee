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
**  Types.h
**  
**  eZeeNet framework: common data types
*******************************************************************************/

#ifndef _TYPES_H_
#define _TYPES_H_

#include "stack.h"
#include <stddef.h>
/*****************************************************************************
                               Common types
*****************************************************************************/
typedef uint16_t       NodeAddress;

#if 0
  typedef signed char    int8;
  typedef unsigned char  uint8;
  typedef unsigned long  uint32;
  typedef unsigned short uint16;
  typedef long           int32;
  typedef short          int16;
  
  typedef struct 
  {
    uint32  lo;
    uint32  hi;
  } uint64;
  
  typedef struct 
  {
    uint64 lo;
    uint64  hi;
  } uint128;
#endif // endif

#ifndef false
  #define false 0
#endif

#ifndef true
  #define true 1
#endif

#if 0
  #define externalCprogram 

  #undef EXTERN_C 
  #define EXTERN_C externalCprogram 

  /*    Assertion support                   */
  #ifndef ASSERT
    #include <assert.h>
    #ifdef _DEBUG
      #define ASSERT(x)  assert(x)
    #else
      #define ASSERT(_ignore)  ((void)0)
    #endif  /* _DEBUG */
  #endif  /* ASSERT */
  
  /*** Inline methods definition ***/
  #undef inline_
  #if (defined COMPILER_MSVC)||(defined COMPILER_ARM)
    #define inline_ __inline
  #else
    #define inline_ static inline
  #endif
  
  #ifndef NO_COMFUNCTIONS
  //#include "comfunc.h"
  #endif //NO_COMFUNCTIONS
#endif

#endif /* __TYPES_H */

// eof Types.h
