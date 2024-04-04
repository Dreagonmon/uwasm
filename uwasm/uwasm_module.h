#ifndef UWASM_MODULE_H
#define UWASM_MODULE_H

#include "uwasm_core.h"
#include "uwasm_port.h"

/**
 * parse module
 */
bool uwm_compile_module(UWasmModule *module);
/**
 * link function to module
 */
bool uwm_module_link_function(UWasmModule *module, const char8_t *mod, const char8_t *name, UWasmLinkedFunction *func);
/**
 * find exported function
 */
bool uwm_find_exported_function(UWasmModule *module, const char8_t *name, uint32_t *func_id_dest);
/**
 * init module struct
 */
bool uwm_init_module(UWasmModule *module, const char8_t *path);

#if (UWASM_SUPPORT_HEAP)

/**
 * malloc new module and load the binary data
 */
UWasmModule *uwm_new_module(const char8_t *path, void *extra);
/**
 * free module and free the binary data
 */
void uwm_destory_module(UWasmModule *module);
/**
 * link function to module
 */
bool uwm_module_link_raw_function(UWasmModule *module, const char8_t *mod, const char8_t *name,
                                  UWasmRawFunction raw_func);

#endif

#endif // UWASM_MODULE_H