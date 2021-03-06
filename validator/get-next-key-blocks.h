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
#pragma once

#include "validator/interfaces/validator-manager.h"

namespace ton {

namespace validator {

class GetNextKeyBlocks : public td::actor::Actor {
 public:
  GetNextKeyBlocks(BlockIdExt block_id, td::uint32 limit, BlockHandle last_known_key_block,
                   td::Ref<MasterchainState> masterchain_state, td::actor::ActorId<ValidatorManager> manager,
                   td::Timestamp timeout, td::Promise<std::vector<BlockIdExt>> promise)
      : block_id_(block_id)
      , limit_(limit)
      , last_known_key_block_(std::move(last_known_key_block))
      , masterchain_state_(std::move(masterchain_state))
      , manager_(manager)
      , timeout_(timeout)
      , promise_(std::move(promise)) {
  }

  void start_up() override;

  void finish_query();
  void abort_query(td::Status error);
  void alarm() override;

 private:
  BlockIdExt block_id_;
  td::uint32 limit_;

  BlockHandle last_known_key_block_;
  td::Ref<MasterchainState> masterchain_state_;

  td::actor::ActorId<ValidatorManager> manager_;
  td::Timestamp timeout_;

  td::Promise<std::vector<BlockIdExt>> promise_;

  std::vector<BlockIdExt> res_;
};

}  // namespace validator

}  // namespace ton
