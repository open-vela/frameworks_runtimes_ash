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
#include "ash/message_loop/message_queue.h"

#include "ash/trace_event/trace_event.h"

namespace ash {

MessageQueue::MessageQueue()
    : pump_(nullptr), next_schedule_time_(Time::Forever()) {}

MessageQueue::~MessageQueue() = default;

void MessageQueue::PostTask(std::function<void()> task) {
  PostDelayedTask(std::move(task), Duration());
}

void MessageQueue::PostDelayedTask(std::function<void()> task, Duration delay) {
  Time time = Time::Now() + delay;
  {
    std::unique_lock<std::mutex> lock(mutex_);
    tasks_.push({time, std::move(task)});
    if (tasks_.top().time < time)
      return;
  }
  ScheduleWork();
}

void MessageQueue::Startup(MessagePump* pump) {
  {
    std::unique_lock<std::mutex> lock(mutex_);
    pump_ = pump;
  }
  ScheduleWork();
}

void MessageQueue::Shutdown() {
  //TODO(xuyan): 考虑unique_lock和lock_guard的选择
  std::unique_lock<std::mutex> lock(mutex_);
  pump_ = nullptr;
}

void MessageQueue::ScheduleWork() {
  std::unique_lock<std::mutex> lock(mutex_);
  if (!pump_ || tasks_.empty())
    return;
  pump_->Schedule();
}

Duration MessageQueue::Drive() {
  SCOPED_TRACE_EVENT("MessageQueue::Drive");
  Time now = Time::Now();
  std::unique_lock<std::mutex> lock(mutex_);
  next_schedule_time_ = Time::Forever();
  while (!tasks_.empty() && tasks_.top().time <= now) {
    std::function<void()> task = std::move(tasks_.top().task);
    tasks_.pop();
    lock.unlock();
    {
      SCOPED_TRACE_EVENT("MessageQueue::RunTask");
      task();
    }
    lock.lock();
  };
  return tasks_.empty() ? Duration::Infinity() : tasks_.top().time - now;
}

bool MessageQueue::TaskComparator::operator()(const Task& lhs,
                                              const Task& rhs) const {
  return lhs.time > rhs.time;
}

}  // namespace ash
