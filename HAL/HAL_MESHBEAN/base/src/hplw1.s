;;
; Copyright (c) 2005-2007 LuxLabs Ltd. dba MeshNetics, All Rights Reserved.
;
; No changes (alteration, merge, modification, reverse engineer) can be applied
; to the source or binary code without specific prior written permission.   
;
; Redistribution, sublicensing and use in source and binary forms 
; are permitted provided that the following conditions are met:
;
; - Redistribution, sublicensing of this source code and its derivatives is 
;  allowed only with expressed written permission of MeshNetics and must 
;  retain the above copyright notice, this list of conditions and 
;  the following disclaimer.
;
; - Redistribution, sublicensing of this software or its derivatives in binary
;  form is allowed only with expressed written permission of MeshNetics and 
;  must reproduce the above copyright notice, this list of conditions and 
;  the following disclaimer in the documentation and/or other materials 
;  provided with the distribution.
;
; Neither the name of LuxLabs Ltd., MeshNetics, or the names of contributors 
; may be used to endorse or promote products derived from this software 
; without specific prior written permission.
;
; This software is provided "AS IS," without a warranty or support of any kind.
; ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING 
; ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
; OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MESHNETICS AND ITS LICENSORS SHALL 
; NOT BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
; OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS 
; DERIVATIVES. IN NO EVENT WILL MESHNETICS OR ITS LICENSORS BE LIABLE 
; FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, 
; CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS 
; OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE 
; SOFTWARE, EVEN IF MESHNETICS HAS BEEN ADVISED OF THE POSSIBILITY 
; OF SUCH DAMAGES.
;
; You acknowledge that this software is not designed, licensed or intended for 
; use in the design, construction, operation or maintenance of any nuclear 
; facility, medical or other mission-critical system.
;;


.set __w1_port, 0x14; PORTG
.set __w1_bit, 5; PORTG5
.set __w1_ddr, 0x13; DDRG
.set __w1_pin, 0x12; PING

;*******************************************************************************
;  Description: Resets all devices connected to the bus. Function asserts on 
;               the bus reset pulse and detects presence pulse. The result is 
;               contained in r24.
;
;  Parameters: nothing.
;
;  Returns: W1_SUCCESS_STATUS - If device(s) was(were) detected.
;           W1_NO_DEVICE_STATUS - If device(s) was(were) not detected.
;******************************************************************************/
.global w1_reset
.type w1_reset,@function
w1_reset:
  ; Store SREG
  push r23
  in r23, 0x3F
  ; Disable interrupts
  cli
  ; Pull down
  sbi __w1_ddr, __w1_bit
  cbi __w1_port, __w1_bit
  ; Reset Low Time (500 us)
  ldi r24, 250
  call __delay_us
  ldi r24, 250
  call __delay_us
  ; Tri-state (external pullup)
  cbi __w1_ddr, __w1_bit
  ; Presence-Detect Sample Time (70 us)
  ldi r24, 70
  call __delay_us
  ; Precense-Detect
  ldi r24, 0x01
  sbic __w1_pin, __w1_bit
  ldi r24, 0x00
  push r24
  ; Tail of Reset High Time
  ldi r24, 240
  call __delay_us
  pop r24
  ; Restore SREG
  out 0x3F, r23
  pop r23
  ret

;/*******************************************************************************
;  Description: Reads bit from the bus.
;
;  Parameters: nothing.
;
;  Returns: bit read from the bus in r24.
;*******************************************************************************/
.global w1_read_bit
.type w1_read_bit,@function
w1_read_bit:
  ; Store SREG
  push r23
  in r23, 0x3F
  ; Disable interrupts
  cli
  ; Pull down
  sbi __w1_ddr, __w1_bit
  cbi __w1_port, __w1_bit
  ; Read Low Time (2 us)
  ldi r24, 2
  call __delay_us
  ; Tri-state (external pullup)
  cbi __w1_ddr, __w1_bit
  ; Tail of Read Sample Time (10 us)
  ldi r24, 10
  call __delay_us
  ; Read Sample
  clc
  sbic __w1_pin, __w1_bit
  sec
  rol r24
  ; Tail of Timeslot Duration
  push r24
  ldi r24, 100
  call __delay_us
  pop r24
  ; Restore SREG
  out 0x3F, r23
  pop r23
  ret

;/*******************************************************************************
;  Description: Reads byte from the bus.
;
;  Parameters: nothing.
;
;  Returns: byte read from the bus in r24.
;*******************************************************************************/
.global w1_read
.type w1_read,@function
  w1_read:
  push r25
  push r23
  ldi r25, 8
__read_bit_again:
  call w1_read_bit
  ror r24
  ror r23
  dec r25
  tst r25
  brne __read_bit_again
  mov r24, r23
  pop r23
  pop r25
  ret

;/*******************************************************************************
;  Description: Writes bit to the bus.
;
;  Parameters: value - bit that should be written to the bus.
;
;  Returns: nothing.
;*******************************************************************************/
.global w1_write_bit
.type w1_write_bit,@function
w1_write_bit:
  ; Store SREG
  push r23
  in r23, 0x3F
  ; Disable interrupts
  cli
  ; Pull down
  cbi __w1_port, __w1_bit
  sbi __w1_ddr, __w1_bit
  ; Write-1 Low Time
  push r24
  ldi r24, 2
  call __delay_us
  pop r24
  ; Write bit
  ror r24
  brcc __w1_write_zero
  ; Write-One -> tri-state (external pullup)
  cbi __w1_ddr, __w1_bit
__w1_write_zero:
  ; Tail of Timeslot Duration
  push r24
  ldi r24, 100
  call __delay_us
  pop r24
  ; Tri-state (external pullup)
  cbi __w1_ddr, __w1_bit
  ; Restore SREG
  out 0x3F, r23
  pop r23
  ret

;/*******************************************************************************
;  Description: Writes byte to the bus.
;
;  Parameters: value - byte that should be written to the bus.
;
;  Returns: nothing.
;*******************************************************************************/
.global w1_write
.type w1_write,@function
w1_write:
  push r25
  ldi r25, 8
__write_bit_again:
  call w1_write_bit
  dec r25
  tst r25
  brne __write_bit_again
  pop r25
  ret

;/*******************************************************************************
;  Description: Delay in microseconds. Note that these routine are clock 
;               speed sensitive! (CLK = 4MHz)
;
;  Parameters: us - delay time in microseconds
;
;  Returns: nothing.
;*******************************************************************************/
.global __delay_us
.type __delay_us,@function
__delay_us:
__w0: 
  dec r24
  tst r24
  brne __w0
  ret

; eof plw1.s
