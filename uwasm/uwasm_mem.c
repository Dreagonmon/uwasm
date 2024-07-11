#include "uwasm_mem.h"
#include "uwasm_module.h"
#include "uwasm_type.h"
#include "uwasm_error.h"
#include "uwasm_utils.h"

bool uwm_module_seek_to_memory(UWasmModule *module, uint32_t mem_id) {
    if (!uwm_module_seek_to_section(module, UWASM_SECTION_MEMORY)) {
        return false;
    }
    mem_id -= module->mem_base_id; // the real mem id
    uint32_t section_size = uwm_module_read_uleb128(module);
    if (section_size > 0) {
        uint32_t mem_count = uwm_module_read_uleb128(module);
        // uwm_log("mem_count: %u\n", func_count);
        if (mem_id >= mem_count) {
            // not found
            uwm_error_return(UWASM_ERROR_MODULE_MEMORY_NOT_FOUND, false);
        }
        // skip 
        uint8_t limit_has_maxmium;
        while (mem_id > 0) {
            mem_id--;
            limit_has_maxmium = uwm_module_read_byte(module);
            uwm_module_read_uleb128(module); // min
            if (limit_has_maxmium) {
                uwm_module_read_uleb128(module); // max
            }
        }
        // seek to type
        // limit_has_maxmium = uwm_module_read_byte(module);
        // uint32_t v_min = uwm_module_read_uleb128(module); // min
        // if (limit_has_maxmium) {
        //     uint32_t v_max = uwm_module_read_uleb128(module); // max
        //     uwm_log("limit: %u %u\n", v_min, v_max);
        // } else {
        //     uwm_log("limit: %u ~\n", v_min);
        // }
        return true;
    }
    uwm_error_return(UWASM_ERROR_MODULE_MEMORY_NOT_FOUND, false);
}