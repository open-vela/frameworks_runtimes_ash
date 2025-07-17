#include <gtest/gtest.h>
#include "device_info/device_info_test.cpp"
#include "fds/scoped_fd_test.cpp"
#include "file/file_path_test.cpp"
#include "file/file_test.cpp"
#include "functional/closure_test.cpp"
#include "functional/fn_once_test.cpp"
#include "memory/raw_ptr_test.cpp"
#include "memory/variable_segment_test.cpp"
#include "strings/number_string_conversions_test.cpp"
#include "strings/string_converter_test.cpp"
#include "time/duration_test.cpp"
#include "time/time_test.cpp"

extern "C" int main(int argc, char** argv) {
  ::testing::GTEST_FLAG(filter) =
      "DeviceInfo*:ScopedFD*:File*:FilePath*:Duration*:Time*:RawPtr*:"
      "NumberStringConversion*:StringConverter*:VariableSegment*:FnOnce*:"
      "*Closure*";
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}