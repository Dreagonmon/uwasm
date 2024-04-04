#include "uwasm_utils.h"
#include <stddef.h>

bool uwm_name_is_matched(uint32_t wasm_vec_len, const uint8_t *wasm_name, const char8_t *cstr) {
    if (cstr == NULL || wasm_vec_len == 0) {
        return false;
    }
    uint8_t ch1;
    uint8_t ch2;
    do {
        ch1 = *wasm_name;
        wasm_name++;
        wasm_vec_len--;
        ch2 = (uint8_t)*cstr;
        cstr++;
        if (ch2 == '\0') {
            return false;
        }
        if (ch1 != ch2) {
            return false;
        }
    } while (wasm_vec_len > 0);
    return (*cstr) == '\0';
}
