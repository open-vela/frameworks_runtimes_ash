#include <gtest/gtest.h>
#include "ash/device_info/device_info.h"
#include "device_info/device_info_test.cpp"
extern "C" int main(int argc, char** argv) {
  ::testing::GTEST_FLAG(filter) = "DeviceInfo*";
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}