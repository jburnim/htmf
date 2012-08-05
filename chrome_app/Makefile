# Copyright (c) 2012, The Native Client Authors.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#  - Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#
#  - Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in
#    the documentation and/or other materials provided with the
#    distribution.
#
#  - Neither the name of Google Inc. nor the names of its contributors
#    may be used to endorse or promote products derived from this
#    software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.


# Pepper directory for toolchain and includes.
NACL_SDK_ROOT=/Users/jburnim/nacl_sdk/pepper_21

#
# Defaults
#
NACL_WARNINGS:=-Wno-long-long -Wall -Wswitch-enum -Werror -pedantic
NACL_CCFLAGS:=-O0 -g -pthread $(NACL_WARNINGS)
NACL_CXXFLAGS:= -O0 -g -pthread -std=gnu++98 $(NACL_WARNINGS)
NACL_LDFLAGS:=-g -pthread -lppapi_cpp -lppapi

#
# Project Settings
#
HTMF_CXX:=htmf_module.cpp
HTMF_CXXFLAGS:=$(NACL_CXXFLAGS)
HTMF_LDFLAGS:=$(NACL_LDFLAGS)

#
# Default target
#
all: build/htmf.nmf

#
# Alias for standard commands
#
CP:=python $(NACL_SDK_ROOT)/tools/oshelpers.py cp
MKDIR:=python $(NACL_SDK_ROOT)/tools/oshelpers.py mkdir
MV:=python $(NACL_SDK_ROOT)/tools/oshelpers.py mv

#
# Compute path to requested NaCl Toolchain
#
OSNAME:=$(shell python $(NACL_SDK_ROOT)/tools/getos.py)
TC_PATH:=$(abspath $(NACL_SDK_ROOT)/toolchain)

#
# Verify we have a valid NACL_SDK_ROOT by looking for the toolchain directory
#
ifeq (,$(wildcard $(TC_PATH)))
$(warning No valid NACL_SDK_ROOT at $(NACL_SDK_ROOT))
ifeq ($(origin NACL_SDK_ROOT), 'file')
$(error Override the default value via enviornment variable, or command-line.)
else
$(error Fix the NACL_SDK_ROOT specified in the environment or command-line.)
endif
endif

#
# Disable DOS PATH warning when using Cygwin based NaCl tools on Windows
#
CYGWIN ?= nodosfilewarning
export CYGWIN

#
# Defaults for TOOLS
#
NEWLIB_CC?=$(TC_PATH)/$(OSNAME)_x86_newlib/bin/i686-nacl-gcc -c
NEWLIB_CXX?=$(TC_PATH)/$(OSNAME)_x86_newlib/bin/i686-nacl-g++ -c
NEWLIB_LINK?=$(TC_PATH)/$(OSNAME)_x86_newlib/bin/i686-nacl-g++ -Wl,-as-needed
NEWLIB_DUMP?=$(TC_PATH)/$(OSNAME)_x86_newlib/x86_64-nacl/bin/objdump

#
# NMF Manifiest generation
#
# Use the python script create_nmf to scan the binaries for dependencies using
# objdump.  Pass in the (-L) paths to the default library toolchains so that we
# can find those libraries and have it automatically copy the files (-s) to
# the target directory for us.
NMF:=python $(NACL_SDK_ROOT)/tools/create_nmf.py

#
# Rules for newlib toolchain
#
build:
	$(MKDIR) build

HTMF_x86_32_CXX_O:=$(patsubst %.cpp, build/%_x86_32.o,$(HTMF_CXX))
$(HTMF_x86_32_CXX_O) : build/%_x86_32.o : %.cpp $(THIS_MAKE) | build
	$(NEWLIB_CXX) -o $@ $< -m32 $(HTMF_CXXFLAGS) -DTCNAME=build

build/htmf_x86_32.nexe : $(HTMF_x86_32_CXX_O)
	$(NEWLIB_LINK) -o $@ $^ -m32 $(HTMF_LDFLAGS)
NMF_FILES+=build/htmf_x86_32.nexe

HTMF_x86_64_CXX_O:=$(patsubst %.cpp, build/%_x86_64.o,$(HTMF_CXX))
$(HTMF_x86_64_CXX_O) : build/%_x86_64.o : %.cpp $(THIS_MAKE) | build
	$(NEWLIB_CXX) -o $@ $< -m64 $(HTMF_CXXFLAGS) -DTCNAME=build

build/htmf_x86_64.nexe : $(HTMF_x86_64_CXX_O)
	$(NEWLIB_LINK) -o $@ $^ -m64 $(HTMF_LDFLAGS)
NMF_FILES+=build/htmf_x86_64.nexe

build/htmf.nmf : $(NMF_FILES)
	$(NMF) -D $(NEWLIB_DUMP) -o $@ $^ -t newlib -s build
