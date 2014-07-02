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
  ConfigMacros.h
  
  Macros for configuration select (coordinator/router/TOSSIM)
********************************************************************************/

#ifndef _CONFIGMACROS_H_
#define _CONFIGMACROS_H_

#ifdef _TOSSIM_
  #define BEGIN_COORD_SECTION     if(gNodeType == ZIGBEE_COORDINATOR_TYPE) {
  #define BEGIN_ROUTER_SECTION    if(gNodeType == ZIGBEE_ROUTER_TYPE) { 
  #define BEGIN_ENDDEV_SECTION      if(gNodeType == ZIGBEE_END_DEVICE_TYPE) { 
  #define ELSE_SECTION            } else {
  #define END_SECTION             }
#else
  #define BEGIN_COORD_SECTION  
  #define BEGIN_ROUTER_SECTION
  #define BEGIN_ENDDEV_SECTION
  #define ELSE_SECTION
  #define END_SECTION
#endif

  #if (defined _ROUTER_) || (defined _COORDINATOR_)
    #define _CHILDREN_EXIST_
  #endif

  #if (defined _ROUTER_) || (defined _ZBED_)
    #define _PARENT_EXIST_
  #endif

#ifndef PACK
#ifdef PACK_STRUCTURES
#define PACK __attribute__ ((packed))
#else
#define PACK
#endif
#endif

  
#endif//_CONFIGMACROS_H_
