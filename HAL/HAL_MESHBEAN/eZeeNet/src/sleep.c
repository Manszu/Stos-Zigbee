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

/****************************************************************
  The module to make power off mode.
****************************************************************/

#include <tos.h>
#include <atomic.h>
#include <hplat86rf230m.h>
#include <sleeptimerclock.h>

uint8_t wakeupFlag = 0; //it is used for reliable wake up

/*******************************************************************************
  Description:  Lets the network stack to control TRSLEEP pin.
                (Disconnects TRSLEEP pin from comparator output.)
 
  Parameters:   none.
  
  Returns:      none.
*******************************************************************************/
void PowerOn()
{ // will let the application the control of TRSLEEP pin
  wakeupFlag = 1;
#ifdef HAL_RC_OSCILLATOR
  TOSH_CLR_TRSLEEP_PIN();
#else
  TOSH_MAKE_TRSLEEP_INPUT(); // Disconnects TRSLEEP pin from comparator output.
  TCCR2A &= ~(1<<COM2A1 | 1<<COM2A0);// no compare  
  while ( ASSR & 0x1B); 
#endif
  TOSH_MAKE_TRSLEEP_OUTPUT();
  
  #ifdef ZDM_A1281_PN
    // ZigBit with amplifier.
    PORTC |= 1<<PIN1;
  #endif
  
}
  
/*******************************************************************************
  Description:  Shutdown system.
 
  Parameters:   none.
  
  Returns:      none.
  
  NOTES:        the application should be sure the poweroff will not be 
                interrupted before the execution of the sleep command.
*******************************************************************************/
void PowerOff()
{      
  if(wakeupFlag)
   return;  // it is a too late for sleep.  The sleep interval is expired.
#ifdef HAL_RC_OSCILLATOR

  #ifdef ZDM_A1281_PN
    // ZigBit with amplifier.
    PORTC &= ~(1<<PIN1);
  #endif


  HPLTimer0Clock__sleepControl( TRUE );// will be shutdown
  TOSH_SET_TRSLEEP_PIN();
  TOSH_MAKE_TRSLEEP_OUTPUT();
  SMCR |= (1 << SM1) | (1 << SM0);
  SMCR |= 1 << SE;
  asm volatile
  (
    "SLEEP\n\t"
  ::);
  SMCR = 0;
#else
  uint8_t timsk5 = TIMSK5;
  uint8_t twcr = TWCR;
  uint8_t adcsra =  ADCSRA;
  TIMSK5 = 0;
  TWCR = 0;
  ADCSRA = 0;
  HPLTimer0Clock__sleepControl( TRUE );// will be shutdown
  TOSH_MAKE_TRSLEEP_OUTPUT();
  SMCR |= (1 << SM1) | (1 << SM0);
  SMCR |= 1 << SE;
  asm volatile
  (
    "SLEEP\n\t"
  ::);
  SMCR = 0;
  TIMSK5 = timsk5;
  TWCR = twcr;
  ADCSRA = adcsra;       
#endif
}
 
/*******************************************************************************
  Description:  Prepares system for shutdown.
 
  Parameters:   none.
  
  Returns:      none.
*******************************************************************************/
void sleep()
{ 
  wakeupFlag = 0;  // the reset of sign of entry to the sleep mode.
//    PORTA = 0xFF;
//    PORTB = 0xFF;
    PORTC = 0xFF;
//    PORTD = 0xFF;
//    PORTE = 0xFF;
//    PORTF = 0xFF;
#if 0
    DDRA = 0xFF;
//    DDRB = 0xFF;
    DDRC = 0xFF;
//    DDRD = 0xFF;
    DDRD = 0xFB;
//    DDRE = 0xFF;
    DDRE = 0xDE;
//    DDRF = 0xFF;
    DDRF = 0x6F;
    DDRG = 0xFF;
#endif
#if 0
//    DDRA = 0x00;
//    DDRB = 0x00;
    DDRC = 0x00;
    DDRD = 0x00;
    DDRE = 0x00;
    DDRF = 0x00;
    DDRG = 0x00;
#endif
//    PORTD = 0x7F;
//    DDRD = 0x80;

//    PORTF = 0xFC;
//    DDRF = 0x03;
#ifndef HAL_RC_OSCILLATOR
  TOSH_MAKE_TRSLEEP_INPUT();
  while ( ASSR & 0x1B);  
  TCCR2A |= (1<<COM2A1 | 1<<COM2A0); // set OC2A on compare
  while ( ASSR & 0x1B);
  TCCR2B |= 1<<FOC2A;// force output to set OC2A
  while ( ASSR & 0x1B);
  TCCR2A &= ~(1<<COM2A1 | 1<<COM2A0);// no compare  
  while ( ASSR & 0x1B);
  TCCR2A |= ( 1<<COM2A1 ); // clear OC2A on compare
  while ( ASSR & 0x1B);
#endif
  TOS_post(PowerOff);
}

// eof sleep.c
