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
  uart_avr1281.c
  
  Base UART functions for AVR 1281

  This file contains commonly used functions for uart0, uart1, usrt0, usrt1 
  implementations and must not be used directly. Use one of listed above modules
  for USART functionality.
  
  Written by M.Balakhno
***********************************************************************/

#include "uart_avr1281.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/pgmspace.h>
#include <atomic.h>
#include "com_assert.h"
#include "circ_buf.h"

// Register offset for UART registers from register group start 
#define UART_OFFSET_UCSRA 0
#define UART_OFFSET_UCSRB 1
#define UART_OFFSET_UCSRC 2
#define UART_OFFSET_UBRRL 4
#define UART_OFFSET_UBRRH 5
#define UART_OFFSET_UDR   6

// Macros for operating on UART registers
#define UART_REG_UCSRA(basereg)   (*((basereg) + UART_OFFSET_UCSRA))
#define UART_REG_UCSRB(basereg)   (*((basereg) + UART_OFFSET_UCSRB))
#define UART_REG_UCSRC(basereg)   (*((basereg) + UART_OFFSET_UCSRC))
#define UART_REG_UBRRL(basereg)   (*((basereg) + UART_OFFSET_UBRRL))
#define UART_REG_UBRRH(basereg)   (*((basereg) + UART_OFFSET_UBRRH))
#define UART_REG_UDR(basereg)     (*((basereg) + UART_OFFSET_UDR))



#define UART_PRESC_UBRR_MASK    0xFFF  // UBRR part in pre-compiled prescaler table
#define UART_PRESC_U2X          0x4000   // U2X bit in pre-compiled prescaler table

// Setup values for 8MHz oscillator
#ifdef FOSC_8MHZ 

#define UART_PRESC_INVALID  0                    // 
#define UART_PRESC_1200     417                  // U2X = 0, 0.0% error
#define UART_PRESC_9600     51                   // U2x = 0, 0.2% error
#define UART_PRESC_38400    12                   // U2X = 0, 0.2% error with 
#define UART_PRESC_57600    (16|UART_PRESC_U2X)  // U2X = 1, 2.1% error 
#define UART_PRESC_115200   0                    // 8.5% error or -3.5% with U2X = 1, both ways is useless

// Setup values for 4MHz oscillator
#else

#define UART_PRESC_INVALID  0                     // 
#define UART_PRESC_1200     208                   // U2X = 0, 0.0% error
#define UART_PRESC_9600     25                    // U2x = 0, 0.2% error
#define UART_PRESC_38400    (12|UART_PRESC_U2X)   // U2X = 1, 0.2% error with 
#define UART_PRESC_57600    0                     // 8.5% error or -3.5% with U2X = 1, both ways is useless
#define UART_PRESC_115200   0                     // 8.5% error with or without U2X = 1, both ways is useless

#endif



#define UART_FLAG_CTS           0x2
#define UART_FLAG_RTS           0x4
#define UART_FLAG_SOURCE_QUENCH 0x8
#define UART_FLAG_RX_DISABLE    0x10


#define UART_MIN_BUFFER_SPACE   10

static const Uint16 uartPrescalers[] PROGMEM =   // prescale values for selected FOSC
{
  UART_PRESC_INVALID,
  UART_PRESC_1200, 
  UART_PRESC_9600, 
  UART_PRESC_38400, 
  UART_PRESC_57600, 
  UART_PRESC_115200
}; 



