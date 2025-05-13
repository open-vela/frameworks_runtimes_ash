#
# Copyright (C) 2025 Xiaomi Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

include $(APPDIR)/Make.defs

CXXEXT     := .cpp
CXXFLAGS   += -std=c++17

# workaround for gcc-13 warning
GCC_VERSION := $(shell gcc -dumpversion)
ifeq ($(shell expr $(GCC_VERSION) \>= 13), 1)
  CFLAGS += --param=min-pagesize=0
  CXXFLAGS += --param=min-pagesize=0
endif

ifeq ($(CONFIG_LIB_ASH),y)
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/frameworks/runtimes/ash/include
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/system/zlib/zlib
CXXFLAGS += ${INCDIR_PREFIX}$(APPDIR)/system/zlib/zlib/contrib

CXXSRCS += $(filter-out \
        $(APPDIR)/frameworks/runtimes/ash/file.cpp \
        $(APPDIR)/frameworks/runtimes/ash/zip_file.cpp \
        $(APPDIR)/frameworks/runtimes/ash/in_zip.cpp \
        $(APPDIR)/frameworks/runtimes/ash/out_zip.cpp \
        $(APPDIR)/frameworks/runtimes/ash/scoped_fd.cpp \
        $(APPDIR)/frameworks/runtimes/ash/device_info.cpp, \
        $(wildcard $(APPDIR)/frameworks/runtimes/ash/*.cpp))
CXXSRCS += $(wildcard $(APPDIR)/frameworks/runtimes/ash/message_loop/*.cpp)
CXXSRCS += $(wildcard $(APPDIR)/frameworks/runtimes/ash/threading/*.cpp)
CXXSRCS += $(wildcard $(APPDIR)/frameworks/runtimes/ash/time/*.cpp)
CXXSRCS += $(wildcard $(APPDIR)/frameworks/runtimes/ash/trace_event/*.cpp)
endif

include $(APPDIR)/Application.mk