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
#include "ash/trace_event/trace_event.h"

#if defined(__NuttX__)
#include <nuttx/sched_note.h>
#endif  // defined(__NuttX__)

namespace ash {

void TraceEvent::Begin(const char* name) {
#if defined(__NuttX__)
  sched_note_beginex(NOTE_TAG_ALWAYS, name);
#endif  // defined(__NuttX__)
}

void TraceEvent::End(const char* name) {
#if defined(__NuttX__)
  sched_note_endex(NOTE_TAG_ALWAYS, name);
#endif  // defined(__NuttX__)
}

ScopedTraceEvent::ScopedTraceEvent(const char* name) : name_(name) {
  TraceEvent::Begin(name_);
}

ScopedTraceEvent::~ScopedTraceEvent() {
  TraceEvent::End(name_);
}

}  // namespace ash
