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
 * clean up extra data.
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

/**
 * create stack in module.
 * return true if succeed.
 */
bool uwm_port_stack_create(UWasmModule *module);

/**
 * close stack in module.
 * clean up stack.
 */
void uwm_port_stack_close(UWasmModule *module);

/**
 * read from a stack position.
 * return true if succeed.
 */
bool uwm_port_stack_read(UWasmModule *module, UWasmValue *pos, uint8_t *itype, UWasmValue *dest);

/**
 * write to a stack position.
 * return true if succeed.
 */
bool uwm_port_stack_write(UWasmModule *module, UWasmValue *pos, uint8_t *itype, UWasmValue *source);

/**
 * create a 65536 bytes memory page.
 * return true if succeed.
 */
bool uwm_port_memory_create(UWasmMemPage *mem);

/**
 * close a memory page.
 * clean up extra data.
 */
void uwm_port_memory_close(UWasmMemPage *mem);

/**
 * read from a memory page.
 * return true if succeed.
 */
bool uwm_port_memory_read(UWasmMemPage *mem, uint16_t offset, uint8_t *buffer, uwm_size_t len);

/**
 * write to a memory page.
 * return true if succeed.
 */
bool uwm_port_memory_write(UWasmMemPage *mem, uint16_t offset, uint8_t *buffer, uwm_size_t len);

#endif // UWASM_PORT_H