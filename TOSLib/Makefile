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
SHELL = /bin/bash

CPU=atmega1281

CFLAGS = -mmcu=$(CPU)

ifdef DEBUG_LIBS_BUILD
CFLAGS += -Os 
CFLAGS += -g
else
CFLAGS += -Os 
endif

HALLIB=../HAL/HAL_MESHBEAN/
CFLAGS += -I./include/ -I$(HALLIB)/base/include 
BUILDDIR = $(shell pwd)
OBJDIR = $(BUILDDIR)/objs
SRCDIR = $(BUILDDIR)/src
LIBDIR = $(BUILDDIR)/lib
EXTERNAL_LIB =c
LDFLAGS = -g -L. -L$(HALLIB)/lib -L./lib #-l$(EXTERNAL_LIB) -L/usr/$(CROSS_COMPILE)/lib/
PROG = toslib.elf
includedir = $(BUILDDIR)/include

CFLAGS += -I$(includedir)

CC		= $(CROSS_COMPILE)-gcc
AS              = $(CROSS_COMPILE)-as
LD              = $(CROSS_COMPILE)-ld
CC              = $(CROSS_COMPILE)-gcc
CPP             = $(CROSS_COMPILE)-g++
AR              = $(CROSS_COMPILE)-ar
NM              = $(CROSS_COMPILE)-nm
STRIP           = $(CROSS_COMPILE)-strip
OBJCOPY         = $(CROSS_COMPILE)-objcopy
OBJDUMP         = $(CROSS_COMPILE)-objdump

objects = $(OBJDIR)/sched.o $(OBJDIR)/tos.o $(OBJDIR)/main.o

LIB = $(LIBDIR)/libtos.a

all: clean all-am

all-am:  $(objects) $(LIB) 
#	mkdir -p $(BUILDDIR)/objs
#	touch TOSlib.elf

$(objects):
	$(CC) $(CFLAGS) -c $^ -o $@ 
$(OBJDIR)/sched.o: $(SRCDIR)/sched.c 
$(OBJDIR)/tos.o: $(SRCDIR)/tos.c
$(OBJDIR)/main.o: $(SRCDIR)/main.c

$(LIB): $(objects)
	$(AR) cr $(LIB) $(objects) 
	touch $(PROG) 
#	$(OBJCOPY) --strip-unneeded $(objects) lib$(LIB).o


#$(PROG): $(LIB)
#		$(CC) -mmcu=$(CPU) $(LDFLAGS)  -ltos -lhal -o $(PROG)

clean:
	rm -rf $(objects) $(LIB) TOSlib.elf


