# Copyright (c) 2006-2011, Universities Space Research Association (USRA).
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
#

# Android makefile for PlexilAppFramework module

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := UniversalExec
LOCAL_SRC_FILES := UniversalExec.cc
LOCAL_CPP_EXTENSION := .cc
LOCAL_CPPFLAGS := -D'LIB_EXT="$(SUFSHARE)"'
LOCAL_EXPORT_CPPFLAGS := $(LOCAL_CPPFLAGS)
LOCAL_C_INCLUDES := $(LOCAL_PATH)/../../third-party/pugixml/src \
 $(LOCAL_PATH)/../utils \
 $(LOCAL_PATH)/../exec \
 $(LOCAL_PATH)/../app-framework \
 $(LOCAL_PATH)/../interfaces/LuvListener \
 $(LOCAL_PATH)/../interfaces/PlanDebugListener \
 $(LOCAL_PATH)/../interfaces/Sockets
LOCAL_STATIC_LIBRARIES := stdc++ pugixml PlexilAppFramework PlanDebugListener PlexilExec PlexilUtils LuvListener PlexilSockets
LOCAL_LDLIBS := -ldl

#include $(BUILD_STATIC_LIBRARY)
include $(BUILD_EXECUTABLE)
