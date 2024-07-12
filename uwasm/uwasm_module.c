#include "uwasm_module.h"
#include "uwasm_error.h"
#include "uwasm_utils.h"

static const uint8_t MAGIC_HEADER[] = {'\0', 'a', 's', 'm'};

/** skip 1 import element */
static bool uwm_module_skip_import(UWasmModule *module, uint8_t import_type) {
    uint8_t limit_has_maxmium;
    switch (import_type) {
    case 0x00:
        // function
        uwm_module_read_uleb128(module); // func type
        break;
    case 0x01:
        // table
        uwm_module_read_byte(module);
        __attribute__((fallthrough));
    case 0x02:
        // mem
        limit_has_maxmium = uwm_module_read_byte(module);
        uwm_module_read_uleb128(module);
        if (limit_has_maxmium) {
            uwm_module_read_uleb128(module);
        }
        break;
    case 0x03:
        // global
        uwm_module_read_byte(module);
        uwm_module_read_byte(module);
        break;
    }
    return true;
}

bool uwm_module_seek_to_section(UWasmModule *module, uint8_t section_id) {
    // skip header
    uwm_error_return_check_module_seek(module, 8, UWASM_SEEK_SET, false);
    // skip other sections
    uint8_t buf;
    uwm_size_t count;
    do {
        count = uwm_port_module_read(module, &buf, 1);
        if (count <= 0) {
            break;
        }
        // uwm_log("section type: %u\n", buf);
        if (buf == section_id) {
            return true;
        } else {
            // skip
            uint32_t section_size = uwm_module_read_uleb128(module);
            uwm_error_return_check_module_seek(module, (uwm_ssize_t)section_size, UWASM_SEEK_CUR, false);
        }
    } while (count > 0);
    uwm_error_return(UWASM_ERROR_MODULE_SECTION_NOT_FOUND, false);
}

/* ========================================================================== */

bool uwm_compile_module(UWasmModule *module) {
    uwm_error_return_check_module_seek(module, 0, UWASM_SEEK_SET, false);
    // check magic number
    uint8_t buffer[4];
    uwm_size_t count = uwm_port_module_read(module, buffer, 4);
    if (count != 4 || buffer[0] != MAGIC_HEADER[0] || buffer[1] != MAGIC_HEADER[1] || buffer[2] != MAGIC_HEADER[2] ||
        buffer[3] != MAGIC_HEADER[3]) {
        uwm_error_return(UWASM_ERROR_MODULE_INVALID, false);
    }
    uint32_t version = uwm_module_read_integer_lsb(module, 4);
    uwm_error_return_check_bool(version == 1, UWASM_ERROR_MODULE_UNSUPPORT, false);
    // uwm_log("wasm version: %u\n", version);
    // parse minimal function/table/mem/global id
    do {
        // read section id
        count = uwm_port_module_read(module, buffer, 1);
        if (count <= 0) {
            break;
        }
        uint32_t section_size = uwm_module_read_uleb128(module);
        uint32_t import_count, len;
        uint8_t import_type;
        switch (buffer[0]) {
        case UWASM_SECTION_IMPORT:
            // check import, generate minimal id
            import_count = uwm_module_read_uleb128(module);
            while (import_count) {
                import_count--;
                len = uwm_module_read_uleb128(module); // mod
                uwm_error_return_check_module_seek(module, len, UWASM_SEEK_CUR, false);
                len = uwm_module_read_uleb128(module); // name
                uwm_error_return_check_module_seek(module, len, UWASM_SEEK_CUR, false);
                import_type = uwm_module_read_byte(module);
                switch (import_type) {
                case 0x00:
                    module->func_base_id++;
                    break;
                case 0x01:
                    module->table_base_id++;
                    break;
                case 0x02:
                    module->mem_base_id++;
                    break;
                case 0x03:
                    module->glob_base_id++;
                    break;
                }
                if (!uwm_module_skip_import(module, import_type)) {
                    return false;
                }
            }
            break;
        default:
            // skip
            uwm_error_return_check_module_seek(module, (uwm_ssize_t)section_size, UWASM_SEEK_CUR, false);
            break;
        }
    } while (count > 0);
    // TODO: cache and speed up.
    return true;
}

