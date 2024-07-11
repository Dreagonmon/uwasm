#ifndef UWASM_TYPE_H
#define UWASM_TYPE_H

#include <stdbool.h>
#include "uwasm_core.h"

/* ==== internal ==== */
/** seek to type start (before 0x60 byte). */
bool uwm_module_seek_to_type(UWasmModule *module, uint32_t type_id);


#endif // UWASM_TYPE_H