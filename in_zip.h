/*
 * Copyright (C) 2025 Xiaomi Corporation
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BASE_IN_ZIP_H_
#define BASE_IN_ZIP_H_

#include <stdint.h>
#include <map>
#include <memory>
#include <string>
#include "ash/scoped_fd.h"

#define MIN_ECDR_SIZE 22
#define MAX_ECDR_SIZE 65536
namespace ferry {

class InZip {
 public:
  enum class CompressionMethod {
    kStore = 0,
    kDeflate = 8,
  };

  class Entry {
   public:
    Entry(uint32_t offset,
          CompressionMethod compression_method,
          uint32_t uncompressed_size,
          uint32_t compressed_size);

    uint32_t offset() const { return offset_; }
    CompressionMethod compression_method() const { return compression_method_; }
    uint32_t uncompressed_size() const { return uncompressed_size_; }
    uint32_t compressed_size() const { return compressed_size_; }

   private:
    uint32_t offset_;
    CompressionMethod compression_method_;
    uint32_t uncompressed_size_;
    uint32_t compressed_size_;
  };

  static std::unique_ptr<InZip> Open(const std::string& path);
  static std::unique_ptr<InZip> Open(ScopedFD fd);

  size_t GetEntryCount();
  const Entry* GetEntry(const std::string& path);
  const std::map<std::string, Entry>& GetEntries();
  std::unique_ptr<uint8_t[]> LoadEntry(const Entry* entry,
                                       size_t extra_size = 0);
  std::unique_ptr<uint8_t[]> LoadEntry(const std::string& path,
                                       size_t extra_size = 0);
  bool ExtractEntry(const std::string& path, const std::string& dest);

 private:
  InZip(ScopedFD fd, std::map<std::string, Entry> entries);

  ScopedFD fd_;
  std::map<std::string, Entry> entries_;
};

}  // namespace ferry

#endif  // BASE_IN_ZIP_H_