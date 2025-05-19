#include "ash/memory/global_variable.h"
#include "ash/macros/compiler_macros.h"
#include "ash/memory/lazy_instance.h"

#if defined(ASH_OS_NUTTX)

#include <nuttx/tls.h>
#include <mutex>
#include "ash/crash/crash.h"

namespace ash {

namespace {

LazyInstance<VariableSegmentDefination> segment_defination =
    ASH_LAZY_INSTANCE_INITIALIZER;
LazyInstance<std::mutex> mutex = ASH_LAZY_INSTANCE_INITIALIZER;
int key = -1;

}  // namespace

VariableSegmentDefination* GetGlobalVariableSegmentDefination() {
  return segment_defination.Pointer();
}

uint8_t* GetGlobalVariableSegment() {
  std::lock_guard<std::mutex> lock(mutex.Get());
  if (key == -1) {
    key = task_tls_alloc([](void* segment) {
      GetGlobalVariableSegmentDefination()->DestroySegment(
          static_cast<uint8_t*>(segment));
    });
    if (key < 0) {
      CrashImmediately();
    }
  }

  uint8_t* segment = reinterpret_cast<uint8_t*>(task_tls_get_value(key));
  if (segment == nullptr) {
    segment = GetGlobalVariableSegmentDefination()->CreateSegment();
    if (task_tls_set_value(key, reinterpret_cast<uintptr_t>(segment)) != 0) {
      CrashImmediately();
    }
  }

  return segment;
}

}  // namespace ash

#endif  // defined(ASH_OS_NUTTX)