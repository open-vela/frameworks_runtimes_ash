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
#ifndef BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_
#define BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_

#include <memory>
#include "base/message_loop/message_pump.h"
#include "base/message_loop/message_queue.h"

#include <uv.h>

#if defined(__ANDROID__)
#include <android/looper.h>
#endif  // defined(__ANDROID__)

namespace ferry {

class MessageLoop {
 public:
  MessageLoop(std::unique_ptr<MessagePump> pump,
              std::shared_ptr<MessageQueue> queue);
  ~MessageLoop();
  MessageLoop(const MessageLoop&) = delete;
  MessageLoop& operator=(const MessageLoop&) = delete;

  std::shared_ptr<TaskRunner> GetTaskRunner();

  static MessageLoop* Current();
  static void SetCurrent(MessageLoop* loop);
  static void ClearAndDestroyCurrentLoop();

  void Run();
  void Quit();

  using FDWatchCB = MessagePump::FDWatchCB;
  void WatchFD(int fd,
               FDWatchCB on_can_read,
               FDWatchCB on_can_write,
               FDWatchCB on_error);
  void UnwatchFD(int fd);

  static MessageLoop* Create(std::shared_ptr<MessageQueue> queue = nullptr);
#if defined(__NuttX__)
  static MessageLoop* CreateForUV(uv_loop_t* uv_loop = nullptr);
#endif  // defined(__NuttX__)

#if defined(__ANDROID__)
  static MessageLoop* CreateForAndroid(ALooper* looper = nullptr);
#endif  // defined(__ANDROID__)

 private:
  std::unique_ptr<MessagePump> pump_;
  std::shared_ptr<MessageQueue> queue_;
};

}  // namespace ferry

#endif  // BASE_MESSAGE_LOOP_MESSAGE_LOOP_H_
