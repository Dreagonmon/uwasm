#ifndef UWASM_MEM_H
#define UWASM_MEM_H

#include <stdbool.h>
#include "uwasm_core.h"

/* ==== internal ==== */
/** seek to internal mem */
bool uwm_module_seek_to_memory(UWasmModule *module, uint32_t mem_id);

#endif // UWASM_MEM_H