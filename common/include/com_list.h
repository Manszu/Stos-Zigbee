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
  com_list.h
  
  Double linked list capability declaration.
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef LIST_H_
#define LIST_H_

#include "com_types.h"

#define LIST_MAX_LENGTH 255

typedef struct TagListItem
{
  struct TagListItem* prev;
  struct TagListItem* next;
} ListItem;

typedef struct
{
  ListItem* first;
  ListItem* last;
  Uint8 length;
} List;

/*******************************************************************************
  Description: inits list.
  
  Parameters: pList - list reference.
  
  Returns: nothing.
*******************************************************************************/
void list_init(List* pList);

/*******************************************************************************
  Description: provides list head reference.
  
  Parameters: pList - list reference.
  
  Returns: list head reference.
*******************************************************************************/
ListItem* list_peekHead(List* pList);

/*******************************************************************************
  Description: provides list tail reference.
  
  Parameters: pList - list reference.
  
  Returns: list tail reference.
*******************************************************************************/
ListItem* list_peekTail(List* pList);

/*******************************************************************************
  Description: adds item to list head.
  
  Parameters: pList - list reference.
              pListItem - item reference to add.
  
  Returns: nothing.
*******************************************************************************/
void list_queueHead(List* pList, ListItem* pListItem);

/*******************************************************************************
  Description: adds item to list tail.
  
  Parameters: pList - list reference.
              pListItem - item reference to add.
  
  Returns: nothing.
*******************************************************************************/
void list_queueTail(List* pList, ListItem* pListItem);

/*******************************************************************************
  Description: removes item from list head.
  
  Parameters: pList - list reference.
  
  Returns: removed item reference.
*******************************************************************************/
ListItem* list_dequeueHead(List* pList);

/*******************************************************************************
  Description: removes item from list tail.
  
  Parameters: pList - list reference.
  
  Returns: removed item reference.
*******************************************************************************/
ListItem* list_dequeueTail(List* pList);

/*******************************************************************************
  Description: unlinks item from list.
  
  Parameters: pList - list reference.
              pListItem - item reference to unlink.
  
  Returns: nothing.
*******************************************************************************/
void list_unlink(List* pList, ListItem* pListItem);

/*******************************************************************************
  Description: adds item to list after pointed one.
  
  Parameters: pList - list reference.
              pNew - item reference to add.
              pOld - pointed item reference.
 
  Returns: nothing.
*******************************************************************************/
void list_append(List* pList, ListItem* pOld, ListItem* pNew);

/*******************************************************************************
  Description: adds item to list before pointed one.
  
  Parameters: pList - list reference.
              pNew - item reference to add.
              pOld - pointed item reference.
 
  Returns: nothing.
*******************************************************************************/
void list_insert(List* pList, ListItem* pOld, ListItem* pNew);

/*******************************************************************************
  Description: provides list length.
  
  Parameters: pList - list reference.
 
  Returns: list length.
*******************************************************************************/
Uint8 list_getLength(List* pList);

#endif

// eof com_list.h
