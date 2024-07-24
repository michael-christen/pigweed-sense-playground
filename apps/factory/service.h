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

#include "apps/factory/factory_pb/factory.rpc.pb.h"
#include "modules/buttons/manager.h"

namespace am {

class FactoryService final
    : public ::factory::pw_rpc::nanopb::Factory::Service<FactoryService> {
 public:
  void Init(ButtonManager& button_manager);

  pw::Status StartTest(const factory_StartTestRequest& request,
                       pw_protobuf_Empty&);

  pw::Status EndTest(const factory_EndTestRequest& request, pw_protobuf_Empty&);

 private:
  ButtonManager* button_manager_;
};

}  // namespace am