#include "uwasm_type.h"
#include "uwasm_module.h"
#include "uwasm_utils.h"
#include "uwasm_error.h"
#include "uwasm_utils.h"

/** seek to type start (before 0x60 byte). */
bool uwm_module_seek_to_type(UWasmModule *module, uint32_t type_id) {
    if (!uwm_module_seek_to_section(module, UWASM_SECTION_TYPE)) {
        return false;
    }
    uint32_t section_size = uwm_module_read_uleb128(module);
    if (section_size > 0) {
        // read function type one by one
        uint32_t type_count = uwm_module_read_uleb128(module);
        // uwm_log("type count: %u\n", type_count);
        if (type_id >= type_count) {
            // not found
            uwm_error_return(UWASM_ERROR_MODULE_FUNCTYPE_NOT_FOUND, false);
        }
        // skip types
        uint32_t type_vec_size;
        while (type_id > 0) {
            type_id--;
            uwm_error_return_check_module_seek(module, 1, UWASM_SEEK_CUR, false);
            type_vec_size = uwm_module_read_uleb128(module); // skip params
            uwm_error_return_check_module_seek(module, type_vec_size, UWASM_SEEK_CUR, false);
            type_vec_size = uwm_module_read_uleb128(module); // skip result
            uwm_error_return_check_module_seek(module, type_vec_size, UWASM_SEEK_CUR, false);
        }
        // check 1 byte
        // uint8_t b = uwm_module_read_byte(module);
        // uwm_error_return_check_bool(b == 0x60, UWASM_ERROR_MODULE_INVALID, false);
        return true;
    }
    // not found
    uwm_error_return(UWASM_ERROR_MODULE_FUNCTYPE_NOT_FOUND, false);
}
