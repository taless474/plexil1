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

bin_PROGRAMS = universalExec
universalExec_SOURCES = UniversalExec.cc

universalExec_CPPFLAGS = $(AM_CPPFLAGS) -I@top_srcdir@/third-party/pugixml/src \
 -I@top_srcdir@/app-framework -I@top_srcdir@/exec -I@top_srcdir@/intfc \
 -I@top_srcdir@/expr -I@top_srcdir@/value -I@top_srcdir@/utils

universalExec_LDADD = @top_builddir@/third-party/pugixml/src/libpugixml.la \
 @top_builddir@/app-framework/libPlexilAppFramework.la \
 @top_builddir@/exec/libPlexilExec.la @top_builddir@/intfc/libPlexilIntfc.la \
 @top_builddir@/expr/libPlexilExpr.la @top_builddir@/value/libPlexilValue.la \
 @top_builddir@/utils/libPlexilUtils.la

# These are required whether or not the build is statically linked, if
# support for the PLEXIL Viewer is enabled.
if VIEWER_OPT
  universalExec_CPPFLAGS += -I@top_srcdir@/interfaces/LuvListener
  universalExec_LDADD += @top_builddir@/interfaces/LuvListener/libLuvListener.la \
   @top_builddir@/interfaces/Sockets/libPlexilSockets.la
endif

# These additions are only required if the build is statically linked.
if STATIC_LIB_OPT

if DEBUG_LISTENER_OPT
  universalExec_LDADD += @top_builddir@/interfaces/PlanDebugListener/libPlanDebugListener.la
endif

if IPC_OPT
  universalExec_LDADD += @top_builddir@/interfaces/IpcAdapter/libIpcAdapter.la \
 @top_builddir@/interfaces/IpcUtils/libIpcUtils.la
endif

if UDP_OPT
  universalExec_LDADD += @top_builddir@/interfaces/UdpAdapter/libUdpAdapter.la
endif

endif # STATIC_LIB_OPT
