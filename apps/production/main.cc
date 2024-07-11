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

#define PW_LOG_MODULE_NAME "MAIN"

#include "modules/board/service.h"
#include "pw_log/log.h"
#include "pw_system/system.h"
#include "system/system.h"
#include "system/worker.h"

int main() {
  am::system::Init();

  static am::BoardService board_service;
  board_service.Init(am::system::GetWorker(), am::system::Board());
  pw::System().rpc_server().RegisterService(board_service);

  PW_LOG_INFO("Welcome to Airmaranth 🌿☁️");
  am::system::Start();
  PW_UNREACHABLE;
}