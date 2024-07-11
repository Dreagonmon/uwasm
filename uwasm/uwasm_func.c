#include "uwasm_func.h"
#include "uwasm_module.h"
#include "uwasm_utils.h"
#include "uwasm_type.h"
#include "uwasm_error.h"
#include "uwasm_utils.h"

bool uwm_module_seek_to_function_type(UWasmModule *module, uint32_t func_id) {
    if (!uwm_module_seek_to_section(module, UWASM_SECTION_FUNCTION)) {
        return false;
    }
    func_id -= module->func_base_id; // the real func id
    uint32_t section_size = uwm_module_read_uleb128(module);
    if (section_size > 0) {
        uint32_t func_count = uwm_module_read_uleb128(module);
        // uwm_log("func_count: %u\n", func_count);
        if (func_id >= func_count) {
            // not found
            uwm_error_return(UWASM_ERROR_MODULE_FUNCTION_NOT_FOUND, false);
        }
        // skip 
        while (func_id > 0) {
            func_id--;
            uwm_module_read_uleb128(module); // skip func type id
            // uint32_t type_id = uwm_module_read_uleb128(module); // skip func type id
            // uwm_log("func: %u type: %u\n", func_count - func_id - 1, type_id);
        }
        // seek to type
        uint32_t type_id = uwm_module_read_uleb128(module); // skip func type id
        uwm_module_seek_to_type(module, type_id);
        return true;
    }
    uwm_error_return(UWASM_ERROR_MODULE_FUNCTION_NOT_FOUND, false);
}