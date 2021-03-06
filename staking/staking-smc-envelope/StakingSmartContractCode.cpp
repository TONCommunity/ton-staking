/*
    This file is part of TON Blockchain Library.

    TON Blockchain Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    TON Blockchain Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TON Blockchain Library.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2017-2019 Telegram Systems LLP
*/
#include "StakingSmartContractCode.h"

#include "vm/boc.h"
#include <map>

#include "td/utils/base64.h"

namespace ton {
namespace {
const auto& get_map() {
  static auto map = [] {
    std::map<std::string, td::Ref<vm::Cell>, std::less<>> map;
    auto with_tvm_code = [&](auto name, td::Slice code_str) {
      map[name] = vm::std_boc_deserialize(td::base64_decode(code_str).move_as_ok()).move_as_ok();
    };
#include "smartcont/auto/staking-pool-code.cpp"
#include "smartcont/auto/nominator-code.cpp"
    return map;
  }();
  return map;
}
}  // namespace

td::Result<td::Ref<vm::Cell>> StakingSmartContractCode::load(td::Slice name) {
  auto& map = get_map();
  auto it = map.find(name);
  if (it == map.end()) {
    return td::Status::Error(PSLICE() << "Can't load td::ref<vm::cell " << name);
  }
  return it->second;
}


td::Ref<vm::Cell> StakingSmartContractCode::staking_pool() {
  static auto res = load("staking-pool").move_as_ok();
  return res;
}

td::Ref<vm::Cell> StakingSmartContractCode::nominator() {
  static auto res = load("nominator").move_as_ok();
  return res;
}


}  // namespace ton
