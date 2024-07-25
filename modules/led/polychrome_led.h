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

#include <cstdint>

#include "modules/pwm/digital_out.h"

namespace sense {

/// Interface for a multi-color LED.
class PolychromeLed {
 public:
  static constexpr uint32_t kRedShift = 16;
  static constexpr uint32_t kGreenShift = 8;
  static constexpr uint32_t kBlueShift = 0;

  PolychromeLed(PwmDigitalOut& red, PwmDigitalOut& green, PwmDigitalOut& blue)
      : red_(red), green_(green), blue_(blue) {}
  ~PolychromeLed() = default;

  /// Turns off the LED.
  void TurnOff();

  // Turns the LED on.
  void TurnOn();

  /// Sets the brightness of the LED.
  ///
  /// @param  level   Relative brightness of the LED
  void SetBrightness(uint8_t level);

  /// Sets the RGB LED using individual red, green, and blue components.
  void SetColor(uint8_t red, uint8_t green, uint8_t blue);

  /// Sets the RGB LED using a 24-bit hex color code.
  void SetColor(uint32_t hex);

  /// Fades the LED on and off continuously.
  ///
  /// @param  interval_ms   The duration of a fade cycle, in milliseconds.
  void Pulse(uint32_t hex, uint32_t interval_ms);

  /// Cycles back and forth between two colors.
  void PulseBetween(uint32_t hex1, uint32_t hex2, uint32_t interval_ms);

  /// Cycles thorugh all the colors.
  void Rainbow(uint32_t interval_ms);

 private:
  /// Sets the levels of the red, green, and blue PWM slices.
  void Update();

  /// Adjusts the given 8-bit value using sRGB, and scales according to the
  /// current brightness.
  uint16_t GammaCorrect(uint32_t value) const;

  PwmDigitalOut& red_;
  PwmDigitalOut& green_;
  PwmDigitalOut& blue_;
  uint32_t hex_ = 0;
  uint32_t alternate_hex_ = 0;
  uint16_t brightness_ = 0;
};

}  // namespace sense
