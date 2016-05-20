# Top level Makefile for Plexil

# Copyright (c) 2006-2016, Universities Space Research Association (USRA).
#  All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in the
#       documentation and/or other materials provided with the distribution.
#     * Neither the name of the Universities Space Research Association nor the
#       names of its contributors may be used to endorse or promote products
#       derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY USRA ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
# DISCLAIMED. IN NO EVENT SHALL USRA BE LIABLE FOR ANY DIRECT, INDIRECT,
# INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
# BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
# OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
# ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
# TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
# USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

MY_PLEXIL_HOME := $(shell pwd)
ifneq ($(PLEXIL_HOME),)
ifneq ($(PLEXIL_HOME),$(MY_PLEXIL_HOME))
$(error Environment variable PLEXIL_HOME is in error. It must be set to $(MY_PLEXIL_HOME) before proceeding)
endif
endif

export PLEXIL_HOME := $(MY_PLEXIL_HOME)

include makeinclude/standard-defs.make

#
# Locations for GNU autotools
#

# TODO? test for existence
AUTOCONF := autoconf
AUTOMAKE := automake
AUTORECONF := autoreconf
LIBTOOLIZE := libtoolize

# Configuration options for src/configure
CONF_BUILD_OPTS := --enable-debug-listener

# TODO figure out what to do with these
CONF_MODULE_OPTS := --enable-udp --enable-ipc --enable-sas --enable-test-exec

# *** TEMPORARY *** Comment out when done porting to C++11
CONF_MODULE_OPTS += --enable-module-tests

ifneq ($(PLEXIL_SHARED),)
CONF_BUILD_OPTS += --enable-shared --disable-static
else
CONF_BUILD_OPTS += --disable-shared --enable-static
endif

# Primary target
plexil-default: tools

# The whole shooting match
all: universalExec TestExec IpcAdapter UdpAdapter plexil-compiler plexilscript checker plexilsim pv robosim sample

# Just the tools without the examples
tools: universalExec TestExec IpcAdapter UdpAdapter plexil-compiler plexilscript checker plexilsim pv

# Core facilities
essentials: universalExec TestExec IpcAdapter UdpAdapter plexil-compiler plexilscript checker plexilsim

#
# Standalone targets
#

checker:
	(cd checker && ant jar)

pv:
	(cd viewers/pv && ant jar)

plexil-compiler:
	$(MAKE) -C compilers/plexil

plexilscript:
	(cd compilers/plexilscript && ant install)

#
# Targets which depend on the Automake targets below
#

robosim: ipc utils
	$(MAKE) -C examples/robosim

sample: universalExec
	$(MAKE) -C examples/sample-app
	$(MAKE) -C examples/sample-app1

#
# Targets under the Automake build system
#
# Dependencies may be too messy to capture here
#

app-framework: lib/libPlexilAppFramework.$(SUFSHARE)

exec-core: lib/libPlexilExec.$(SUFSHARE)

GanttListener: lib/libGanttListener.$(SUFSHARE)

ipc: lib/libipc.a bin/central

IpcUtils: lib/libIpcUtils.$(SUFSHARE)

IpcAdapter: lib/libIpcAdapter.$(SUFSHARE)

Launcher: lib/libLauncher.$(SUFSHARE)

LuvListener: lib/libLuvListener.$(SUFSHARE)

PlanDebugListener: lib/libPlanDebugListener.$(SUFSHARE)

plexilsim: bin/simulator

pugixml: lib/libpugixml.$(SUFSHARE)

sockets: lib/libPlexilSockets.$(SUFSHARE)

TestExec: bin/TestExec

UdpAdapter: lib/libUdpAdapter.$(SUFSHARE)

universalExec: bin/universalExec 

utils: lib/libPlexilUtils.$(SUFSHARE)

value: lib/libPlexilValue.$(SUFSHARE)

bin/central lib/libIpc.a : most
bin/simulator : most
bin/TestExec : most
bin/universalExec : most
lib/libGanttListener.$(SUFSHARE) : most
lib/libipc.a : most
lib/libIpcAdapter.$(SUFSHARE) : most
lib/libIpcUtils.$(SUFSHARE) : most
lib/libLuvListener.$(SUFSHARE) : most
lib/libLauncher.$(SUFSHARE) : most
lib/libPlanDebugListener.$(SUFSHARE) : most
lib/libPlexilAppFramework.$(SUFSHARE) : most
lib/libPlexilExec.$(SUFSHARE) : most
lib/libPlexilExpr.$(SUFSHARE) : most
lib/libPlexilIntfc.$(SUFSHARE) : most
lib/libPlexilSockets.$(SUFSHARE) : most
lib/libPlexilUtils.$(SUFSHARE) : most
lib/libPlexilValue.$(SUFSHARE) : most
lib/libpugixml.$(SUFSHARE) : most
lib/libUdpAdapter.$(SUFSHARE) : most

most: most-build most-install

most-build: src/Makefile
	$(MAKE) -C src

most-install: most-build src/Makefile
	$(MAKE) -C src install

# Be sure to inherit compiler options from makeinclude directory
src/Makefile: src/configure src/Makefile.am makeinclude/standard-defs.make makeinclude/platform-defs.make makeinclude/platform-$(TARGET_OS).make
	cd ./src && ./configure --prefix="$(PLEXIL_HOME)" \
 CC="$(CC)" CFLAGS="$(CFLAGS)" CXX="$(CXX)" CXXFLAGS="$(CXXFLAGS)" \
 $(CONF_BUILD_OPTS) $(CONF_MODULE_OPTS)


#
# Bootstrapping autobuild files
#

src/configure: src/configure.ac Makefile
	cd ./src && $(AUTORECONF) -f -i

#
# End Automake targets
#

clean:
	-$(MAKE) -C compilers/plexil $@
	-$(MAKE) -C examples/robosim $@
	-$(MAKE) -C examples/sample-app $@
	-$(MAKE) -C src $@
	(cd checker && ant $@)
	(cd compilers/plexilscript && ant $@)
	(cd jars && $(RM) -f plexilscript.jar)	
	(cd viewers/pv && ant $@)
	-$(RM) lib/lib* bin/*
	@ echo Done.

# Clean up after autotools
squeaky-clean: | clean
	(cd src && $(RM) -f */Makefile */Makefile.in)
	(cd src/apps && $(RM) -f */Makefile */Makefile.in)
	(cd src/interfaces && $(RM) -f */Makefile */Makefile.in)
	(cd src/third-party/ipc && $(RM) Makefile Makefile.in)
	(cd src/third-party/pugixml/src && $(RM) Makefile Makefile.in)
	(cd src && $(RM) -f Makefile Makefile.in configure config.guess config.sub \
 cppcheck.sh install-sh libtool)

# *** TODO: release target(s) ***

# Convenience targets

# Create an Emacs tags file capturing most relevant file types
#   Note: The indirection forces recreation of TAGS file in
#   MacOS, which is case-insensitive.
#
tags: alltags

ctags: 
	@ find . \( -name "*.cc" -or -name "*.cpp" -or -name "*.hh" -or -name "*.hpp" -or -name Makefile \) | etags -

jtags:
	@ find . \( -name "*.java" \) | etags -

alltags:
	@ find . \( -name "*.cc" -or -name "*.cpp" -or -name "*.hh" -or -name "*.hpp" -or -name "*.java" -or -name "*.xml" -or -name Makefile \) | etags -

.PHONY: app-framework exec-core GanttListener ipc IpcAdapter IpcUtils 

.PHONY: alltags clean ctags jtags most most-build most-install plexil-default squeaky-clean tags
