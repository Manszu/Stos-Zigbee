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
else
CFLAGS += -Os 
endif

#CFLAGS += -g CFLAGS += -Os
# -mcall-prologues

#CFLAGS += -DWDM_A1281_P1

TOSLIBDIR = ../TOSLib
HALDIR = ../HAL/HAL_MESHBEAN

BUILDDIR = $(shell pwd)
LIBDIR = $(BUILDDIR)/lib
LDFLAGS = -L$(TOSLIBDIR)/lib/  -L$(LIBDIR)
PROG = StackSup.elf
LIBS = $(LIBDIR)/libstackSupport.a 

CFLAGS += -I./include 
CFLAGS += -I$(TOSLIBDIR)/include 
CFLAGS += -I$(HALDIR)/base/include 
CFLAGS += -I$(HALDIR)/eZeeNet/include 

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
$(BUILDDIR)/objs/ParamServer.o \
$(BUILDDIR)/objs/sbuf.o \

all: clean $(objects) LIB
$(objects): 
	$(CC) $(CFLAGS) -c $^ -o $@
$(BUILDDIR)/objs/ParamServer.o: $(BUILDDIR)/src/ParamServer.c
$(BUILDDIR)/objs/sbuf.o: $(BUILDDIR)/src/sbuf.c

LIB : $(objects) 
	$(AR) cr $(LIBDIR)/libstackSupport.a $(objects)
	$(SIZE) -td $(LIBDIR)/libstackSupport.a
	touch $(PROG)

clean:
	rm -f $(PROG) $(objects) $(LIBS)
