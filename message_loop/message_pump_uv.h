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
#ifndef BASE_MESSAGE_LOOP_MESSAGE_PUMP_UV_H_
#define BASE_MESSAGE_LOOP_MESSAGE_PUMP_UV_H_

#if defined(__NuttX__)

#include "base/message_loop/message_pump.h"

#include <uv.h>
#include <map>

namespace ferry {

class MessagePumpUV : public MessagePump {
 public:
  MessagePumpUV(uv_loop_t* uv_loop);
  ~MessagePumpUV() override;

  void Schedule() override;

  void Run() override;
  void Quit() override;

  void WatchFD(int fd,
               FDWatchCB on_can_read,
               FDWatchCB on_can_write,
               FDWatchCB on_error) override;

  void UnwatchFD(int fd) override;

 private:
  class FDWatcher;

  static void AsyncCB(uv_async_t* async);
  static void TimerCB(uv_timer_t* timer);

  void RunCB();

  uv_loop_t* loop_;
  uv_async_t* async_;
  uv_timer_t* timer_;
  std::map<int, FDWatcher*> watchers_;
};

}  // namespace ferry

#endif  // defined(__NuttX__)

#endif  // BASE_MESSAGE_LOOP_MESSAGE_PUMP_UV_H_
