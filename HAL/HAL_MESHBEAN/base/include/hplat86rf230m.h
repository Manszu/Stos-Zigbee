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
 
#ifndef HPLAT86RF230M_H
#define HPLAT86RF230M_H

#include <avrhardware.h>

TOSH_ASSIGN_PIN(TRSLEEP, B, 4); // the manipulation by TRSLEEP pin
TOSH_ASSIGN_PIN(RSTIN, A, 7);   // the manipulation by RSTIN pin
//TOSH_ASSIGN_PIN(RSTIN, B, 5); //stk 500
TOSH_ASSIGN_PIN(RFIRQ, E, 5);   // the manipulation by RFIRQ pin


void HPLAT86RF230C__HPLAT86RF230__init();
result_t HPLAT86RF230C__HPLAT86RF230__open(void (*irq)());
result_t HPLAT86RF230C__HPLAT86RF230__close();
result_t HPLAT86RF230C__HPLAT86RF230__irqEnable();
void HPLAT86RF230C__HPLAT86RF230__irqDisable();
void HPLAT86RF230C__HPLAT86RF230__setRST();
void HPLAT86RF230C__HPLAT86RF230__clrRST();
void HPLAT86RF230C__HPLAT86RF230__setSLPTR();
void HPLAT86RF230C__HPLAT86RF230__clrSLPTR();
#endif

// endof HPLAT86RF230.h
