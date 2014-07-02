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
  com_list.c
  
  Double linked list capability implementation.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#include "com_list.h"
#include "com_assert.h"

/*******************************************************************************
  Description: inits list.
  
  Parameters: pList - list reference.
  
  Returns: nothing.
*******************************************************************************/
void list_init(List* pList)
{
  ASSERT(0 != pList);
  pList->first = 0;
  pList->last = 0;
  pList->length = 0;  
}

/*******************************************************************************
  Description: provides list head reference.
  
  Parameters: pList - list reference.
  
  Returns: list head reference.
*******************************************************************************/
ListItem* list_peekHead(List* pList)
{
  ASSERT(0 != pList);
  return pList->first;
}

/*******************************************************************************
  Description: provides list tail reference.
  
  Parameters: pList - list reference.
  
  Returns: list tail reference.
*******************************************************************************/
ListItem* list_peekTail(List* pList)
{
  ASSERT(0 != pList);
  return pList->last;
}

/*******************************************************************************
  Description: adds item to list head.
  
  Parameters: pList - list reference.
              pListItem - item reference to add.
  
  Returns: nothing.
*******************************************************************************/
void list_queueHead(List* pList, ListItem* pListItem)
{
  ASSERT((0 != pList) && (0 != pListItem));
  ASSERT(LIST_MAX_LENGTH != pList->length);
  {
    ListItem* old = pList->first;

    pList->length++;
    pListItem->next = old;
    pListItem->prev = 0;
    pList->first = pListItem;
    if (old)
      old->prev = pListItem;
    else
      // It is the only one item in the list.
      pList->last = pListItem;
  }
}

/*******************************************************************************
  Description: adds item to list tail.
  
  Parameters: pList - list reference.
              pListItem - item reference to add.
  
  Returns: nothing.
*******************************************************************************/
void list_queueTail(List* pList, ListItem* pListItem)
{
  ASSERT((0 != pList) && (0 != pListItem));
  ASSERT(LIST_MAX_LENGTH != pList->length);
  {
    ListItem* old = pList->last;
  
    pList->length++;
    pListItem->next = 0;
    pListItem->prev = old;
    pList->last = pListItem;
    if (old)
      old->next = pListItem;
    else
      // It is the only one item in the list.
      pList->first = pListItem;
  }
}

/*******************************************************************************
  Description: removes item from list head.
  
  Parameters: pList - list reference.
  
  Returns: removed item reference.
*******************************************************************************/
ListItem* list_dequeueHead(List* pList)
{
  ListItem* pListItem;
  
  ASSERT(0 != pList);
  pListItem = pList->first;
  if (0 != pListItem)
  {
    ListItem* next;
    
    ASSERT(0 != pList->length);    
    pList->length--;
    next = pListItem->next;
    pList->first = next;
    if (next)
      next->prev = 0;
    else
      // It is the only one item in the list.
      pList->last = 0;
    
    // Just for pointing that item is not in list. 
    pListItem->next = 0; 
  }
  return pListItem;
}

/*******************************************************************************
  Description: removes item from list tail.
  
  Parameters: pList - list reference.
  
  Returns: removed item reference.
*******************************************************************************/
ListItem* list_dequeueTail(List* pList)
{
  ListItem* pListItem;
  
  ASSERT(0 != pList);
  pListItem = pList->last;
  if (0 != pListItem)
  {
    ListItem* prev;
    
    ASSERT(0 != pList->length);
    pList->length--;
    prev = pListItem->prev;
    pList->last = prev;
    if (prev)
      prev->next = 0;
    else
      // It is the only one item in the list.
      pList->first = 0;
    
    // Just for pointing that item is not in list.
    pListItem->prev = 0;
  }
  return pListItem;
}

/*******************************************************************************
  Description: unlinks item from list.
  
  Parameters: pList - list reference.
              pListItem - item reference to unlink.
  
  Returns: nothing.
*******************************************************************************/
void list_unlink(List* pList, ListItem* pListItem)
{
  ASSERT((0 != pList) && (0 != pListItem));
  ASSERT(pList->length);
  {
    ListItem* prev;
    ListItem* next;
    
    prev = pListItem->prev;
    next = pListItem->next;
    if (prev && next)
    {
      // It is not neither first nor last item in the list.
      pList->length--;
      prev->next = next;
      next->prev = prev;
    }
    else
    {
      // It is either first or last item in the list.
      if (!prev)
        // It is the first item in the list.
        list_dequeueHead(pList);
      else
        // It is the last item in the list.
        list_dequeueTail(pList);
    }

    // Just for pointing that item is not in list.
    pListItem->prev = 0;
    pListItem->next = 0;
  }
}

/*******************************************************************************
  Description: adds item to list after pointed one.
  
  Parameters: pList - list reference.
              pNew - item reference to add.
              pOld - pointed item reference.
 
  Returns: nothing.
*******************************************************************************/
void list_append(List* pList, ListItem* pOld, ListItem* pNew)
{
  ASSERT((0 != pList) && (0 != pOld) && (0 != pNew));
  ASSERT(LIST_MAX_LENGTH != pList->length);
  pList->length++;
  if (pOld->next)
  {
    // Add not after last.
    ListItem* prev = pOld;
    ListItem* next = pOld->next;
    
    pNew->prev = prev;
    pNew->next = next;
    prev->next = pNew;
    next->prev = pNew;
  }
  else
    // Add after last.
    list_queueTail(pList, pNew);
}

/*******************************************************************************
  Description: adds item to list before pointed one.
  
  Parameters: pList - list reference.
              pNew - item reference to add.
              pOld - pointed item reference.
 
  Returns: nothing.
*******************************************************************************/
void list_insert(List* pList, ListItem* pOld, ListItem* pNew)
{
  ASSERT((0 != pList) && (0 != pOld) && (0 != pNew));
  ASSERT(LIST_MAX_LENGTH != pList->length);
  pList->length++;
  if (pOld->prev)
  {
    // Add not before first.
    ListItem* prev = pOld->prev;
    ListItem* next = pOld;
    
    pNew->prev = prev;
    pNew->next = next;
    prev->next = pNew;
    next->prev = pNew;
  }
  else
    // Add before first.
    list_queueHead(pList, pNew);
}

/*******************************************************************************
  Description: provides list length.
  
  Parameters: pList - list reference.
 
  Returns: list length.
*******************************************************************************/
Uint8 list_getLength(List* pList)
{
  ASSERT(0 != pList);
  return pList->length;  
}

// eof com_list.c
