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
#include "base/threading/thread.h"

#include "base/logging.h"
#include "base/message_loop/message_loop.h"
#include "base/message_loop/message_loop_scope.h"

namespace ferry {

Thread::Thread() : message_queue_(std::make_shared<MessageQueue>()) {
  pthread_t th;
  pthread_attr_t attr;
  pthread_attr_init(&attr);
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
#if defined(__NuttX__)
  pthread_attr_setstacksize(&attr, CONFIG_QUICKAPP_THREADSTACKSIZE);
  attr.priority = CONFIG_QUICKAPP_PRIORITY;
#endif  // defined(__NuttX__)

  int r = pthread_create(&th, &attr, &Thread::Run,
                         new std::shared_ptr<MessageQueue>(message_queue_));
  CHECK(r == 0) << "Failed to create thread";
}

Thread::~Thread() = default;

std::shared_ptr<TaskRunner> Thread::GetTaskRunner() {
  return message_queue_;
}

void Thread::Quit() {
  message_queue_->PostTask([]() { MessageLoop::Current()->Quit(); });
}

void* Thread::Run(void* arg) {
  std::shared_ptr<MessageQueue>* queue =
      reinterpret_cast<std::shared_ptr<MessageQueue>*>(arg);
  std::unique_ptr<MessageLoop> message_loop(MessageLoop::Create(*queue));
  delete queue;
  MessageLoopScope message_loop_scope(message_loop.get());
  message_loop->Run();
  return nullptr;
}

}  // namespace ferry
