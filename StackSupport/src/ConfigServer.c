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
  ConfigServer.c
  
  This module contains global pararmeters and memory blocks assignment 
  for eZeeNet stack and framework.
********************************************************************************/

#include "NWK.h"
#include "ZDO.h"
#include "APS.h"
#include "Config.h"
#include "ConfigServer.h"

#ifndef _NO_FRAMEWORK_
  #include "fw_extdata.h"
#endif
#include "ConfigMacros.h"
#include "ConfigServerHeap.h"

/*******************************************************************************
  Description: get buffer from ConfigServer

  Parameters: moduleIndex - index of the requesting module,
              paramIndex  - index of the requested buffer.

  Returns: pointer to the buffer, must be cast to (void *).
*******************************************************************************/
int CServ__ConfigServer__getBuf(uint8_t moduleIndex, uint8_t paramIndex)
{
  void * ptrBuf = moduleBuf[moduleIndex];
  return /*(void *)*/ (((int*) ptrBuf)[paramIndex]);
}

/*******************************************************************************
  Description: get parameter from ConfigServer

  Parameters: moduleIndex - index of the requesting module,
              paramIndex  - index of the requested parameter.

  Returns: pointer to the parameter, must be cast to appropraite pointer type.
*******************************************************************************/
int CServ__ConfigServer__getParam(uint8_t moduleIndex, uint8_t paramIndex)
{
  void * ptrModule = moduleParam[moduleIndex];
  return /*(void *)*/ (((int*) ptrModule)[paramIndex]);
}




