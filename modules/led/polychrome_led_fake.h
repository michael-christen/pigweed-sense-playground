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
#pragma once

#include "modules/led/polychrome_led.h"
#include "modules/pwm/digital_out.h"

namespace am {

/// Interface for a simple LED.
class PolychromeLedFake : public PolychromeLed {
 public:
  PolychromeLedFake() : PolychromeLed(red_, green_, blue_) {}

 private:
  PwmDigitalOutFake red_;
  PwmDigitalOutFake green_;
  PwmDigitalOutFake blue_;
};

}  // namespace am
