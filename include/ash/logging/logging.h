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
#ifndef ASH_LOGGING_H_
#define ASH_LOGGING_H_

#include <sstream>

#define LOG_STREAM(tag, level)                                             \
  ::ash::LogStream(tag, ::ash::LogLevel::k##level, __FILE__, __LINE__, \
                     __FUNCTION__)                                         \
      .stream()

#define LAZY_STREAM(tag, level, condition)                                \
  !(condition) ? (void)0                                                  \
               : ::ash::LogStreamVoidify() &                            \
                     ::ash::LogStream(tag, ::ash::LogLevel::k##level, \
                                        __FILE__, __LINE__, __FUNCTION__) \
                         .stream()

#define LOG_IF(tag, level, condition) LAZY_STREAM(tag, level, condition)

#define LOG(tag, level) LOG_IF(tag, level, true)

#define CHECK(condition) \
  LOG_IF("DEBUG", FATAL, !(condition)) << "check failed: " #condition " "
#define CHECK_EQ(val1, val2) CHECK((val1) == (val2))
#define CHECK_NE(val1, val2) CHECK((val1) != (val2))
#define CHECK_LT(val1, val2) CHECK((val1) < (val2))
#define CHECK_LE(val1, val2) CHECK((val1) <= (val2))
#define CHECK_GT(val1, val2) CHECK((val1) > (val2))
#define CHECK_GE(val1, val2) CHECK((val1) >= (val2))
#define UNREACHED() LOG(FATAL) << "UNREACHED"

#if defined(NDEBUG)
#define DLOG_IF(tag, level, condition) LOG_IF(tag, level, condition)
#define DLOG(tag, level) LOG(tag, level)
#define DCHECK(condition) CHECK(condition)
#define DCHECK_EQ(val1, val2) CHECK_EQ(val1, val2)
#define DCHECK_NE(val1, val2) CHECK_NE(val1, val2)
#define DCHECK_LT(val1, val2) CHECK_LT(val1, val2)
#define DCHECK_LE(val1, val2) CHECK_LE(val1, val2)
#define DCHECK_GT(val1, val2) CHECK_GT(val1, val2)
#define DCHECK_GE(val1, val2) CHECK_GE(val1, val2)
#else
#define DLOG_IF(tag, level, condition) LOG_IF(tag, level, false && (condition))
#define DLOG(tag, level) LOG_IF(tag, level, false)
#define DCHECK(condition) DLOG_IF("DEBUG", FATAL, !(condition))
#define DCHECK_EQ(val1, val2) DCHECK(val1 == val2)
#define DCHECK_NE(val1, val2) DCHECK(val1 != val2)
#define DCHECK_LT(val1, val2) DCHECK(val1 < val2)
#define DCHECK_LE(val1, val2) DCHECK(val1 <= val2)
#define DCHECK_GT(val1, val2) DCHECK(val1 > val2)
#define DCHECK_GE(val1, val2) DCHECK(val1 >= val2)
#endif

namespace ash {

enum class LogLevel {
  kVERBOSE,
  kDEBUG,
  kINFO,
  kWARNING,
  kERROR,
  kFATAL,
};

class LogStream {
 public:
  LogStream(const char* tag,
            LogLevel level,
            const char* file,
            int line,
            const char* function);
  ~LogStream();

  std::ostream& stream() { return stream_; }

 private:
  const char* tag_;
  LogLevel level_;
  const char* file_;
  int line_;
  const char* function_;
  std::ostringstream stream_;
};

class LogStreamVoidify {
 public:
  void operator&(std::ostream&) {}
};

}  // namespace ash

#endif  // ASH_LOGGING_H_