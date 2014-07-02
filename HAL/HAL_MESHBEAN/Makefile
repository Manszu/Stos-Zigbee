##
# Copyright (c) 2005-2007 LuxLabs Ltd. dba MeshNetics, All Rights Reserved.
#
# No changes (alteration, merge, modification, reverse engineer) can be applied
# to the source or binary code without specific prior written permission.   
#
# Redistribution, sublicensing and use in source and binary forms 
# are permitted provided that the following conditions are met:
#
# - Redistribution, sublicensing of this source code and its derivatives is 
#  allowed only with expressed written permission of MeshNetics and must 
#  retain the above copyright notice, this list of conditions and 
#  the following disclaimer.
#
# - Redistribution, sublicensing of this software or its derivatives in binary
#  form is allowed only with expressed written permission of MeshNetics and 
#  must reproduce the above copyright notice, this list of conditions and 
#  the following disclaimer in the documentation and/or other materials 
#  provided with the distribution.
#
# Neither the name of LuxLabs Ltd., MeshNetics, or the names of contributors 
# may be used to endorse or promote products derived from this software 
# without specific prior written permission.
#
# This software is provided "AS IS," without a warranty or support of any kind.
# ALL EXPRESS OR IMPLIED CONDITIONS, REPRESENTATIONS AND WARRANTIES, INCLUDING 
# ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
# OR NON-INFRINGEMENT, ARE HEREBY EXCLUDED. MESHNETICS AND ITS LICENSORS SHALL 
# NOT BE LIABLE FOR ANY DAMAGES OR LIABILITIES SUFFERED BY LICENSEE AS A RESULT
# OF OR RELATING TO USE, MODIFICATION OR DISTRIBUTION OF THE SOFTWARE OR ITS 
# DERIVATIVES. IN NO EVENT WILL MESHNETICS OR ITS LICENSORS BE LIABLE 
# FOR ANY LOST REVENUE, PROFIT OR DATA, OR FOR DIRECT, INDIRECT, SPECIAL, 
# CONSEQUENTIAL, INCIDENTAL OR PUNITIVE DAMAGES, HOWEVER CAUSED AND REGARDLESS 
# OF THE THEORY OF LIABILITY, ARISING OUT OF THE USE OF OR INABILITY TO USE 
# SOFTWARE, EVEN IF MESHNETICS HAS BEEN ADVISED OF THE POSSIBILITY 
# OF SUCH DAMAGES.
#
# You acknowledge that this software is not designed, licensed or intended for 
# use in the design, construction, operation or maintenance of any nuclear 
# facility, medical or other mission-critical system.
##

CROSS_COMPILE = avr
CPU = atmega1281

SHELL = /bin/bash

CFLAGS =  -mmcu=$(CPU) -fsigned-char
ifdef DEBUG_LIBS_BUILD
CFLAGS += -Os 
CFLAGS += -g
#CFLAGS += -DDEBUG_SUPERVISE_ATOMIC     -DMAX_ATOMIC_DURATION=4000
#CFLAGS += -DDEBUG_PROFILE_HPLCLOCK_ISR -DDEBUG_PROFILE_APPCLOCK_ISR 
#CFLAGS += -DDEBUG_PROFILE_RF230_ISR
#CFLAGS += -DDEBUG_PROFILE_SLEEPTIMERCLOCK_ISR
#CFLAGS += -DDEBUG_PROFILE_TOSH_INT 
#CFLAGS += -DDEBUG_PROFILE_ATOMIC
else
CFLAGS += -Os
endif
# -mcall-prologues
CFLAGS += -DWDM_A1281_P1
CFLAGS += -DRTC_CALIBRATION
############################
CFLAGS += -DSLEEPTIMER_PRESCALER=SLEEPTIMER_PRESCALER_1024 # -D_DEBUG_ #-D_STACK_
#CFLAGS += -DUART0_RX_BUFF_SIZE=256 -DUART0_TX_BUFF_SIZE=256 -DUART1_RX_BUFF_SIZE=256 -DUART1_TX_BUFF_SIZE=256

############################
#Define a number of user timers here.
CFLAGS += -DAPP_NUM_TIMERS=4

## ZigBit with amplifier.
##CFLAGS += -DZDM_A1281_PN

## Meshbean with ZigBit with amplifier.
##CFLAGS += -DWDB_A1281_PN

WSNDIR = ../../
TOSLIBDIR = $(WSNDIR)/TOSLib
TOSLIB = tos

