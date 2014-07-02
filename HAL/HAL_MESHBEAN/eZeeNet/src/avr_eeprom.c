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
  
  EEPROM interface implementation for ATMega1281.
  Written by M.Balakhno.
*******************************************************************************/

#ifndef __AVR_ATmega1281__
#define __AVR_ATmega1281__
#endif

#include <avr/io.h>
#include <atomic.h>

#include "eeprom_fxns.h"
#include "eeprom_cntrl_fxns.h"
#include "com_assert.h"



static Eeprom* pEepromObject;  // pointer to EEPROM object

static void doJobs(Eeprom* pEeprom);

/*****************************************************************************
  Description: Check if item is in list
  	  
  Parameters: pList - list
              pListItem - list item
    
  Returns: true if pListItem belongs to pList, false otherwise
*****************************************************************************/
static Bool itemInList(List* pList, ListItem* pListItem)
{
  Bool res = false;  // return result

  if (0 < list_getLength(pList))
  {
    if (1 == list_getLength(pList))
    {
      res = (pList->first == pListItem);
    }
    else
    {
      ListItem* curItem = pList->first;
    
      while(curItem != pList->last)
      {
        if (curItem == pListItem) return true;
        curItem = curItem->next;
      }
    }
  }
    
  return res;
}

/*****************************************************************************
  Description: Puts command to EEPROM.
               Invalid cmdId, zero data length lead to ASSERT().
  	  
  Parameters: pEeprom - EEPROM object reference.
              pCmdDescr - EEPROM command descriptor.
    
  Returns: nothing.
*****************************************************************************/
static void putCmd(Eeprom* pEeprom, EepromCmdDescr* pCmdDescr)
{
  ASSERT(0 != pEeprom);
  ASSERT(0 != pCmdDescr);
  ASSERT(!itemInList(&pEeprom->jobQueue, &pCmdDescr->listItem));
  ASSERT(!itemInList(&pEeprom->doneQueue, &pCmdDescr->listItem));
  ASSERT((EEPROM_CMD_ID_READ == pCmdDescr->id) || (EEPROM_CMD_ID_WRITE == pCmdDescr->id));
  ASSERT(pCmdDescr->length);

  if (EEPROM_DATA_MEMORY_SIZE < pCmdDescr->startAddr + pCmdDescr->length)  
  { 
    // address out of range
    pCmdDescr->status = EEPROM_STATUS_OUT_OF_RANGE;
    list_queueTail(&(pEeprom->doneQueue), &(pCmdDescr->listItem));
  }
  else
  {
    list_queueTail(&(pEeprom->jobQueue), &(pCmdDescr->listItem));

    if (EEPROM_STATE_IDLE == pEeprom->state) 
    { 
      // Pending queue is empty
      doJobs(pEeprom);
    }
  }
}
  
/*****************************************************************************
  Description: Tests whether previously issued command has been completed.
  	  
  Parameters: pEeprom - EEPROM object reference.
              pCmdDescr - EEPROM command descriptor that is being waiting
                          to complete.
    
  Returns: true if command has been completed or false, otherwise.
*****************************************************************************/
static Bool getStatus(Eeprom* pEeprom, EepromCmdDescr* pCmdDescr)
{
  Bool res = false;  // return result
  
  ASSERT(0 != pEeprom);
  ASSERT(0 != pCmdDescr);
  
  if (!itemInList(&(pEeprom->jobQueue), &(pCmdDescr->listItem))) 
  {
    if (itemInList(&(pEeprom->doneQueue), &(pCmdDescr->listItem)))
    {
      list_unlink(&(pEeprom->doneQueue), &(pCmdDescr->listItem));  // remove job from completed list
      res = true;
    }
    else
    {
      ASSERT(false);
    }
  }
  
  return res;
}

/*****************************************************************************
  Description: Returns EEPROM interface implementation reference.
  	
  Parameters: pEepromFxns - EEPROM interface reference.
  	
  Returns: nothing.
*****************************************************************************/
void eeprom_getFxns(EepromFxns* pEepromFxns)
{
  pEepromFxns->putCmd = (void (*)(struct Eeprom*, EepromCmdDescr*))putCmd;
  pEepromFxns->getStatus = (Bool (*)(struct Eeprom*, EepromCmdDescr*))getStatus;
}

/*******************************************************************************
  Description: inits the Eeprom object.
  	
  Parameters: pEeprom - EEPROM object reference.
  	
  Returns: nothing.
*******************************************************************************/
void eeprom_init(Eeprom* pEeprom)
{
  ASSERT(0 != pEeprom);

  pEepromObject = pEeprom;

  // Init active and done job lists  
  list_init(&(pEeprom->jobQueue));   
  list_init(&(pEeprom->doneQueue));  
  
  pEeprom->state = EEPROM_STATE_IDLE;

  // Initialize active job fields
  pEeprom->bytesLeft = 0;  
  pEeprom->dataPointer = (Uint8*)0;
  pEeprom->dataAddress = 0;
  pEeprom->cmdId = EEPROM_CMD_ID_INVALID;      
}

