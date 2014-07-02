
/*******************************************************************************
  buttons_cntrl_fxns.h
  
  Buttons control interface declaration.
  Data structure declaration.
  
  Written by A.Kokhonovskiy.
*******************************************************************************/

#ifndef BUTTONS_CNTRL_FXNS_FXNS_
#define BUTTONS_CNTRL_FXNS_FXNS_

#include "buttons_fxns.h"
#include "buttons.h"

// Buttons object description.
struct Buttons
{
   Bool        isEvent;
   ButtonEvent event;
};

/*******************************************************************************
  Description: provides reference to Buttons interface implementation.
  	
  Parameters: pButtonsFxns - buttons interface implementation reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_getFxns(ButtonsFxns* pButtonsFxns);

/*******************************************************************************
  Description: inits the buttons.
  	
  Parameters: pButtons - buttons object reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_init(struct Buttons* pButtons);

/*******************************************************************************
  Description: work loop for buttons.
  	
  Parameters: pButtons - buttons object reference.
  	
  Returns: nothing.
*******************************************************************************/
void buttons_process(struct Buttons* pButtons);

#endif

// eof buttons_cntrl_fxns.h
