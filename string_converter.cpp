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
#ifndef BASE_STRING_CONVERTER_H_
#define BASE_STRING_CONVERTER_H_

#include "base/string_converter.h"

namespace ferry {

bool LiteralProcessor::Build(std::string& output, const char* literal) {
  output.append(literal);
  return true;
}

bool LiteralProcessor::Parse(const char** input, const char* literal) {
  const char* p = *input;
  while (*p && *literal && *p == *literal) {
    p++;
    literal++;
  }
  if (*literal == '\0') {
    *input = p;
    return true;
  }
  return false;
}

size_t SkipSpaceProcessor<true>::Skip(const char** input) {
  const char* p = *input;
  while (*p == ' ' || *p == '\t' || *p == '\n' || *p == '\r') {
    p++;
  }
  size_t len = p - *input;
  *input = p;
  return len;
}

size_t SkipSpaceProcessor<false>::Skip(const char** input) {
  const char* p = *input;
  while (*p == ' ') {
    p++;
  }
  size_t len = p - *input;
  *input = p;
  return len;
}

bool IdentifierProcessor::Build(std::string& output, const std::string& value) {
  output.append(value);
  return true;
}

bool IdentifierProcessor::Parse(const char** input, std::string* value) {
  const char* p = *input;
  char c = *p;
  if (!((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') || c == '_'))
    return false;
  ++p;

  for (c = *p; (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
               (c >= '0' && c <= '9') || c == '_';
       c = *p++)
    ;
  --p;

  *value = std::string(*input, p - *input);
  *input = p;
  return true;
}

bool StringProcessor<bool>::Build(std::string& output, bool value) {
  return LiteralProcessor::Build(output, value ? "true" : "false");
}

bool StringProcessor<bool>::Parse(const char** input, bool* value) {
  if (LiteralProcessor::Parse(input, "true")) {
    *value = true;
    return true;
  }

  if (LiteralProcessor::Parse(input, "false")) {
    *value = false;
    return true;
  }

  return false;
}

bool StringProcessor<long>::Build(std::string& output, long value) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%ld", value);
  output.append(buffer);
  return true;
}

bool StringProcessor<long>::Parse(const char** input, long* value) {
  const char* p = *input;
  long result = 0;
  long flag = 1;

  if (*p == '+') {
    p++;
  } else if (*p == '-') {
    p++;
    flag = -1;
  }

  if (*p < '0' || *p > '9')
    return false;

  while (*p >= '0' && *p <= '9') {
    result = result * 10 + (*p - '0');
    p++;
  }

  *value = result * flag;
  *input = p;
  return true;
}

bool StringProcessor<unsigned long>::Build(std::string& output,
                                           unsigned long value) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lu", value);
  output.append(buffer);
  return true;
}

bool StringProcessor<unsigned long>::Parse(const char** input,
                                           unsigned long* value) {
  const char* p = *input;
  unsigned long result = 0;

  if (*p < '0' || *p > '9')
    return false;

  while (*p >= '0' && *p <= '9') {
    result = result * 10 + (*p - '0');
    p++;
  }

  *value = result;
  *input = p;
  return true;
}

bool StringProcessor<float>::Build(std::string& output, float value) {
  return StringProcessor<double>::Build(output, value);
}

bool StringProcessor<float>::Parse(const char** input, float* value) {
  double v;
  if (!StringProcessor<double>::Parse(input, &v))
    return false;
  *value = v;
  return true;
}

bool StringProcessor<double>::Build(std::string& output, double value) {
  char buffer[32];
  snprintf(buffer, sizeof(buffer), "%lf", value);
  output.append(buffer);
  return true;
}

bool StringProcessor<double>::Parse(const char** input, double* value) {
  const char* p = *input;
  unsigned long integer = 0;
  double flag = 1.0;
  if (*p == '+') {
    p++;
  } else if (*p == '-') {
    p++;
    flag = -1.0;
  }

  bool r = StringProcessor<unsigned long>::Parse(&p, &integer);
  if (*p++ != '.') {
    if (r) {
      *value = integer;
      *input = p - 1;
      return true;
    }
    return false;
  }

  if (!r && (*p < '0' || *p > '9'))
    return false;

  double fraction = 0;
  double factor = 0.1;
  while (*p >= '0' && *p <= '9') {
    fraction += factor * (*p - '0');
    factor *= 0.1;
    p++;
  }
  *value = (integer + fraction) * flag;
  *input = p;
  return true;
}

}  // namespace ferry

#endif  // BASE_STRING_CONVERTER_H_
