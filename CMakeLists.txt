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

# If called on nuttx platform at the preapps stage, do nothing.
# The equality of CMAKE_CURRENT_BINARY_DIR and CMAKE_BINARY_DIR implies that
# 'ash' is not the root project. The absence of CMAKE_PROJECT_NAME indicates
# that the root project has not declared project. So far, only nuttx has such
# behaviours.
if (NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "${CMAKE_BINARY_DIR}"
    AND "${CMAKE_PROJECT_NAME}" STREQUAL "")
  return()
endif()

# Enable ash on nuttx only when CONFIG_LIBASH is enabled.
# TODO(xuyan15): Using the root project name to identify the platform is urgly.
# See blow.
if ("${CMAKE_PROJECT_NAME}" STREQUAL NuttX AND NOT CONFIG_LIBASH)
  return()
endif()

cmake_minimum_required(VERSION 3.16)

project(ash)
set(CMAKE_C_STANDARD 11) #C11
set(CMAKE_CXX_STANDARD 17) #C17
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(ash)

target_sources(ash
  PRIVATE
  src/crash/crash.cpp
  src/device_info/device_info.cpp
  src/fds/scoped_fd.cpp
  src/file/file.cpp
  src/file/file_path.cpp
  src/logging/logging.cpp
  src/memory/lifecycle_watcher.cpp
  src/message_loop/message_loop.cpp
  src/message_loop/message_pump_android.cpp
  src/message_loop/message_pump.cpp
  src/message_loop/message_pump_impl.cpp
  src/message_loop/message_pump_uv.cpp
  src/message_loop/message_queue.cpp
  src/stream/input_stream.cpp
  src/strings/number_string_conversions.cpp
  src/strings/string_converter.cpp
  src/threading/thread.cpp
  src/time/duration.cpp
  src/time/time.cpp
  src/trace_event/trace_event.cpp
  src/zip/in_zip.cpp
  src/zip/out_zip.cpp
  src/zip/zip_file.cpp
  PUBLIC
  include/ash/crash/crash.h
  include/ash/device_info/device_info.h
  include/ash/fds/scoped_fd.h
  include/ash/file/file.h
  include/ash/file/file_path.h
  include/ash/geometry/size.h
  include/ash/logging/logging.h
  include/ash/memory/lifecycle_watcher.h
  include/ash/memory/weak_ptr.h
  include/ash/message_loop/message_loop.h
  include/ash/message_loop/message_loop_scope.h
  include/ash/message_loop/message_pump_android.h
  include/ash/message_loop/message_pump.h
  include/ash/message_loop/message_pump_impl.h
  include/ash/message_loop/message_pump_uv.h
  include/ash/message_loop/message_queue.h
  include/ash/stream/input_stream.h
  include/ash/strings/number_string_conversions.h
  include/ash/strings/string_converter.h
  include/ash/task_runner/task_runner.h
  include/ash/threading/thread.h
  include/ash/time/duration.h
  include/ash/time/time.h
  include/ash/trace_event/trace_event.h
  include/ash/utils/scoped_setter.h
  include/ash/utils/value_or_function.h
  include/ash/zip/in_zip.h
  include/ash/zip/out_zip.h
  include/ash/zip/zip_file.h
)

target_include_directories(ash PUBLIC include)

# Build on nuttx
#
# TODO(xuyan15): Using the root project name to identify the platform is not
# an elegant practice. Use a global property instead once nuttx build system
# provide one.
if(${CMAKE_PROJECT_NAME} STREQUAL NuttX)
  target_include_directories(ash PRIVATE
    ${NUTTX_APPS_DIR}/system/zlib/zlib
    ${NUTTX_APPS_DIR}/system/zlib/zlib/contrib)

  nuttx_add_external_library(ash)
endif()

# TODO(xuyan15): Support building on other platforms, especially on linux.
