#ifndef UWASM_UTILS_H
#define UWASM_UTILS_H

#include "uwasm_core.h"
#include <stdbool.h>

bool uwm_name_is_matched(uint32_t wasm_vec_len, const uint8_t *wasm_name, const char8_t *cstr);
uint64_t uwm_module_read_integer_lsb(UWasmModule *module, uint8_t len);
uint8_t uwm_module_read_byte(UWasmModule *module);
uint64_t uwm_module_read_uleb128(UWasmModule *module);
int64_t uwm_module_read_sleb128(UWasmModule *module);

#endif // UWASM_UTILS_H