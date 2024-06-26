#ifndef UWASM_PORT_H
#define UWASM_PORT_H

#include "uwasm_core.h"
#include <stdbool.h>

/* import and export name buffer size limit, on stack */
#define UWASM_MAX_NAME_LENGTH (32)

/* use malloc() and free() functions */
#define UWASM_SUPPORT_HEAP (1)
#if (UWASM_SUPPORT_HEAP)
#include <stdlib.h>
#define UWASM_MALLOC(n) malloc(n)
#define UWASM_FREE(ptr) free(ptr)
#endif

/* debug printf */
#ifndef UWASM_DEBUG
#define UWASM_DEBUG (0)
#endif
#if (UWASM_DEBUG)
#include <stdio.h>
#define uwm_log(...) printf(__VA_ARGS__)
#else
#define uwm_log(...)
#endif

/* host usize type */
typedef uint32_t uwm_size_t;
/* host ssize type */
typedef int32_t uwm_ssize_t;
#define UWASM_SEEK_SET (0)
#define UWASM_SEEK_CUR (1)
#define UWASM_SEEK_END (2)

/**
 * open wasm module.
 * return true if succeed.
 */
bool uwm_port_module_open(UWasmModule *module, const char8_t *path);
/**
 * close wasm module.
 * clean up.
 */
void uwm_port_module_close(UWasmModule *module);
/**
 * read wasm module. like fread().
 * return bytes being read.
 */
uwm_size_t uwm_port_module_read(UWasmModule *module, uint8_t *buffer, uwm_size_t len);
/**
 * seek wasm module. like fseek().
 * return true if succeed.
 */
bool uwm_port_module_seek(UWasmModule *module, uwm_ssize_t offset, uint8_t whence);
/**
 * tell wasm module position. like ftell().
 * return zero or positive if succeed.
 */
uwm_ssize_t uwm_port_module_tell(UWasmModule *module);

#endif // UWASM_PORT_H