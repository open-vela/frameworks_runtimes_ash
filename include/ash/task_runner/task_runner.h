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
#ifndef ASH_TASK_RUNNER_TASK_RUNNER_H_
#define ASH_TASK_RUNNER_TASK_RUNNER_H_

#include <functional>
#include "ash/memory/disallow_copy.h"
#include "ash/time/duration.h"

namespace ash {

class TaskRunner : public DisallowCopyAndMove {
 public:
  TaskRunner() = default;
  virtual ~TaskRunner() = default;

  virtual void PostTask(std::function<void()> task) = 0;
  virtual void PostDelayedTask(std::function<void()> task, Duration delay) = 0;
};

}  // namespace ash

#endif  // ASH_TASK_RUNNER_TASK_RUNNER_H_