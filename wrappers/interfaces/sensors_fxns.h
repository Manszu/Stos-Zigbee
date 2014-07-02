/**
 * Copyright (c) 2005-2006 LuxLabs Ltd., All Rights Reserved.
 *
 * Redistribution, sublicensing and use in source and binary forms
 * are permitted provided that the following conditions are met:
 *
 * -Redistributions, sublicensing of source code is allowed for authorised companies
 *  only and must retain the above copyright notice,
 *  this list of conditions and the following disclaimer.
 *
 * -Redistribution, sublicensing in binary form is allowed for authorised companies
 *  only and must reproduce the above copyright
 *  notice, this list of conditions and the following disclaimer in
 *  the documentation and/or other materials provided with the distribution.
 *
 * No changes (alteration, merge, modification, reverse engineer) can be applied
 * to the source or binary code without specific prior written permission.
 *
 * Neither the name of LuxLabs Ltd./MeshNetics or the names of contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * This software is provided "AS IS," without a warranty of any kind.
 * ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING
 * ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. LUXLABS LTD. AND ITS LICENSORS SHALL NOT
 * BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
 * OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS
 * DERIVATIVES. IN NO EVENT WILL LUXLABS OR ITS LICENSORS BE LIABLE FOR ANY LOST
 * REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, CONSEQUENTIAL,
 * INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS OF THE THEORY
 * OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE SOFTWARE, EVEN
 * IF LUXLABS Ltd. HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 *
 * You acknowledge that Software is not designed, licensed or intended for
 * use in the design, construction, operation or maintenance of any nuclear
 * facility, medical or other mission-critical system.
 **/

/***********************************************************************
  sensors_fxns.h
  
  Declares sensors interface.
  
  Written by K.Kuzmin.
***********************************************************************/

#ifndef SENSORS_FXNS_H_
#define SENSORS_FXNS_H_

#include "com_types.h"

typedef float SensorsData;

struct Sensors;

typedef struct
{
  struct Sensors* pSensors;

  void (*open)(struct Sensors* pSensors, Uint8 id);
  void (*close)(struct Sensors* pSensors, Uint8 id);
  Bool (*start)(struct Sensors* pSensors, Uint8 id);
  Bool (*get)(struct Sensors* pSensors, Uint8 id, SensorsData* data);
} SensorsFxns;

#endif

// eof sensors_fxns.h
