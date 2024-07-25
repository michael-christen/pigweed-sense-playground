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

#include "modules/blinky/blinky.h"

#include "modules/led/monochrome_led_fake.h"
#include "modules/led/polychrome_led_fake.h"
#include "modules/worker/test_worker.h"
#include "pw_thread/sleep.h"
#include "pw_unit_test/framework.h"

namespace sense {

// Test fixtures.

class BlinkyTest : public ::testing::Test {
 protected:
  using Event = ::sense::MonochromeLedFake::Event;
  using State = ::sense::MonochromeLedFake::State;

  static constexpr uint32_t kIntervalMs = 10;
  static constexpr pw::chrono::SystemClock::duration kInterval =
      pw::chrono::SystemClock::for_at_least(
          std::chrono::milliseconds(kIntervalMs));

  // TODO(b/352327457): Ideally this would use simulated time, but no
  // simulated system timer exists yet. For now, relax the constraint by
  // checking that the LED was in the right state for _at least_ the expected
  // number of intervals. On some platforms, the fake LED is implemented using
  // threads, and may sleep a bit longer.
  BlinkyTest() : clock_(pw::chrono::VirtualSystemClock::RealClock()) {}

  pw::InlineDeque<Event>::iterator FirstActive() {
    pw::InlineDeque<Event>& events = monochrome_led_.events();
    pw::InlineDeque<Event>::iterator event = events.begin();
    while (event != events.end()) {
      if (event->state == State::kActive) {
        break;
      }
      ++event;
    }
    return event;
  }

  uint32_t ToMs(pw::chrono::SystemClock::duration interval) {
    return std::chrono::duration_cast<std::chrono::milliseconds>(interval)
        .count();
  }

  pw::chrono::VirtualSystemClock& clock_;
  MonochromeLedFake monochrome_led_;
  PolychromeLedFake polychrome_led_;
};

// Unit tests.

TEST_F(BlinkyTest, Toggle) {
  TestWorker<> worker;
  Blinky blinky;
  blinky.Init(worker, monochrome_led_, polychrome_led_);

  auto start = clock_.now();
  blinky.Toggle();
  pw::this_thread::sleep_for(kInterval * 1);
  blinky.Toggle();
  pw::this_thread::sleep_for(kInterval * 2);
  blinky.Toggle();
  pw::this_thread::sleep_for(kInterval * 3);
  blinky.Toggle();
  worker.Stop();

  auto event = FirstActive();
  ASSERT_NE(event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kActive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 0);
  start = event->timestamp;

  ASSERT_NE(++event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kInactive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 1);
  start = event->timestamp;

  ASSERT_NE(++event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kActive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 2);
  start = event->timestamp;

  ASSERT_NE(++event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kInactive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 3);
}

TEST_F(BlinkyTest, Blink) {
  TestWorker worker;
  Blinky blinky;
  blinky.Init(worker, monochrome_led_, polychrome_led_);

  auto start = clock_.now();
  EXPECT_EQ(blinky.Blink(1, kIntervalMs), pw::OkStatus());
  while (!blinky.IsIdle()) {
    pw::this_thread::sleep_for(kInterval);
  }
  worker.Stop();

  auto event = FirstActive();
  ASSERT_NE(event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kActive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs);
  start = event->timestamp;

  ASSERT_NE(++event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kInactive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs);
}

TEST_F(BlinkyTest, BlinkMany) {
  TestWorker<> worker;
  Blinky blinky;
  blinky.Init(worker, monochrome_led_, polychrome_led_);

  auto start = clock_.now();
  EXPECT_EQ(blinky.Blink(100, kIntervalMs), pw::OkStatus());
  while (!blinky.IsIdle()) {
    pw::this_thread::sleep_for(kInterval);
  }
  worker.Stop();

  // Every "on" and "off" is recorded.
  EXPECT_GE(monochrome_led_.events().size(), 200);
  EXPECT_GE(ToMs(clock_.now() - start), kIntervalMs * 200);
}

TEST_F(BlinkyTest, BlinkSlow) {
  TestWorker<> worker;
  Blinky blinky;
  blinky.Init(worker, monochrome_led_, polychrome_led_);

  auto start = clock_.now();
  EXPECT_EQ(blinky.Blink(1, kIntervalMs * 32), pw::OkStatus());
  while (!blinky.IsIdle()) {
    pw::this_thread::sleep_for(kInterval);
  }
  worker.Stop();

  auto event = FirstActive();
  ASSERT_NE(event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kActive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 32);
  start = event->timestamp;

  ASSERT_NE(++event, monochrome_led_.events().end());
  EXPECT_EQ(event->state, State::kInactive);
  EXPECT_GE(ToMs(event->timestamp - start), kIntervalMs * 32);
}

}  // namespace sense
