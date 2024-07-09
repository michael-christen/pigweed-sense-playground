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

#include "system/system.h"

#include "device/pico_led.h"
#include "modules/board/board.h"
#include "modules/led/monochrome_led.h"

namespace am::system {

am::Board& Board() {
  static ::am::Board board;
  return board;
}

am::MonochromeLed& MonochromeLed() {
  static ::am::PicoMonochromeLed monochrome_led;
  return monochrome_led;
}

}  // namespace am::system
