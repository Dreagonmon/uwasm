#include "uwasm_port.h"

__attribute__((weak)) bool uwm_port_module_open(__attribute__((unused)) UWasmModule *module,
                                                __attribute__((unused)) const char8_t *path) {
    module->mod_base = NULL;
    module->mod_pos = NULL;
    module->mod_top = NULL;
    return true;
}

__attribute__((weak)) void uwm_port_module_close(__attribute__((unused)) UWasmModule *module) { return; }

__attribute__((weak)) uwm_size_t uwm_port_module_read(UWasmModule *module, uint8_t *buffer, uwm_size_t len) {
    uwm_size_t read = 0;
    while (len > 0 && module->mod_pos < module->mod_top) {
        buffer[read] = *(module->mod_pos);
        read++;
        module->mod_pos++;
        len--;
    }
    return read;
}

__attribute__((weak)) bool uwm_port_module_seek(UWasmModule *module, uwm_ssize_t offset, uint8_t whence) {
    uint8_t *nptr = module->mod_base;
    switch (whence) {
    case UWASM_SEEK_SET:
        nptr = module->mod_base + offset;
        break;
    case UWASM_SEEK_CUR:
        nptr = module->mod_pos + offset;
        break;
    case UWASM_SEEK_END:
        nptr = module->mod_top - offset;
        break;
    default:
        break;
    }
    if (nptr < module->mod_base || nptr > module->mod_top) {
        return false;
    }
    module->mod_pos = nptr;
    return true;
}

__attribute__((weak)) uwm_ssize_t uwm_port_module_tell(UWasmModule *module) {
    return module->mod_pos - module->mod_base;
}