/*******************************************************************************
  Description:  Prepare some UART registers for opening UART 
                This function sets the baudrate, parity, data bits and stop bits.

  Parameters:   pUart - UART base object
                mode - UART mode
  
  Returns:      none.
*******************************************************************************/
void uartAvr1281_preOpen(UartBase* pUart, const UartMode *mode)
{
  Uint16 prescaler_config;

  ASSERT(0 != pUart);
  ASSERT(0 != mode);
  ASSERT(UART_STATE_CLOSED == pUart->state);
  ASSERT( (UART_BAUD_RATE_INVALID < mode->baudRate) && (UART_BAUD_RATE_115200 >= mode->baudRate) );
  ASSERT( (UART_DATA_BITS_5 <= mode->dataBits) && (UART_DATA_BITS_8 >= mode->dataBits) );
  ASSERT( (UART_PARITY_NONE == mode->parity) || (UART_PARITY_EVEN == mode->parity)  || (UART_PARITY_ODD == mode->parity));
  ASSERT( (UART_STOP_BITS_1 == mode->stopBits) || (UART_STOP_BITS_2 == mode->stopBits) );
  
  // Read pre-compiled prescaler value from program memory
  memcpy_P(&prescaler_config, &uartPrescalers[ mode->baudRate ], sizeof(Uint16));    

  // Check that selected baudrate is supported
  ASSERT(prescaler_config);

  // Init UART object fields
  pUart->flags = 0;
  CIRC_BUFFER_INIT(pUart->rxHead, pUart->rxTail);
  CIRC_BUFFER_INIT(pUart->txHead, pUart->txTail);

  UART_REG_UCSRA(pUart->baseReg) = 0;
  UART_REG_UCSRB(pUart->baseReg) = 0;                               // uart off
  UART_REG_UCSRC(pUart->baseReg) = 0;
  
  // init baud rate
  UART_REG_UBRRL(pUart->baseReg) = prescaler_config & 0xFF;  
  UART_REG_UBRRH(pUart->baseReg) = (prescaler_config & UART_PRESC_UBRR_MASK) >> 8;  
  if (prescaler_config & UART_PRESC_U2X)
  {
    UART_REG_UCSRA(pUart->baseReg) |= (1<<U2X0);  
  }

  // Set bit count
  if (UART_DATA_BITS_6 == mode->dataBits)
  {
    UART_REG_UCSRC(pUart->baseReg) |= (1<<UCSZ00);    
  }
  else if (UART_DATA_BITS_7 == mode->dataBits)
  {
    UART_REG_UCSRC(pUart->baseReg) |= (1<<UCSZ01); 
  }
  else if (UART_DATA_BITS_8 == mode->dataBits)
  {
    UART_REG_UCSRC(pUart->baseReg) |= (1<<UCSZ00)|(1<<UCSZ01); 
  }

  // Set parity
  if (UART_PARITY_EVEN == mode->parity)
  { 
    UART_REG_UCSRC(pUart->baseReg) |= (1 << UPM01);
  }
  else if (UART_PARITY_ODD == mode->parity)
  {
    UART_REG_UCSRC(pUart->baseReg) |= (1 << UPM01)|(1 << UPM00);
  }

  // Store flow control information in flags
  pUart->flags |= ( (mode->flowCntrl.rts)?(UART_FLAG_RTS):(0) ) | 
                  ( (mode->flowCntrl.cts)?(UART_FLAG_CTS):(0) );
                  
  pUart->state = UART_STATE_OPENED;

}

/*******************************************************************************
  Description:  Close UART hardware

  Parameters:   pUart - UART base object
  
  Returns:      none.
*******************************************************************************/
void uartAvr1281_close(UartBase* pUart)
{
  ASSERT(UART_STATE_OPENED == pUart->state);

  UART_REG_UCSRA(pUart->baseReg) = 0;
  UART_REG_UCSRB(pUart->baseReg) = 0;       // uart off
  UART_REG_UCSRC(pUart->baseReg) = 0;
  
  pUart->state = UART_STATE_CLOSED;
}

