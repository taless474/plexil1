# Makefile for Plexil documentation

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

SHELL = /bin/sh

# User may need to specify this on make command line if doxygen is not in $PATH
DOXYGEN ?= doxygen

# Doxygen configuration files
DOXY_FILES := Doxyfile PLEXIL_layout.xml PLEXIL_footer.html PLEXIL_header.html \
 PLEXIL_stylesheet.css

# Narrative files
DOC_SRCS := $(wildcard src/*.dox) $(wildcard src/*.md)

# Code files with Doxygen-format comments
CODE_SRC_DIRS := ../src/app-framework ../src/exec ../src/expr \
 ../src/interfaces/IpcUtils ../src/interfaces/IpcAdapter ../src/interfaces/UdpAdapter \
 ../src/intfc ../src/universal-exec ../src/utils ../src/value ../src/xml-parser
CODE_SRCS := $(foreach dir,$(CODE_SRC_DIRS),$(wildcard ../src/utils/*.[hc]) $(wildcard $(dir)/*.hh) $(wildcard $(dir)/*.cc))

all: html/index.html

html/index.html: $(DOXY_FILES) $(DOC_SRCS) $(CODE_SRCS)
	$(DOXYGEN) Doxyfile

clean:
	$(RM) -r html

.PHONY: clean
