#ifndef UWASM_ERROR_H
#define UWASM_ERROR_H

#include <stdint.h>

typedef enum UWasmErrorCode {
    UWASM_ERROR_OK = 0x00000000,
    UWASM_ERROR_MALLOC,
    UWASM_ERROR_MODULE_OPEN,
    UWASM_ERROR_MODULE_SEEK,
    UWASM_ERROR_MODULE_READ_OR_INVALID,
    UWASM_ERROR_MODULE_UNSUPPORT,
    UWASM_ERROR_MODULE_INVALID,
    UWASM_ERROR_MODULE_SECTION_NOT_FOUND,
    UWASM_ERROR_MODULE_FUNCTION_NOT_FOUND,
    UWASM_ERROR_MODULE_FUNCTYPE_NOT_MATCHED,
    UWASM_ERROR_MODULE_FUNCTYPE_NOT_FOUND,
    UWASM_ERROR_MODULE_NAME_TOO_LONG,
    UWASM_ERROR_MODULE_MEMORY_NOT_FOUND,
    UWASM_ERROR_MODULE_STACK_CREATE,
} UWasmErrorCode;

void uwm_set_last_error_code(UWasmErrorCode code);
UWasmErrorCode uwm_get_last_error_code(void);

#define uwm_error_return(error_code, return_value)                                                                     \
    uwm_set_last_error_code((error_code));                                                                             \
    return (return_value)

#define uwm_error_check_bool(condition, error_code, on_error_op)                                                       \
    if (!(condition)) {                                                                                                \
        uwm_set_last_error_code((error_code));                                                                         \
        on_error_op;                                                                                                   \
    }

#define uwm_error_return_check_bool(condition, error_code, return_value_if_false)                                      \
    uwm_error_check_bool(condition, error_code, return (return_value_if_false))

#define uwm_error_return_check_module_seek(module, offset, whence, return_value_if_false)                              \
    uwm_error_return_check_bool(uwm_port_module_seek(module, offset, whence), UWASM_ERROR_MODULE_SEEK,                 \
                                return_value_if_false)

#endif // UWASM_ERROR_H