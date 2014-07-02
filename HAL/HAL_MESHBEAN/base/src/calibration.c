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

#include <tos.h>
#include <avrhardware.h> 
#include <hplat86rf230m.h>

#define INTERNAL_CLOCK 4000000

#ifndef RTC_CALIBRATION

#define EXTERNAL_TICKS 100*4
#define CYCLE_LENGTH 10
#define REFERENCE_CLOCK 1000000
#define REFERENCE_COUNT (INTERNAL_CLOCK * EXTERNAL_TICKS) / ( REFERENCE_CLOCK * CYCLE_LENGTH )

/*******************************************************************************
  Description: measure offset.

  Parameters: nothing.

  Returns: offset value.
*******************************************************************************/
uint16_t measurement(void)
{
  uint16_t cnt = 0;
  TCCR3B = 0; // stop the timer
  TCNT3 = 0;
  TCCR3B = (1 << CS30) | (1 << CS31) | (1 << CS32); // external clock source on T3 pin. Rising edge.
  do
  {
    cnt++;
  } while (TCNT3 < EXTERNAL_TICKS);
  return cnt;
}

/*******************************************************************************
  Description: clear measurement.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void measurement_clear()
{
  TCCR3B = 0; // Stops the timer
  TIFR3  |= (1 << ICF3) | (1 << OCF3A) | (1 << OCF3B) | (1 << OCF3C) | (1 << TOV3); // Clears interrupt flags
}
#else

#define EXTERNAL_TICKS 2*4
#define CYCLE_LENGTH 7
#define REFERENCE_CLOCK 32768
#define REFERENCE_COUNT (INTERNAL_CLOCK * EXTERNAL_TICKS) / ( REFERENCE_CLOCK * CYCLE_LENGTH )

/*******************************************************************************
  Description: measure offset.

  Parameters: nothing.

  Returns: offset value.
*******************************************************************************/
uint16_t measurement(void)
{
  uint16_t cnt = 0;
  ASSR |= (1 << AS2);
  TCCR2B = 1 << CS20; 
  TCNT2 = 0;
  while (ASSR & 0x1B);
  do
  {
    cnt++;
  } while (TCNT2 < EXTERNAL_TICKS);
  return cnt;                       
}

/*******************************************************************************
  Description: clear measurement.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void measurement_clear()
{
  TCCR2B = 0; // Stops the timer
  TIFR2 |= (1 << OCF2A) | (1 << OCF2B) | (1 << TOV2); // Clears interrupt flags
}
#endif

/*******************************************************************************
  Description: calibrate interanl RC-generator.

  Parameters: nothing.

  Returns: nothing.
*******************************************************************************/
void calibrateInternalRc(void)
{
  uint16_t count;
  uint8_t cycles = 0x80;
  do
  {
    count = measurement();
    if (count > REFERENCE_COUNT)
      OSCCAL--;
    if (count < REFERENCE_COUNT)
      OSCCAL++;
    if (count == REFERENCE_COUNT)
    {
      break;
    }
  } while(--cycles); 
  measurement_clear();
}

// eof calibration.c
