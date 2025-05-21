#include "ash/memory/thread_local.h"

#if ASH_THREAD_LOCAL_USE_VARIABLE_SEGMENT

#include <pthread.h>
#include <mutex>
#include "ash/crash/crash.h"
#include "ash/memory/lazy_instance.h"

namespace ash {

namespace {

LazyInstance<VariableSegmentDefination> segment_defination =
    ASH_LAZY_INSTANCE_INITIALIZER;
LazyInstance<std::mutex> mutex = ASH_LAZY_INSTANCE_INITIALIZER;
pthread_key_t key = -1;

}  // namespace

VariableSegmentDefination* GetThreadLocalSegmentDefination() {
  return segment_defination.Pointer();
}

uint8_t* GetThreadLocalSegment() {
  {
    std::lock_guard<std::mutex> lock(mutex.Get());
    if (key == -1) {
      int r = pthread_key_create(&key, [](void* segment) {
        GetThreadLocalSegmentDefination()->DestroySegment(
            static_cast<uint8_t*>(segment));
      });
      if (r < 0) {
        CrashImmediately();
      }
    }
  }

  uint8_t* segment = static_cast<uint8_t*>(pthread_getspecific(key));
  if (segment == nullptr) {
    segment = GetThreadLocalSegmentDefination()->CreateSegment();
    if (pthread_setspecific(key, segment) < 0) {
      CrashImmediately();
    }
  }
  return segment;
}

}  // namespace ash

#endif  // ASH_THREAD_LOCAL_USE_VARIABLE_SEGMENT