/*******************************************************************************
  Description:  If it is OK to continue transmit, this function sends next byte. 
                If transmit should be paused for some reason (TX buffer depleted 
				or due to RTS signalling), function disables the TX interrupt.
                This function doesn't check the availability of TX buffer, so 
				certain precautions should be made prior to calling it.

  Parameters:   pUart - UART base object
  
  Returns:      none.
*******************************************************************************/
void uartAvr1281_resumeTransmit(UartBase* pUart)
{
  // Transmit can be continued only when two conditions are met: 
  // 1) outgoing buffer is not empty
  // 2) RTS is high (if enabled) 
  ATOMIC_SECTION_ENTER
  if ( (!CIRC_BUFFER_EMPTY(pUart->txHead, pUart->txTail, pUart->txLength)) && 
       ((!(pUart->flags & UART_FLAG_RTS)) || (!((*(pUart->rtsPinReg)) & pUart->rtsPinMask))) )
  {
    // Send next byte and advance TX buffer
	CIRC_BUFFER_READ(pUart->txBuff, pUart->txTail, pUart->txLength, UART_REG_UDR(pUart->baseReg));

    // Re-enable TX interrupt
    UART_REG_UCSRB(pUart->baseReg) |= (1 << UDRIE0);
  }
  else
  {
    // Cannot (or nothing to) send. Disable TX data interrupt
    UART_REG_UCSRB(pUart->baseReg) &= ~(1 << UDRIE0);

  }
  ATOMIC_SECTION_LEAVE
}


/*******************************************************************************
  Description:  Queue bytes for sending.

  Parameters:   pUart - UART base object
                data - pointer to byte array
				length - total bytes to send
  
  Returns:      Number of bytes queued
*******************************************************************************/
Uint16 uartAvr1281_put(UartBase* pUart, const Uint8* data, Uint16 length)
{
  Uint16 res = 0;

  ASSERT(0 != pUart);
  ASSERT(0 < length);  
  ASSERT(0 != data);
  ASSERT(UART_STATE_OPENED == pUart->state);

  ATOMIC_SECTION_ENTER
  while ( (!CIRC_BUFFER_FULL(pUart->txHead, pUart->txTail, pUart->txLength)) && 
          (res < length) )
  {
    CIRC_BUFFER_WRITE(pUart->txBuff, pUart->txHead, pUart->txLength, *(data++));  
	res++;
  }
  ATOMIC_SECTION_LEAVE

  // If transmission is off (TX interrupt is disabled), try to resume it
  if (!(UART_REG_UCSRB(pUart->baseReg) & (1 << UDRIE0)))
  {
    uartAvr1281_resumeTransmit(pUart);
  }

  return res;
}

/*******************************************************************************
  Description:  Manage CTS signalling depending on RX buffer state

  Parameters:   pUart - UART base object
  
  Returns:      none.
*******************************************************************************/
void uartAvr1281_manageCts(UartBase* pUart)
{
  if (!(pUart->flags & UART_FLAG_RX_DISABLE))
  {
    if ( UART_MIN_BUFFER_SPACE > (pUart->rxLength - CIRC_BUFFER_COUNT(pUart->rxHead, pUart->rxTail, pUart->rxLength)) )
    {
      if (!(pUart->flags & UART_FLAG_SOURCE_QUENCH))
      {
        // CTS is up but buffer is almost full - disable CTS
        *(pUart->ctsPortReg) |= (pUart->ctsPortMask);
        pUart->flags |= UART_FLAG_SOURCE_QUENCH;
      }
    }
    else 
    {
      if (pUart->flags & UART_FLAG_SOURCE_QUENCH)
      {
        // CTS is down, but now buffer has some space, so re-enable CTS
        *(pUart->ctsPortReg) &= ~(pUart->ctsPortMask);
        pUart->flags &= ~UART_FLAG_SOURCE_QUENCH;
      }
    }
  }
}

/*******************************************************************************
  Description: explicitly control cts signal for UART1

  Parameters: pUart1     - UART1 object reference
              enableRx   - if true, cts will be contolled by uart according to 
                                    its buffer state,
                           if false, cts will unconditionally set to disable rx.

  Returns: nothing.
*******************************************************************************/
void uartAvr1281_rxControl(UartBase* pUart, Bool enableRx)
{
  ASSERT(0 != pUart);
  if (pUart->flags & UART_FLAG_CTS)
  {
    if (enableRx)
    {
      pUart->flags &= ~(UART_FLAG_RX_DISABLE);
    }
    else
    {
      *(pUart->ctsPortReg) |= (pUart->ctsPortMask);
      pUart->flags |= UART_FLAG_RX_DISABLE | UART_FLAG_SOURCE_QUENCH;
    }
  }
}

