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
if (CONFIG_FRAMEWORKS_RUNTIMES_BASE)
  set(BASE_DIR ${CMAKE_CURRENT_LIST_DIR})

  file(GLOB_RECURSE BASE_APPEND_SRCS ${BASE_DIR}/**/*.cpp)
  list(APPEND BASE_SRCS ${BASE_APPEND_SRCS})
  file(GLOB BASE_APPEND_SRCS ${BASE_DIR}/*.cpp)
  list(APPEND BASE_SRCS ${BASE_APPEND_SRCS})
  list(REMOVE_ITEM BASE_SRCS
        ${BASE_DIR}/device_info.cpp
        ${BASE_DIR}/file.cpp
        ${BASE_DIR}/in_zip.cpp
        ${BASE_DIR}/out_zip.cpp
        ${BASE_DIR}/scoped_fd.cpp
        ${BASE_DIR}/zip_file.cpp)

  nuttx_add_library(runtimes_base)
  target_sources(runtimes_base PRIVATE ${BASE_SRCS})
  target_include_directories(
    runtimes_base
    PRIVATE
    ${NUTTX_APPS_DIR}/frameworks/runtimes
    ${NUTTX_APPS_DIR}/system/zlib/zlib
    ${NUTTX_APPS_DIR}/system/zlib/zlib/contrib)
endif()