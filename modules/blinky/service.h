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
#include "modules/blinky/blinky_pb/blinky.rpc.pb.h"
#include "pw_system/rpc_server.h"

namespace am {

class BlinkyService final
    : public ::blinky::pw_rpc::nanopb::Blinky::Service<BlinkyService> {
 public:
  constexpr explicit BlinkyService(Blinky& blinky) : blinky_(blinky) {}

  pw::Status ToggleLed(const pw_protobuf_Empty&, pw_protobuf_Empty&);

  pw::Status Blink(const blinky_BlinkRequest& request, pw_protobuf_Empty&);

 private:
  Blinky& blinky_;
};

}  // namespace am