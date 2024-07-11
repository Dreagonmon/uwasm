#ifndef UWASM_FUNC_H
#define UWASM_FUNC_H

#include <stdbool.h>
#include "uwasm_core.h"

/* ==== internal ==== */
/** seek to internal function type, by function id. */
bool uwm_module_seek_to_function_type(UWasmModule *module, uint32_t func_id);


#endif // UWASM_FUNC_H