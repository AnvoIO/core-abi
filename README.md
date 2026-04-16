# core-abi

[![MIT licensed][1]][2]

[1]: https://img.shields.io/badge/license-MIT-blue.svg
[2]: LICENSE

C++ library for binary ↔ JSON conversion of ABI-encoded data. Serializes and deserializes Anvo Core, Antelope, and EOSIO transactions, actions, and SHiP (State History) messages against a contract's ABI definition.

Based on [AntelopeIO/abieos](https://github.com/AntelopeIO/abieos) with first-class support for the `core_net::abi/*` version prefix emitted by chains bootstrapped under the Anvo Core `core_net` namespace.

## Features

- **Native Anvo Core support** — accepts `core_net::abi/{1,2}.x` ABI version prefixes alongside `eosio::abi/{1,2}.x`.
- **Source-compatible with Antelope/EOSIO** — existing C++ code using `eosio::` types compiles unchanged via a permanent `namespace eosio = core_net;` alias.
- **SHiP protocol support** — deserializes state-history WebSocket streams from Spring / Leap 5.x and Anvo Core nodes.
- **Compatible with languages that can interface to C** — see [`src/core_abi.h`](src/core_abi.h).
- **Rolling release** — `main` branch contains the latest fully supported version.

## Build

```bash
git clone --recursive https://github.com/AnvoIO/core-abi.git
cd core-abi
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
ctest --test-dir build
```

### Build Requirements

- CMake 3.16+
- C++17 compiler (GCC 13+ or Clang 16+ recommended)
- Threads support (POSIX or Windows native)

## Install

```bash
sudo cmake --install build
```

Installs the static library, public headers to `include/core_net/`, and CMake package config to `share/core-abi/`.

## Usage

### Packing a transaction

```cpp
#include <core_net/core_abi.hpp>

auto* ctx = core_abi_create();

// Load the standard transaction ABI into contract 0
core_abi_set_abi(ctx, /*contract=*/0, transaction_abi_json);

// Load your contract's ABI
uint64_t contract = core_abi_string_to_name(ctx, "eosio.token");
core_abi_set_abi(ctx, contract, token_abi_json);

// Convert action data
core_abi_json_to_bin(ctx, contract, "transfer", action_json);
const char* action_hex = core_abi_get_bin_hex(ctx);

// Convert transaction
uint64_t tx_type = core_abi_string_to_name(ctx, "transaction");
core_abi_json_to_bin(ctx, /*contract=*/0, "transaction", transaction_json);
const char* tx_hex = core_abi_get_bin_hex(ctx);

core_abi_destroy(ctx);
```

### Example action data

```json
{
    "from": "useraaaaaaaa",
    "to": "useraaaaaaab",
    "quantity": "0.0001 SYS",
    "memo": ""
}
```

Object attributes must appear in the order defined by the ABI.

## C++ Namespace

Public headers live under `include/core_net/`. The `core_net` namespace is the canonical home for all types and functions; `eosio` is preserved as a namespace alias for source compatibility with existing Antelope/EOSIO code.

```cpp
#include <core_net/core_abi.hpp>

// Both compile and refer to the same symbol:
core_net::name account{"alice"};
eosio::name    account{"alice"};  // alias
```

The two names refer to the same symbols — `core_net::name` and `eosio::name` are identical at both source and binary level. This alias is permanent; it is not a deprecation shim.

## ABI Version Compatibility

| Version prefix | Accepted on ingest |
|---|---|
| `eosio::abi/1.0` through `eosio::abi/2.x` | yes |
| `core_net::abi/1.0` through `core_net::abi/2.x` | yes |

Anvo Core emits the version prefix that matches the chain's heritage: eosio-bootstrapped chains emit `eosio::abi/*`; chains bootstrapped fresh under `core_net` emit `core_net::abi/*`. See [AnvoIO/core#105](https://github.com/AnvoIO/core/issues/105) for context.

## C API

The C API is the primary binary surface and is exposed from [`src/core_abi.h`](src/core_abi.h). All C API functions are prefixed with `core_abi_` (e.g., `core_abi_create`, `core_abi_json_to_bin`, `core_abi_get_bin_hex`). The C API is stable and suitable for binding from any language with C FFI support.

## Supported Platforms

| Platform | Architecture | Status |
|---|---|---|
| Ubuntu 24.04 | x86_64, ARM64 | Primary — CI tested |
| Ubuntu 22.04 | x86_64, ARM64 | Supported |
| macOS | x86_64, ARM64 | Best-effort |
| Other Linux | — | Best-effort |

## License

[MIT](./LICENSE). See [NOTICE](./NOTICE) for upstream attributions.

## Contributing

See [CONTRIBUTING.md](./CONTRIBUTING.md).