/*******************************************************************************
  Description: Remove active job (jobQueue head) from pending queue, mark it and place into completed list.
               This function is called when active job is completed and should be removed from pending queue.
  	
  Parameters: descr - description of read job
  	
  Returns: nothing.
*******************************************************************************/
static void finishActiveJob(Eeprom* pEeprom, EepromStatus finishStatus)
{
  ListItem* item = list_dequeueHead(&(pEeprom->jobQueue)); 
  
  ((EepromCmdDescr*)item)->status = finishStatus;      
  list_queueTail(&(pEeprom->doneQueue), item);
}

/*******************************************************************************
  Description: Do the read EEPROM job.
               Job is completed instantly.
  	
  Parameters: pEeprom - EEPROM object pointer.
  	
  Returns: nothing.
*******************************************************************************/
static void processReadJob(Eeprom* pEeprom)
{
  for (; pEeprom->bytesLeft; pEeprom->bytesLeft--, pEeprom->dataPointer++, pEeprom->dataAddress++)
  {
    EEARH = pEeprom->dataAddress >> 8;  	
    EEARL = pEeprom->dataAddress;  	
    EECR |= (1 << EERE);
    
    *pEeprom->dataPointer = EEDR;    
  } 
}

/*******************************************************************************
  Description: Write the next byte into EEPROM.
               EEPROM interrupt is enabled to fire upon write completion.
  	
  Parameters: pEeprom - EEPROM object pointer.
  	
  Returns: nothing.
*******************************************************************************/
static void processWriteJob(Eeprom* pEeprom)
{
  EEARH = pEeprom->dataAddress >> 8;  	
  EEARL = (pEeprom->dataAddress++);  	
  EEDR = *(pEeprom->dataPointer++);

  pEeprom->bytesLeft--;

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
  Description: Execute jobs in pending queue.
               Read jobs are completed instantly, 
               while write jobs are done byte-by-byte with interrupt signalling
  	
  Parameters: pEeprom - EEPROM object reference.
  	
  Returns: nothing.
*******************************************************************************/
static void processJobQueue(Eeprom* pEeprom)
{
  pEeprom->state = EEPROM_STATE_IDLE;  

  while (0 < list_getLength(&(pEeprom->jobQueue))) 
  {
    // while there are pending jobs
    {
      // Initiate next job
      EepromCmdDescr *descr = (EepromCmdDescr*)list_peekHead(&(pEeprom->jobQueue));
      pEeprom->bytesLeft = descr->length;
      pEeprom->dataPointer = descr->data;
      pEeprom->dataAddress = descr->startAddr;

      ASSERT( (EEPROM_CMD_ID_READ == descr->id) || (EEPROM_CMD_ID_WRITE == descr->id) );

      if (EEPROM_CMD_ID_READ == descr->id)
      {
        // Read job - do it instantly
        processReadJob(pEeprom);
        finishActiveJob(pEeprom, EEPROM_STATUS_SUCCESS);
      }
      else 
      {    
        // Write job is encountered - start it and leave it. It will be resumed when EEPROM interrupt will fire
        pEeprom->state = EEPROM_STATE_WRITING;
        processWriteJob(pEeprom);
        break;
      }
    }
  }
}

/*******************************************************************************
  Description: Analyse current EEPROM job state and do pending jobs.
  	
  Parameters: pEeprom - EEPROM object reference.
  	
  Returns: nothing.
*******************************************************************************/
static void doJobs(Eeprom* pEeprom)
{
  if (EEPROM_STATE_WRITING == pEeprom->state) 
  {
    // have an active write job
    if (0 == pEeprom->bytesLeft) 
    {
      // job is complete
      finishActiveJob(pEeprom, EEPROM_STATUS_SUCCESS); 
      processJobQueue(pEeprom);
    }
    else 
    {
      // continute job
      processWriteJob(pEeprom);  
    }
  }
  else if (EEPROM_STATE_IDLE == pEeprom->state)
  {
    // Eeprom is idle - get next job from pending queue
    processJobQueue(pEeprom);
  }
  else
  {
    // Impossible state - assert
    ASSERT(false);
  }

}

/*******************************************************************************
  Description: EEPROM Ready interrupt handler.
               Job queue is analyzed here and next job is processed.
              
  Parameters: none.
 
  Returns: none.
*******************************************************************************/
SIGNAL(SIG_EEPROM_READY)
{
  EECR &= ~(1 << EERIE); // disable EEPROM interrupt
  
  doJobs(pEepromObject);
}



//eof eeprom.c
