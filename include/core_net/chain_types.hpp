#pragma once
#include "ship_protocol.hpp"

namespace chain_types {
using namespace core_net::ship_protocol;

struct block_info {
   uint32_t               block_num = {};
   core_net::checksum256     block_id  = {};
   core_net::block_timestamp timestamp;
};

EOSIO_REFLECT(block_info, block_num, block_id, timestamp);
}; // namespace chain_types