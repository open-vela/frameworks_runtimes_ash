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
#include "ash/message_loop/message_pump_uv.h"

#if defined(__NuttX__)

#include "ash/logging/logging.h"

namespace ash {

class MessagePumpUV::FDWatcher {
 public:
  using FDWatchCB = MessagePump::FDWatchCB;

  FDWatcher(int fd,
            FDWatchCB on_can_read,
            FDWatchCB on_can_write,
            FDWatchCB on_error,
            uv_loop_t* loop);

  ~FDWatcher() = default;

  void Destroy();

 private:
  static void UVPollCB(uv_poll_t* handle, int status, int events);

  uv_poll_t poll_;
  int fd_;
  FDWatchCB on_can_read_;
  FDWatchCB on_can_write_;
  FDWatchCB on_error_;
};

MessagePumpUV::MessagePumpUV(uv_loop_t* uv_loop) : loop_(uv_loop) {
  async_ = (uv_async_t*)calloc(1, sizeof(uv_async_t));
  uv_async_init(loop_, async_, &MessagePumpUV::AsyncCB);
  uv_handle_set_data((uv_handle_t*)async_, this);

  timer_ = (uv_timer_t*)calloc(1, sizeof(uv_timer_t));
  uv_timer_init(loop_, timer_);
  uv_handle_set_data((uv_handle_t*)timer_, this);
}

MessagePumpUV::~MessagePumpUV() {
  uv_timer_stop(timer_);
  uv_close((uv_handle_t*)timer_,
           [](uv_handle_t* handle) { free((uv_timer_t*)handle); });
  uv_close((uv_handle_t*)async_,
           [](uv_handle_t* handle) { free((uv_async_t*)handle); });
}

void MessagePumpUV::Schedule() {
  uv_async_send(async_);
}

void MessagePumpUV::Run() {}

void MessagePumpUV::Quit() {}

void MessagePumpUV::WatchFD(int fd,
                            FDWatchCB on_can_read,
                            FDWatchCB on_can_write,
                            FDWatchCB on_error) {
  // TODO(xuyan): 检查当前线程是否是消息循环线程

  ASH_DCHECK(watchers_.find(fd) == watchers_.end());

  watchers_.emplace(
      fd, new FDWatcher(fd, std::move(on_can_read), std::move(on_can_write),
                        std::move(on_error), loop_));
}

void MessagePumpUV::UnwatchFD(int fd) {
  // TODO(xuyan): 检查当前线程是否是消息循环线程
  auto it = watchers_.find(fd);
  ASH_DCHECK(it != watchers_.end());
  it->second->Destroy();
  watchers_.erase(it);
}

void MessagePumpUV::AsyncCB(uv_async_t* handle) {
  MessagePumpUV* pump =
      static_cast<MessagePumpUV*>(uv_handle_get_data((uv_handle_t*)handle));
  pump->RunCB();
}

void MessagePumpUV::TimerCB(uv_timer_t* timer) {
  MessagePumpUV* pump =
      static_cast<MessagePumpUV*>(uv_handle_get_data((uv_handle_t*)timer));
  pump->RunCB();
}

void MessagePumpUV::RunCB() {
  Duration delay = Drive();
  uv_timer_stop(timer_);
  if (delay == Duration::Infinity())
    return;
  uv_timer_start(timer_, &MessagePumpUV::TimerCB, delay.ToMilli(), 0);
}

MessagePumpUV::FDWatcher::FDWatcher(int fd,
                                    FDWatchCB on_can_read,
                                    FDWatchCB on_can_write,
                                    FDWatchCB on_error,
                                    uv_loop_t* loop)
    : fd_(fd),
      on_can_read_(std::move(on_can_read)),
      on_can_write_(std::move(on_can_write)),
      on_error_(std::move(on_error)) {
  int event = 0;
  if (on_can_read_)
    event |= UV_READABLE;
  if (on_can_write_)
    event |= UV_WRITABLE;
  if (on_error_)
    event |= UV_DISCONNECT;

  uv_poll_init(loop, &poll_, fd);
  uv_handle_set_data((uv_handle_t*)&poll_, this);
  ASH_CHECK_EQ(uv_poll_start(&poll_, event, &MessagePumpUV::FDWatcher::UVPollCB),
           0);
}

void MessagePumpUV::FDWatcher::Destroy() {
  ASH_CHECK_EQ(uv_poll_stop(&poll_), 0);

  uv_close((uv_handle_t*)&poll_, [](uv_handle_t* handle) {
    delete reinterpret_cast<FDWatcher*>(uv_handle_get_data(handle));
  });
}

void MessagePumpUV::FDWatcher::UVPollCB(uv_poll_t* handle,
                                        int status,
                                        int events) {
  FDWatcher* watcher =
      reinterpret_cast<FDWatcher*>(uv_handle_get_data((uv_handle_t*)handle));
  int fd = watcher->fd_;
  if (status != 0) {
    if (watcher->on_error_)
      watcher->on_error_(fd);
    return;
  }

  if ((events & UV_READABLE) && watcher->on_can_read_) {
    watcher->on_can_read_(fd);
  }
  if ((events & UV_WRITABLE) && watcher->on_can_write_) {
    watcher->on_can_write_(fd);
  }
  if ((events & UV_DISCONNECT) && watcher->on_error_) {
    watcher->on_error_(fd);
  }
}

}  // namespace ash

#endif  // defined(__NuttX__)
