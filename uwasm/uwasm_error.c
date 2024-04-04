#include "uwasm_error.h"

static UWasmErrorCode error_code = UWASM_ERROR_OK;

void uwm_set_last_error_code(UWasmErrorCode code) { error_code = code; }

UWasmErrorCode uwm_get_last_error_code(void) { return error_code; }