CFLAGS += -I./base/include -I./base/interface -I./eZeeNet/include -I./meshBean2/include -I$(TOSLIBDIR)/include -IC:/WinAVR-20070525/avr/include
CFLAGS += -I$(WSNDIR)/common/include -I$(WSNDIR)/common/implementations -I$(WSNDIR)/common/types -I$(WSNDIR)/common/interfaces 
BUILDDIR = $(shell pwd)
LIBDIR = $(BUILDDIR)/lib
LDFLAGS = -L$(TOSLIBDIR)/lib/  -L$(LIBDIR)
PROG = HAL.elf
LIBS = $(LIBDIR)/libmeshBean2.a $(LIBDIR)/libZigBitExt.a $(LIBDIR)/libZigBitInt.a $(LIBDIR)/wdtinit.o

AS              = $(CROSS_COMPILE)-as
LD              = $(CROSS_COMPILE)-ld
CC		= $(CROSS_COMPILE)-gcc
CPP             = $(CROSS_COMPILE)-g++
AR              = $(CROSS_COMPILE)-ar
NM              = $(CROSS_COMPILE)-nm
STRIP           = $(CROSS_COMPILE)-strip
OBJCOPY         = $(CROSS_COMPILE)-objcopy
OBJDUMP         = $(CROSS_COMPILE)-objdump
SIZE            = $(CROSS_COMPILE)-size

objects = \
$(BUILDDIR)/base/objs/adc.o \
$(BUILDDIR)/base/objs/halmain.o \
$(BUILDDIR)/base/objs/i2c.o \
$(BUILDDIR)/base/objs/i2cpacket.o \
$(BUILDDIR)/base/objs/hplinterrupt.o \
$(BUILDDIR)/base/objs/tos.o \
$(BUILDDIR)/base/objs/gpio.o \
$(BUILDDIR)/base/objs/irq.o \
$(BUILDDIR)/base/objs/w1.o \
$(BUILDDIR)/base/objs/hplw1.o \
$(BUILDDIR)/base/objs/appclock.o \
$(BUILDDIR)/base/objs/apptimer.o \
$(BUILDDIR)/base/objs/resetreason.o \
$(BUILDDIR)/base/objs/nbo.o \
$(BUILDDIR)/base/objs/pwm.o \
$(BUILDDIR)/eZeeNet/objs/hplspi.o \
$(BUILDDIR)/eZeeNet/objs/uid.o \
$(BUILDDIR)/eZeeNet/objs/eeprom.o \
$(BUILDDIR)/eZeeNet/objs/hplclock.o \
$(BUILDDIR)/eZeeNet/objs/hplat86rf230m.o\
$(BUILDDIR)/eZeeNet/objs/sleeptimerclock.o \
$(BUILDDIR)/eZeeNet/objs/sleeptimer.o \
$(BUILDDIR)/base/objs/uart_avr1281.o \
$(BUILDDIR)/base/objs/uart0.o \
$(BUILDDIR)/base/objs/uart1.o \
$(BUILDDIR)/base/objs/uart0spi.o 


ext = $(BUILDDIR)/base/objs/clkctrlext.o \
$(BUILDDIR)/eZeeNet/objs/sleepext.o \

int = $(BUILDDIR)/base/objs/clkctrlint.o \
$(BUILDDIR)/eZeeNet/objs/sleepint.o \
$(BUILDDIR)/base/objs/calibration.o \

meshbean2 = $(BUILDDIR)/meshBean2/objs/pwrctrl.o \
$(BUILDDIR)/meshBean2/objs/lm73.o \
$(BUILDDIR)/meshBean2/objs/tsl2550.o \
$(BUILDDIR)/meshBean2/objs/battery.o \
$(BUILDDIR)/meshBean2/objs/sensors.o \
$(BUILDDIR)/meshBean2/objs/leds.o \
$(BUILDDIR)/meshBean2/objs/buttons.o \
$(BUILDDIR)/meshBean2/objs/sliders.o \

all: clean wdtinit.c LIBEXT LIBINT LIBBOARD

