#include "uwasm_module.h"
#include "uwasm_func.h"
#include "uwasm_error.h"
#include <stdint.h>
#include <stdio.h>

UWasmModule *load_wasm_binary(void) {
    UWasmModule *module = uwm_new_module(u8"test/coremark.wasm", NULL);
    return module;
}

UWasmModule *module;
int main(void) {
    printf("STDC VERSION: %lu\n", __STDC_VERSION__);
    // read wasm binary
    module = load_wasm_binary();
    printf("wasm binary size: %lu.\n", module->mod_top - module->mod_base);
    printf("========================================\n");
    // compile module
    bool res = uwm_compile_module(module);
    printf("compile: %s.\n", res ? "succeed" : "failed");
    printf("func_base_id: %u\n", module->func_base_id);
    printf("table_base_id: %u\n", module->table_base_id);
    printf("mem_base_id: %u\n", module->mem_base_id);
    printf("glob_base_id: %u\n", module->glob_base_id);
    // link function
    res = uwm_module_link_raw_function(module, "env", "clock_ms", NULL);
    printf("link: %s.\n", res ? "succeed" : "failed");
    if (!res) {
        printf("error code: %d\n", uwm_get_last_error_code());
    }
    printf("linked_func: 0x%p\n", module->linked_func);
    // find function
    uint32_t func_id = 0xFF;
    res = uwm_find_exported_function(module, "run", &func_id);
    printf("find: %s.\n", res ? "succeed" : "failed");
    if (!res) {
        printf("error code: %d\n", uwm_get_last_error_code());
    }
    printf("exported_func: %u\n", func_id);
    // test
    printf("================\n");
    res = uwm_module_seek_to_function_type(module, 15);
    if (!res) {
        printf("error code: %d\n", uwm_get_last_error_code());
    }
    // init runtime
    // destory
    uwm_destory_module(module);
    return 0;
}
