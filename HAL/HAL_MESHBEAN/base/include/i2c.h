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

#ifndef HPLI2CM_H_
#define HPLI2CM_H_
#include <avrhardware.h>
#include <tos.h>
#include <i2cpacket.h>

/*******************************************************************************
  Description: Inits TWI module. Setup teh speed of TWI.

  Parameters: i2cMode - the speed of TWI.

  Returns: FAIL - unsupported speed.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_init( const I2CMode_t* );

/*******************************************************************************
  Description: Directs TWI to send start condition.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendStart();

/*******************************************************************************
  Description: Directs TWI to send stop condition.

  Parameters: nothing.

  Returns: SUCCESS.
*******************************************************************************/
result_t i2c_sendEnd();

/*******************************************************************************
  Description: Begins to read an byte from TWI.

  Parameters: ack - defines the need to send ACK after an byte was recieved.

  Returns: FAIL - the TWI is busy or the TWI improperly initialized.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_read(bool ack);

/*******************************************************************************
  Description: Begins the writing an byte to TWI.

  Parameters: data - an byte for the sending.

  Returns: FAIL -  the TWI is busy or the TWI improperly initialized.
           SUCCESS - other case.
*******************************************************************************/
result_t i2c_write(char data);

/*******************************************************************************
  Description:  Notification that the stop condition was sent 

  Parameters:   none.

  Returns:      SUCCESS - always.          
*******************************************************************************/
result_t i2c_sendEndDone(void);

/*******************************************************************************
  Description:  Notification that the start condition was sent.

  Parameters:   none.

  Returns:      SUCCESS - always.      
*******************************************************************************/
result_t i2c_sendStartDone(void);

/*******************************************************************************
  Description:  Notification of a byte sucessfully written to the bus.

  Parameters:   result - contains result of previous operation.

  Returns:      FAIL - there is the state error.
                SUCCESS - other case.
*******************************************************************************/
result_t i2c_writeDone(bool);

/*******************************************************************************
  Description:  Reads a byte off the TWI and adds it to the packet.

  Parameters:   char - read byte

  Returns:      SUCCESS - always.        
*******************************************************************************/
result_t i2c_readDone(char);

#endif /* HPLI2CM_H_*/
// eof i2c.h
