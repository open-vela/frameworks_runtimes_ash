#include <gtest/gtest.h>
#include "device_info/device_info_test.cpp"
#include "fds/scoped_fd_test.cpp"
#include "file/file_path_test.cpp"

extern "C" int main(int argc, char** argv) {
  ::testing::GTEST_FLAG(filter) = "DeviceInfo*:ScopedFD*:FilePath*";
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}