#ifndef UWASM_UTILS_H
#define UWASM_UTILS_H

#include <stdbool.h>
#include "uwasm_core.h"

bool uwm_name_is_matched(uint32_t wasm_vec_len, const uint8_t *wasm_name, const char8_t *cstr);

#endif // UWASM_UTILS_H