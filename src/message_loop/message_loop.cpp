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
#include "ash/message_loop/message_loop.h"
#include "ash/logging/logging.h"
#include "ash/macros/compiler_macros.h"
#include "ash/message_loop/message_pump_android.h"
#include "ash/message_loop/message_pump_impl.h"
#include "ash/message_loop/message_pump_uv.h"

namespace ash {

namespace {
thread_local MessageLoop* tls = nullptr;
}

// TODO(xuyan): MessageLoop need to be refactored to ensure thread-safety.
MessageLoop::MessageLoop(std::unique_ptr<MessagePump> pump,
                         std::shared_ptr<MessageQueue> queue)
    : pump_(std::move(pump)), queue_(std::move(queue)) {
  pump_->queue_ = queue_.get();
  queue_->Startup(pump_.get());
}

MessageLoop::~MessageLoop() {
  queue_->Shutdown();
}

std::shared_ptr<TaskRunner> MessageLoop::GetTaskRunner() {
  return queue_;
}

MessageLoop* MessageLoop::Current() {
  return tls;
}

void MessageLoop::SetCurrent(MessageLoop* loop) {
  tls = loop;
}

void MessageLoop::ClearAndDestroyCurrentLoop() {
  MessageLoop* loop = tls;
  tls = nullptr;
  if (loop)
    delete loop;
}

void MessageLoop::Run() {
  pump_->Run();
}

void MessageLoop::Quit() {
  pump_->Quit();
}

void MessageLoop::WatchFD(int fd,
                          FDWatchCB on_can_read,
                          FDWatchCB on_can_write,
                          FDWatchCB on_error) {
  pump_->WatchFD(fd, std::move(on_can_read), std::move(on_can_write),
                 std::move(on_error));
}

void MessageLoop::UnwatchFD(int fd) {
  pump_->UnwatchFD(fd);
}

MessageLoop* MessageLoop::Create(std::shared_ptr<MessageQueue> queue) {
  std::unique_ptr<MessagePump> pump = std::make_unique<MessagePumpImpl>();
  if (!queue)
    queue = std::make_shared<MessageQueue>();
  return new MessageLoop(std::move(pump), std::move(queue));
}

#if defined(ASH_OS_NUTTX)
MessageLoop* MessageLoop::CreateForUV(uv_loop_t* uv_loop) {
  std::unique_ptr<MessagePump> pump = std::make_unique<MessagePumpUV>(uv_loop);
  std::shared_ptr<MessageQueue> queue = std::make_shared<MessageQueue>();
  return new MessageLoop(std::move(pump), std::move(queue));
}
#endif  // defined(ASH_OS_NUTTX)

#if defined(ASH_OS_ANDROID)
MessageLoop* MessageLoop::CreateForAndroid(ALooper* looper) {
  std::unique_ptr<MessagePump> pump =
      std::make_unique<MessagePumpAndroid>(looper);
  std::shared_ptr<MessageQueue> queue = std::make_shared<MessageQueue>();
  return new MessageLoop(std::move(pump), std::move(queue));
}
#endif  // defined(ASH_OS_ANDROID)

}  // namespace ash