/*******************************************************************************
  Description:  Queue bytes for sending.

  Parameters:   pUart - UART object
                data - pointer to byte array
				length - total bytes to send
  
  Returns:      Number of bytes queued
*******************************************************************************/
Uint16 uartAvr1281_get(UartBase* pUart, Uint8* data, Uint16 length)
{
  Uint16 res = 0;
  
  ASSERT(0 != pUart);
  ASSERT(0 < length);
  ASSERT(0 != data);  
  ASSERT(UART_STATE_OPENED == pUart->state);  

  ATOMIC_SECTION_ENTER
  while ( (!CIRC_BUFFER_EMPTY(pUart->rxHead, pUart->rxTail, pUart->rxLength)) && (res < length) )
  {
    CIRC_BUFFER_READ(pUart->rxBuff, pUart->rxTail, pUart->rxLength, *(data++));  
	res++;
  }
  ATOMIC_SECTION_LEAVE

  // Manage CTS signalling if it is enabled
  if (pUart->flags & UART_FLAG_CTS)
  {
    uartAvr1281_manageCts(pUart);
  }

  return res;
}

/*******************************************************************************
  Description:  Interrupt on RXC (USART receive complete) flag handler

  Parameters:   none. 
  
  Returns:      none.
*******************************************************************************/
void uartAvr1281_receive(UartBase* pUart)
{
  uint8_t data_status;
  uint8_t data;

  // It's important to read USRnA register first 
  // before reading UDRn register (otherwise UCSRnA flags will be cleared)
  data_status = UART_REG_UCSRA(pUart->baseReg);
  data = UART_REG_UDR(pUart->baseReg);

  // Frame or parity error occured - skip invalid byte
  if (data_status & ((1 << FE0) | (1 << UPE0))) 
  {
  }
  else
  {
    // Store received byte in buffer, if there is space left
	  if (!CIRC_BUFFER_FULL(pUart->rxHead, pUart->rxTail, pUart->rxLength))
	  {
	    CIRC_BUFFER_WRITE(pUart->rxBuff, pUart->rxHead, pUart->rxLength, data);
	  }
  }

  // Manage CTS signalling if it is enabled
  if (pUart->flags & UART_FLAG_CTS)
  {
    uartAvr1281_manageCts(pUart);
  }

}



/*******************************************************************************
  Description: Check that all the queued bytes has left the shifting register

  Parameters: pUart - UART object

  Returns: false if UART is currently transmitting or if TX queue is not empty, true otherwise
*******************************************************************************/
Bool uartAvr1281_isTxEmpty(UartBase* pUart)
{
  Bool bufferEmpty;
//  Bool dataRegEmpty;
//  Bool txComplete;
  
  ASSERT(0 != pUart);
  ASSERT(UART_STATE_OPENED == pUart->state);  

  bufferEmpty = CIRC_BUFFER_EMPTY(pUart->txHead, pUart->txTail, pUart->txLength);
  
// TODO: empty only when txComplete.
//  dataRegEmpty = UART_REG_UCSRA(pUart->baseReg) & (1 << UDRE0);
//  txComplete = UART_REG_UCSRA(pUart->baseReg) & (1 << TXC0);

  return bufferEmpty;
}


/*******************************************************************************
  Description: process UART inner tasks

  Parameters: pUart - UART base object reference.

  Returns: nothing.
*******************************************************************************/
void uartAvr1281_process(UartBase* pUart)
{
  // If transmission is off (TX interrupt is disabled), try to resume it
  if (!(UART_REG_UCSRB(pUart->baseReg) & (1 << UDRIE0)))
  {
    uartAvr1281_resumeTransmit(pUart);
  }  

  // Manage CTS signalling if it is enabled
  if (pUart->flags & UART_FLAG_CTS)
  {
    uartAvr1281_manageCts(pUart);
  }
}
