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
#ifndef SLEEPTIMERCLOCK_H
#define SLEEPTIMERCLOCK_H

#define SLEEPTIMER_CLOCK 32768lu
#define SLEEPTIMER_PRESCALER_1    (1u << CS20)                  // No prescaling
#define SLEEPTIMER_PRESCALER_8    (1u << CS21)                  // clk/8
#define SLEEPTIMER_PRESCALER_32   (1u << CS20 | 1u << CS21)     // clk/32
#define SLEEPTIMER_PRESCALER_64   (1u << CS22)                  // clk/64
#define SLEEPTIMER_PRESCALER_128  (1u << CS20 | 1u << CS22)     // clk/128
#define SLEEPTIMER_PRESCALER_256  (1u << CS21 | 1u << CS22)     // clk/256
#define SLEEPTIMER_PRESCALER_1024 (1u << CS20 | 1u << CS21 | 1u << CS22) // clk/1024

/****************************************************************
  Description:  Stops the application timer. Sets the sleep control flag.

  Parameters:   enableSleep - TRUE system will be shutdown after 
                the wake up if the interval is still not counted.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__sleepControl(bool enableSleep);

/****************************************************************
  Description:  Initializes the component.

  Parameters:   none.
  
  Returns:      SUCCESS.
****************************************************************/
result_t HPLTimer0Clock__StdControl__init();

/****************************************************************
  Description:  Sets clock interval. 

  Parameters:   value - contains number of ticks which the timer must count out.
  
  Returns:     none. 
****************************************************************/
void HPLTimer0Clock__setInterval( uint32_t value );

/****************************************************************
  Description:  Returns the current interval in ticks.

  Parameters:   none.
  
  Returns:      current interval in ticks
****************************************************************/
uint32_t HPLTimer0Clock__getInterval();

/****************************************************************
  Description:  Returns the timer frequency in Hz.

  Parameters:   none.
  
  Returns:      timer frequency in Hz.
****************************************************************/
uint32_t HPLTimer0Clock__getFrequency();

/****************************************************************
  Description:  Disables compare interrupt.

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__intDisable();

/****************************************************************
  Description:  Enables compare interrupt.

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__intEnable();

/****************************************************************
  Description:  Notifies on that the set interval was counted out

  Parameters:   none.
  
  Returns:      none.
****************************************************************/
void HPLTimer0Clock__fire();

#endif
// eof sleeptimerclock.h



