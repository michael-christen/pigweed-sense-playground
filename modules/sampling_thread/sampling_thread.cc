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

#include "modules/sampling_thread/sampling_thread.h"

#include <chrono>

#include "pw_assert/check.h"
#include "pw_chrono/system_clock.h"
#include "pw_log/log.h"
#include "pw_thread/sleep.h"
#include "system/pubsub.h"
#include "system/system.h"

namespace am {
namespace {

using pw::chrono::SystemClock;

constexpr SystemClock::duration kPeriod = std::chrono::milliseconds(100);

void ReadProximity() {
  pw::Result<uint16_t> sample = system::ProximitySensor().ReadSample();
  if (!sample.ok()) {
    PW_LOG_WARN("Failed to read proximity sensor sample: %s",
                sample.status().str());
    return;
  }
  system::PubSub().Publish(ProximitySample{*sample});
}

}  // namespace

// Reads sensor samples in a loop and publishes PubSub events for them.
void SamplingLoop() {
  PW_CHECK_OK(system::ProximitySensor().Enable());

  SystemClock::time_point deadline = SystemClock::now();

  while (true) {
    deadline += kPeriod;
    pw::this_thread::sleep_until(deadline);

    ReadProximity();
  }
}

}  // namespace am