$(objects): 
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/base/objs/halmain.o: $(BUILDDIR)/base/src/halmain.c
$(BUILDDIR)/base/objs/wdtinit.o: $(BUILDDIR)/base/src/wdtinit.c
$(BUILDDIR)/base/objs/hplinterrupt.o: $(BUILDDIR)/base/src/hplinterrupt.c
$(BUILDDIR)/base/objs/i2c.o: $(BUILDDIR)/base/src/i2c.c
$(BUILDDIR)/base/objs/i2cpacket.o: $(BUILDDIR)/base/src/i2cpacket.c
$(BUILDDIR)/base/objs/tos.o: $(BUILDDIR)/base/src/tos.c
$(BUILDDIR)/base/objs/adc.o: $(BUILDDIR)/base/src/adc.c
$(BUILDDIR)/base/objs/gpio.o: $(BUILDDIR)/base/src/gpio.c
$(BUILDDIR)/base/objs/irq.o: $(BUILDDIR)/base/src/irq.c
$(BUILDDIR)/base/objs/w1.o: $(BUILDDIR)/base/src/w1.c
$(BUILDDIR)/base/objs/hplw1.o: $(BUILDDIR)/base/src/hplw1.s
$(BUILDDIR)/base/objs/appclock.o: $(BUILDDIR)/base/src/appclock.c
$(BUILDDIR)/base/objs/apptimer.o: $(BUILDDIR)/base/src/apptimer.c
$(BUILDDIR)/base/objs/resetreason.o: $(BUILDDIR)/base/src/resetreason.c
$(BUILDDIR)/base/objs/nbo.o: $(BUILDDIR)/base/src/nbo.c
$(BUILDDIR)/base/objs/pwm.o: $(BUILDDIR)/base/src/pwm.c
$(BUILDDIR)/eZeeNet/objs/sleeptimer.o: $(BUILDDIR)/eZeeNet/src/sleeptimer.c
$(BUILDDIR)/eZeeNet/objs/sleeptimerclock.o: $(BUILDDIR)/eZeeNet/src/sleeptimerclock.c
$(BUILDDIR)/eZeeNet/objs/hplspi.o: $(BUILDDIR)/eZeeNet/src/hplspi.c
$(BUILDDIR)/eZeeNet/objs/hplat86rf230m.o: $(BUILDDIR)/eZeeNet/src/hplat86rf230m.c
$(BUILDDIR)/eZeeNet/objs/uid.o: $(BUILDDIR)/eZeeNet/src/uid.c
$(BUILDDIR)/eZeeNet/objs/eeprom.o: $(BUILDDIR)/eZeeNet/src/eeprom.c
$(BUILDDIR)/eZeeNet/objs/hplclock.o: $(BUILDDIR)/eZeeNet/src/hplclock.c
$(BUILDDIR)/base/objs/uart_avr1281.o: $(BUILDDIR)/base/src/uart_avr1281.c
$(BUILDDIR)/base/objs/uart0.o: $(BUILDDIR)/base/src/uart0.c
$(BUILDDIR)/base/objs/uart1.o: $(BUILDDIR)/base/src/uart1.c
$(BUILDDIR)/base/objs/uart0spi.o: $(BUILDDIR)/base/src/uart0spi.c

$(ext): 
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/base/objs/clkctrlext.o: $(BUILDDIR)/base/src/clkctrl.c
$(BUILDDIR)/eZeeNet/objs/sleepext.o: $(BUILDDIR)/eZeeNet/src/sleep.c

$(int): 
	$(CC) $(CFLAGS) -DHAL_RC_OSCILLATOR -c $^ -o $@
$(BUILDDIR)/base/objs/clkctrlint.o: $(BUILDDIR)/base/src/clkctrl.c
$(BUILDDIR)/eZeeNet/objs/sleepint.o: $(BUILDDIR)/eZeeNet/src/sleep.c
$(BUILDDIR)/base/objs/calibration.o: $(BUILDDIR)/base/src/calibration.c

$(meshbean2): 
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/meshBean2/objs/lm73.o: $(BUILDDIR)/meshBean2/src/lm73.c
$(BUILDDIR)/meshBean2/objs/tsl2550.o: $(BUILDDIR)/meshBean2/src/tsl2550.c
$(BUILDDIR)/meshBean2/objs/battery.o: $(BUILDDIR)/meshBean2/src/battery.c
$(BUILDDIR)/meshBean2/objs/sensors.o: $(BUILDDIR)/meshBean2/src/sensors.c
$(BUILDDIR)/meshBean2/objs/leds.o: $(BUILDDIR)/meshBean2/src/leds.c
$(BUILDDIR)/meshBean2/objs/buttons.o: $(BUILDDIR)/meshBean2/src/buttons.c
$(BUILDDIR)/meshBean2/objs/sliders.o: $(BUILDDIR)/meshBean2/src/sliders.c
$(BUILDDIR)/meshBean2/objs/pwrctrl.o: $(BUILDDIR)/meshBean2/src/pwrctrl.c

wdtinit.c:
	$(CC) $(CFLAGS) -c $(BUILDDIR)/base/src/wdtinit.c -o $(LIBDIR)/wdtinit.o

LIBEXT : $(objects) $(ext)
	$(AR) cr $(LIBDIR)/libZigBitExt.a $(objects) $(ext)

LIBINT : $(objects) $(int)
	$(AR) cr $(LIBDIR)/libZigBitInt.a $(objects) $(int)

LIBBOARD : $(meshbean2)
	$(AR) cr $(LIBDIR)/libmeshBean2.a $(meshbean2)
	$(SIZE) -td $(LIBDIR)/libZigBitExt.a
	$(SIZE) -td $(LIBDIR)/libZigBitInt.a
	$(SIZE) -td $(LIBDIR)/libmeshBean2.a
	touch $(PROG)

clean:
	rm -f $(PROG) $(objects) $(ext) $(int) $(meshbean2) $(LIBS)
