
// Copyright 2024 The Pigweed Authors
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.

#include "modules/indicators/system_led_testing.h"

namespace am {

void SystemLedForTest::Set(bool) {
  // Skip the first "TurnOff" that occurs as part of initialization.
  if (IsOn() || !output_.empty()) {
    size_t num_intervals = (pw::chrono::SystemClock::now() - last_) / interval_;
    output_.push_back(Encode(IsOn(), num_intervals));
  }
  last_ = pw::chrono::SystemClock::now();
}

uint8_t SystemLedForTest::Encode(bool is_on, size_t num_intervals) {
  size_t encoded = num_intervals;
  encoded = encoded > 0x7F ? 0x7F : encoded;
  encoded |= is_on ? 0x80 : 0x00;
  return static_cast<uint8_t>(encoded);
}

}  // namespace am