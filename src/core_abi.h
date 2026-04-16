// copyright defined in LICENSE

#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct core_abi_context_s core_abi_context;
typedef int core_abi_bool;

// Create a context. The context holds all memory allocated by functions in this header. Returns null on failure.
core_abi_context* core_abi_create();

// Destroy a context.
void core_abi_destroy(core_abi_context* context);

// Get last error. Never returns null. The context owns the returned string.
const char* core_abi_get_error(core_abi_context* context);

// Get generated binary. The context owns the returned memory. Functions return null on error; use core_abi_get_error to
// retrieve error.
int core_abi_get_bin_size(core_abi_context* context);
const char* core_abi_get_bin_data(core_abi_context* context);

// Convert generated binary to hex. The context owns the returned string. Returns null on error; use core_abi_get_error to
// retrieve error.
const char* core_abi_get_bin_hex(core_abi_context* context);

// Name conversion. The context owns the returned memory. Functions return null on error; use core_abi_get_error to
// retrieve error.
uint64_t core_abi_string_to_name(core_abi_context* context, const char* str);
const char* core_abi_name_to_string(core_abi_context* context, uint64_t name);

// Set abi (JSON format). Returns false on error.
core_abi_bool core_abi_set_abi(core_abi_context* context, uint64_t contract, const char* abi);

// Set abi (binary format). Returns false on error.
core_abi_bool core_abi_set_abi_bin(core_abi_context* context, uint64_t contract, const char* data, size_t size);

// Set abi (hex format). Returns false on error.
core_abi_bool core_abi_set_abi_hex(core_abi_context* context, uint64_t contract, const char* hex);

// Get the type name for an action. The context owns the returned memory. Returns null on error; use core_abi_get_error
// to retrieve error.
const char* core_abi_get_type_for_action(core_abi_context* context, uint64_t contract, uint64_t action);

// Get the type name for a table. The context owns the returned memory. Returns null on error; use core_abi_get_error
// to retrieve error.
const char* core_abi_get_type_for_table(core_abi_context* context, uint64_t contract, uint64_t table);

// Get the type name for an action_result. The context owns the returned memory. Returns null on error; use
// core_abi_get_error to retrieve error.
const char* core_abi_get_type_for_action_result(core_abi_context* context, uint64_t contract, uint64_t action_result);

// Convert json to binary. Use core_abi_get_bin_* to retrieve result. Returns false on error.
core_abi_bool core_abi_json_to_bin(core_abi_context* context, uint64_t contract, const char* type, const char* json);

// Convert json to binary. Allow json field reordering. Use core_abi_get_bin_* to retrieve result. Returns false on error.
core_abi_bool core_abi_json_to_bin_reorderable(core_abi_context* context, uint64_t contract, const char* type,
                                           const char* json);

// Convert binary to json. The context owns the returned string. Returns null on error; use core_abi_get_error to retrieve
// error.
const char* core_abi_bin_to_json(core_abi_context* context, uint64_t contract, const char* type, const char* data,
                               size_t size);

// Convert hex to json. The context owns the returned memory. Returns null on error; use core_abi_get_error to retrieve
// error.
const char* core_abi_hex_to_json(core_abi_context* context, uint64_t contract, const char* type, const char* hex);

// Convert abi json to bin, Use core_abi_get_bin_* to retrieve result. Returns false on error.
core_abi_bool core_abi_abi_json_to_bin(core_abi_context* context, const char* json);

// Convert abi bin to json, The context.result_str has the result, Returns null on error; use core_abi_get_error to
// retrieve
const char* core_abi_abi_bin_to_json(core_abi_context* context, const char* abi_bin_data, const size_t abi_bin_data_size);

// Delete a contract from the context
core_abi_bool core_abi_delete_contract(core_abi_context* context, uint64_t contract);

#ifdef __cplusplus
}
#endif
