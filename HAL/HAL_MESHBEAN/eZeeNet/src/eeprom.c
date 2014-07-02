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
  eeprom.c

  Implementation of the EEPROM module.
*******************************************************************************/

#include <tos.h>
#include <eeprom.h>
#include <atomic.h>

enum // EEPROM states
{
  EEPROM_IDLE_STATE = 10,
  EEPROM_WRITE_STATE,
  EEPROM_READ_STATE,
} EEPROM__state;
  
enum // EEPROM substates
{
  EEPROM_IDLE_SUBSTATE = 0, // idle
  EEPROM_NOT_LAST_BYTE,     // one more byte (not last) need to be processed
  EEPROM_LAST_BYTE,         // last byte need to be processed
} EEPROM__subState;

  EEPROMParams_t *EEPROM__param;   // pointer to parameter
  void (*EEPROM__operationDone)(); // callback

/*******************************************************************************
  Description: init EEPROM

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
result_t eeprom_init()
{
  EEPROM__state = EEPROM_IDLE_STATE;
  EEPROM__subState = EEPROM_IDLE_SUBSTATE;
  return SUCCESS;
}
  
/*******************************************************************************
  Description: The task signals that the data block has been written.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void EEPROM__signalWriteDone()
{
  EEPROM__param->status = EEPROM_SUCCESS_STATUS;
  EEPROM__operationDone();
}

/*******************************************************************************
  Description: The task signals that the data block has been read.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void EEPROM__signalReadDone()
{
  EEPROM__param->status = EEPROM_SUCCESS_STATUS;
  EEPROM__operationDone();
}

/*******************************************************************************
  Description: Reads bytes from the EEPROM.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void EEPROM_read_byte()
{
  while( EECR & (1 << EEPE) ); // wait for complition of previous write
  for(; EEPROM__param->length > 0; EEPROM__param->length--)
  {
    EEARH = EEPROM__param->addr >> 8;  	
    EEARL = EEPROM__param->addr;  	
    EEPROM__param->addr++;
    EECR |= (1 << EERE);
    *EEPROM__param->data = EEDR;
    EEPROM__param->data++;
  }
}

/*******************************************************************************
  Description: Writes one byte to the EEPROM.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void EEPROM_write_byte()
{
  while( EECR & (1 << EEPE) ); // wait for complition of previous write
  EEPROM__param->length--;
  EEARH = EEPROM__param->addr >> 8;  	
  EEARL = EEPROM__param->addr;  	
  EEPROM__param->addr++;
  EEDR = *EEPROM__param->data;
  EEPROM__param->data++;
    ATOMIC_SECTION_ENTER
    asm volatile
      (
        "sbi 0x1F,0x03\n\t" //sbi EECR,EERIE //enable EEPROM ready interrupt
        "sbi 0x1F,0x02\n\t" //sbi EECR,EEMPE
        "sbi 0x1F,0x01\n\t" //sbi EECR,EEPE        
      ::);
    ATOMIC_SECTION_LEAVE
}

/*******************************************************************************
  Description: The main state machine.

  Parameters: status.
 
  Returns: none.
*******************************************************************************/
void EEPROM__callDispatcher(result_t status)
{
  switch (EEPROM__state)
  {
    case EEPROM_WRITE_STATE:
      switch (EEPROM__subState)
      {
        case EEPROM_IDLE_SUBSTATE:
          if(EEPROM__param->length > 1)
              EEPROM__subState = EEPROM_NOT_LAST_BYTE;
          else
            EEPROM__subState = EEPROM_LAST_BYTE;
            EEPROM_write_byte();
        break;

        case EEPROM_NOT_LAST_BYTE:
          if(EEPROM__param->length == 1)
              EEPROM__subState = EEPROM_LAST_BYTE;
            EEPROM_write_byte();
        break;

        case EEPROM_LAST_BYTE:
          EEPROM__state = EEPROM_IDLE_STATE;
          EEPROM__subState = EEPROM_IDLE_SUBSTATE;
          TOS_post( EEPROM__signalWriteDone );
        break;

        default:
        break;
      }
        
    break;

    case EEPROM_READ_STATE:
      switch (EEPROM__subState)
      {
        case EEPROM_IDLE_SUBSTATE:
          EEPROM_read_byte();
          EEPROM__state = EEPROM_IDLE_STATE;
          TOS_post( EEPROM__signalReadDone );
          break;

        case EEPROM_NOT_LAST_BYTE:
          break;

        case EEPROM_LAST_BYTE:
          break;

        default:
          break;
      }
      
      break;

      default:
        break;	
  }
}

/*******************************************************************************
  Description: call splitter.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
void EEPROM__tDispatcher()
{
    EEPROM__callDispatcher(SUCCESS);
}

/*******************************************************************************
  Description:  Writes the data block to the EEPROM.

  Parameters:   params - address of parameters structure.
                writeDone - callback.

  Returns:      SUCCESS -  no operations is performing and length is not 0
                FAIL - in other case.
*******************************************************************************/
result_t eeprom_write(EEPROMParams_t *params, void (*writeDone)())
{
  if( !params || !writeDone ) return FAIL;
  if( params->addr >= EEPROM_DATA_MEMORY_SIZE )
  {
    params->status = EEPROM_ADDR_TOO_FAR_STATUS;
    return SUCCESS;
  }
  if( (uint16_t)(params->addr + params->length) > EEPROM_DATA_MEMORY_SIZE )
  {
    params->status = EEPROM_LENGTH_TOO_LONG_STATUS;
    return SUCCESS;
  }
  if( EEPROM_IDLE_STATE != EEPROM__state)
  {
    params->status = EEPROM_OPERATION_OVERFLOW_STATUS;
    return SUCCESS;
  }
  EEPROM__state = EEPROM_WRITE_STATE;
  EEPROM__param = params;
  EEPROM__operationDone = writeDone;

  TOS_post( EEPROM__tDispatcher );
  return SUCCESS;
}
  
/*******************************************************************************
  Description:  Reads data block from the EEPROM.

  Parameters:   params - address of parameters structure.

  Returns:      SUCCESS -  no operations is performing and length is not 0 
                FAIL - in other case.
*******************************************************************************/
result_t eeprom_read(EEPROMParams_t *params, void (*readDone)())
{
  if( !params || !readDone ) return FAIL;
  if( params->addr >= EEPROM_DATA_MEMORY_SIZE )
  {
    params->status = EEPROM_ADDR_TOO_FAR_STATUS;
    return SUCCESS;
  }
  if( (uint16_t)(params->addr + params->length) > EEPROM_DATA_MEMORY_SIZE )
  {
    params->status = EEPROM_LENGTH_TOO_LONG_STATUS;
    return SUCCESS;
  }
  if( EEPROM_IDLE_STATE != EEPROM__state)
  {
    params->status = EEPROM_OPERATION_OVERFLOW_STATUS;
    return SUCCESS;
  }
  EEPROM__state = EEPROM_READ_STATE;
  EEPROM__param = params;
  EEPROM__operationDone = readDone;
  TOS_post( EEPROM__tDispatcher );
  return SUCCESS;
}
  
/*******************************************************************************
  Description: EEPROM Ready interrupt handler.

  Parameters: none.
 
  Returns: none.
*******************************************************************************/
SIGNAL(SIG_EEPROM_READY)
{
  EECR &= ~(1 << EERIE); //disable interrupt
  if(EEPROM__state == EEPROM_WRITE_STATE)
  TOS_post( EEPROM__tDispatcher );
}

// eof eeprom.c
