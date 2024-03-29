# Copyright (c) 2006-2021, Universities Space Research Association (USRA).
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

AUTOMAKE_OPTIONS = subdir-objects

lib_LTLIBRARIES = libPlexilIntfc.la

libPlexilIntfc_la_CPPFLAGS = $(AM_CPPFLAGS) -I@top_srcdir@/expr \
 -I@top_srcdir@/value -I@top_srcdir@/utils

# Exposed APIs
include_HEADERS = Command.hh CommandFunction.hh CommandHandleVariable.hh \
 CommandImpl.hh commandUtils.hh Dispatcher.hh ExprVec.hh InputQueue.hh \
 InterfaceError.hh Lookup.hh LookupReceiver.hh Message.hh \
 QueueEntry.hh ResourceArbiterInterface.hh State.hh StateCache.hh Update.hh

# Implementation details which don't need to be advertised
noinst_HEADERS = CachedValue.hh CachedValueImpl.hh CommandOperator.hh \
 StateCacheEntry.hh

libPlexilIntfc_la_SOURCES = CachedValue.cc CachedValueImpl.cc \
 CommandFunction.cc CommandHandleVariable.cc CommandImpl.cc CommandOperator.cc \
 CommandOperatorImpl.cc commandUtils.cc Dispatcher.cc ExprVec.cc \
 InterfaceError.cc Lookup.cc QueueEntry.cc ResourceArbiterInterface.cc \
 State.cc StateCacheEntry.cc StateCache.cc Update.cc

if MODULE_TESTS_OPT
  bin_PROGRAMS = test/intfc-module-tests
  test_intfc_module_tests_SOURCES = @top_srcdir@/expr/test/TrivialListener.cc \
 test/lookupsTest.cc test/serializeTest.cc test/stateTest.cc \
 test/intfc-test-module.cc
  test_intfc_module_tests_CPPFLAGS = $(libPlexilIntfc_la_CPPFLAGS)
  test_intfc_module_tests_LDADD = libPlexilIntfc.la \
 @top_builddir@/expr/libPlexilExpr.la @top_builddir@/value/libPlexilValue.la \
 @top_builddir@/utils/libPlexilUtils.la
endif