bool uwm_module_link_function(UWasmModule *module, const char8_t *mod, const char8_t *name, UWasmLinkedFunction *func) {
    if (!uwm_module_seek_to_section(module, UWASM_SECTION_IMPORT)) {
        return false;
    }
    uwm_module_read_uleb128(module); // skip section size
    uint32_t count = uwm_module_read_uleb128(module);          // import count
    uint32_t func_id = 0;
    uint8_t buffer[UWASM_MAX_NAME_LENGTH];
    while (count) {
        uint8_t match_count = 0;
        count--;
        uint32_t len = uwm_module_read_uleb128(module);
        uwm_error_return_check_bool(len <= UWASM_MAX_NAME_LENGTH, UWASM_ERROR_MODULE_NAME_TOO_LONG, false);
        uwm_port_module_read(module, buffer, len);
        if (uwm_name_is_matched(len, buffer, mod)) {
            match_count++;
        }
        len = uwm_module_read_uleb128(module);
        uwm_error_return_check_bool(len <= UWASM_MAX_NAME_LENGTH, UWASM_ERROR_MODULE_NAME_TOO_LONG, false);
        uwm_port_module_read(module, buffer, len);
        if (uwm_name_is_matched(len, buffer, name)) {
            match_count++;
        }
        uint8_t import_type = uwm_module_read_byte(module);
        if (match_count >= 2) {
            // matched
            uwm_log("linked function: %s.%s\n", mod, name);
            uwm_error_return_check_bool(import_type == 0x00, UWASM_ERROR_MODULE_FUNCTYPE_NOT_MATCHED, false);
            __attribute__((unused)) uint32_t type_idx = uwm_module_read_uleb128(module);
            uwm_log("function type: %u\n", type_idx);
            // TODO: check function params type and return type
            // make function
            func->func_id = func_id;
            func->next = NULL;
            // link function
            if (module->linked_func == NULL) {
                module->linked_func = func;
            } else {
                UWasmLinkedFunction *flast = module->linked_func;
                while (flast->next != NULL) {
                    flast = flast->next;
                }
                flast->next = func;
            }
            return true;
        } else {
            // skip this import
            if (!uwm_module_skip_import(module, import_type)) {
                return false;
            }
            if (import_type == 0x00) {
                func_id++;
            }
        }
    }
    uwm_error_return(UWASM_ERROR_MODULE_FUNCTION_NOT_FOUND, false);
}

bool uwm_find_exported_function(UWasmModule *module, const char8_t *name, uint32_t *func_id_dest) {
    if (!uwm_module_seek_to_section(module, UWASM_SECTION_EXPORT)) {
        return false;
    }
    uwm_module_read_uleb128(module); // skip section size
    uint32_t count = uwm_module_read_uleb128(module);          // export count
    uint8_t buffer[UWASM_MAX_NAME_LENGTH];
    while (count) {
        count--;
        uint32_t len = uwm_module_read_uleb128(module);
        uwm_error_return_check_bool(len <= UWASM_MAX_NAME_LENGTH, UWASM_ERROR_MODULE_NAME_TOO_LONG, false);
        uwm_port_module_read(module, buffer, len);
        uint8_t export_type = uwm_module_read_byte(module);
        uint32_t idx = uwm_module_read_uleb128(module);
        if (export_type != 0x00) {
            continue; // not a function
        }
        if (uwm_name_is_matched(len, buffer, name)) {
            uwm_log("function found: %s\n", name);
            *func_id_dest = idx;
            return true;
        }
    }
    uwm_error_return(UWASM_ERROR_MODULE_FUNCTION_NOT_FOUND, false);
}

bool uwm_init_module(UWasmModule *module, const char8_t *path) {
    module->linked_func = NULL;
    module->mem = NULL;
    module->func_base_id = 0;
    module->table_base_id = 0;
    module->mem_base_id = 0;
    module->glob_base_id = 0;
    uwm_error_return_check_bool(uwm_port_module_open(module, path), UWASM_ERROR_MODULE_OPEN, false);
    uwm_error_return_check_bool(uwm_port_stack_create(module), UWASM_ERROR_MODULE_OPEN, false);
    return true;
}

void uwm_deinit_module(UWasmModule *module) {
    uwm_port_module_close(module);
    uwm_port_stack_close(module);
}

#if (UWASM_SUPPORT_HEAP)

UWasmModule *uwm_new_module(const char8_t *path, void *extra) {
    UWasmModule *module = UWASM_MALLOC(sizeof(UWasmModule));
    if (module != NULL) {
        module->linked_func = NULL;
        module->mem = NULL;
        module->extra = extra;
        module->func_base_id = 0;
        module->table_base_id = 0;
        module->mem_base_id = 0;
        module->glob_base_id = 0;
        if (!uwm_init_module(module, path)) {
            UWASM_FREE(module);
            module = NULL;
        }
    } else {
        uwm_set_last_error_code(UWASM_ERROR_MALLOC);
    }
    return module;
}

void uwm_destory_module(UWasmModule *module) {
    if (module != NULL) {
        uwm_deinit_module(module);
        // functions
        UWasmLinkedFunction *func = module->linked_func;
        while (func != NULL) {
            UWasmLinkedFunction *fnext = func->next;
            UWASM_FREE(func);
            func = fnext;
        }
        // mem
        UWasmMem *mem = module->mem;
        while (mem != NULL) {
            // mem page
            UWasmMemPage *page = mem->value;
            while (page != NULL) {
                if (page->value != NULL) {
                    UWASM_FREE(page->value);
                }
                UWasmMemPage *pnext = page->next;
                UWASM_FREE(page);
                page = pnext;
            }
            // outer mem
            UWasmMem *mnext = mem->next;
            UWASM_FREE(mem);
            mem = mnext;
        }
        // finally
        UWASM_FREE(module);
    }
}

bool uwm_module_link_raw_function(UWasmModule *module, const char8_t *mod, const char8_t *name,
                                  UWasmRawFunction raw_func) {
    // make function
    UWasmLinkedFunction *func = UWASM_MALLOC(sizeof(UWasmLinkedFunction));
    uwm_error_return_check_bool(func != NULL, UWASM_ERROR_MALLOC, false);
    func->func = raw_func;
    func->func_id = 0;
    func->next = NULL;
    if (!uwm_module_link_function(module, mod, name, func)) {
        UWASM_FREE(func);
        return false;
    }
    return true;
}

#endif
