#include "uwasm_utils.h"
#include "uwasm_port.h"
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

uint64_t uwm_module_read_integer_lsb(UWasmModule *module, uint8_t len) {
    uint64_t num = 0;
    uint8_t base = 0;
    uint8_t buf;
    while (base < len) {
        uwm_port_module_read(module, &buf, 1);
        num = num | ((uint64_t)buf << (base * 8));
        base++;
    }
    return num;
}

uint8_t uwm_module_read_byte(UWasmModule *module) {
    uint8_t buf;
    uwm_port_module_read(module, &buf, 1);
    return buf;
}

uint64_t uwm_module_read_uleb128(UWasmModule *module) {
    uint64_t num = 0;
    uint8_t base = 0;
    uint8_t buf;
    do {
        uwm_port_module_read(module, &buf, 1);
        num = num | ((uint64_t)(buf & 0b01111111) << base);
        base += 7;
    } while (buf & 0b10000000);
    return num;
}

int64_t uwm_module_read_sleb128(UWasmModule *module) {
    uint64_t num = 0;
    uint8_t base = 0;
    uint8_t buf;
    do {
        uwm_port_module_read(module, &buf, 1);
        num = num | ((uint64_t)(buf & 0b01111111) << base);
        base += 7;
    } while (buf & 0b10000000);
    if (buf & 0b01000000) {
        // nagetive
        int64_t neg = num | (-(1 << base));
        return neg;
    } else {
        // positive
        return (int64_t)num;
    }
}