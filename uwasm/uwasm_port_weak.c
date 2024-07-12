#include "uwasm_port.h"

__attribute__((weak)) bool uwm_port_module_open(UWasmModule *module, __attribute__((unused)) const char8_t *path) {
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

__attribute__((weak)) bool uwm_port_stack_create(UWasmModule *module) {
    module->stack_type_base = NULL;
    module->stack_base = NULL;
    module->stack_pos = NULL;
    module->stack_top = NULL;
    module->stack_return_pos = NULL;
    module->stack_global_pos = NULL;
    return true;
}

__attribute__((weak)) void uwm_port_stack_close(__attribute__((unused)) UWasmModule *module) { return; }

__attribute__((weak)) bool uwm_port_stack_read(UWasmModule *module, UWasmValue *pos, uint8_t *itype_dest,
                                               UWasmValue *dest) {
    if (pos >= module->stack_base && pos < module->stack_top) {
        *dest = *pos;
        uint8_t *stack_type = module->stack_type_base + (pos - module->stack_base);
        *itype_dest = *stack_type;
        return true;
    }
    return false;
}

__attribute__((weak)) bool uwm_port_stack_write(UWasmModule *module, UWasmValue *pos, uint8_t *itype_source,
                                                UWasmValue *source) {
    if (pos >= module->stack_base && pos < module->stack_top) {
        *pos = *source;
        uint8_t *stack_type = module->stack_type_base + (pos - module->stack_base);
        *stack_type = *itype_source;
        return true;
    }
    return false;
}

__attribute__((weak)) bool uwm_port_memory_create(UWasmMemPage *mem) {
    mem->extra = NULL;
#if (UWASM_SUPPORT_HEAP)
    mem->value = UWASM_MALLOC(UWASM_MEM_PAGE_SIZE);
    if (mem->value == NULL) {
        return false;
    }
#endif
    return true;
}

__attribute__((weak)) void uwm_port_memory_close(UWasmMemPage *mem) {
#if (UWASM_SUPPORT_HEAP)
    if (mem->value) {
        UWASM_FREE(mem->value);
        mem->value = NULL;
    }
#endif
}

__attribute__((weak)) bool uwm_port_memory_read(UWasmMemPage *mem, uint16_t offset, uint8_t *buffer, uwm_size_t len) {
    if (mem->value != NULL) {
        uint8_t *mem8 = (uint8_t *)mem->value;
        mem8 += offset;
        while (len > 0) {
            *buffer = *mem8;
            mem8++;
            buffer++;
            len--;
        }
        return true;
    } else {
        return false;
    }
}

__attribute__((weak)) bool uwm_port_memory_write(UWasmMemPage *mem, uint16_t offset, uint8_t *buffer, uwm_size_t len) {
    if (mem->value != NULL) {
        uint8_t *mem8 = (uint8_t *)mem->value;
        mem8 += offset;
        while (len > 0) {
            *mem8 = *buffer;
            mem8++;
            buffer++;
            len--;
        }
        return true;
    } else {
        return false;
    }
}
