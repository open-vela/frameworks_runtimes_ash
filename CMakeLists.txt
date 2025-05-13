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
if (CONFIG_LIB_ASH)
  set(ASH_DIR ${CMAKE_CURRENT_LIST_DIR})

  file(GLOB_RECURSE ASH_APPEND_SRCS ${ASH_DIR}/**/*.cpp)
  list(APPEND ASH_SRCS ${ASH_APPEND_SRCS})
  file(GLOB ASH_APPEND_SRCS ${ASH_DIR}/*.cpp)
  list(APPEND ASH_SRCS ${ASH_APPEND_SRCS})
  list(REMOVE_ITEM ASH_SRCS
        ${ASH_DIR}/device_info.cpp
        ${ASH_DIR}/file.cpp
        ${ASH_DIR}/in_zip.cpp
        ${ASH_DIR}/out_zip.cpp
        ${ASH_DIR}/scoped_fd.cpp
        ${ASH_DIR}/zip_file.cpp)

  nuttx_add_library(ash)
  target_sources(ash PRIVATE ${ASH_SRCS})
  target_include_directories(
    ash
    PRIVATE
    ${NUTTX_APPS_DIR}/frameworks/runtimes/ash/include
    ${NUTTX_APPS_DIR}/system/zlib/zlib
    ${NUTTX_APPS_DIR}/system/zlib/zlib/contrib)
endif()