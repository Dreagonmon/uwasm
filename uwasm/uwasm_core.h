#ifndef UWASM_CORE_H
#define UWASM_CORE_H

#include <stdint.h>

#define UWASM_MEM_PAGE_SIZE (65536)

#define UWASM_SECTION_TYPE (1)
#define UWASM_SECTION_IMPORT (2)
#define UWASM_SECTION_FUNCTION (3)
#define UWASM_SECTION_TABLE (4)
#define UWASM_SECTION_MEMORY (5)
#define UWASM_SECTION_GLOBAL (6)
#define UWASM_SECTION_EXPORT (7)
#define UWASM_SECTION_START (8)
#define UWASM_SECTION_ELEMENT (9)
#define UWASM_SECTION_CODE (10)
#define UWASM_SECTION_DATA (11)
#define UWASM_SECTION_DATACOUNT (12)

#define UWASM_TYPE_I32 (0x7F)
#define UWASM_TYPE_I64 (0x7E)
#define UWASM_TYPE_F32 (0x7D)
#define UWASM_TYPE_F64 (0x7C)
#define UWASM_TYPE_VEC (0x7B)
#define UWASM_TYPE_FUNCREF (0x70)
#define UWASM_TYPE_EXTERNREF (0x6F)

#if (__STDC_VERSION__ <= 201710L)
typedef char char8_t;
#else
#include <uchar.h>
// type changed for u8 string since c23
// typedef unsigned char char8_t;
#endif

typedef union UWasmValue {
    int32_t i32;
    uint32_t u32;
    float f32;
    double f64;
    void *pointer;
} UWasmValue;

typedef struct UWasmPNode {
    void *value;
    struct UWasmPNode *next;
    void *extra;
} UWasmPNode;

typedef UWasmPNode UWasmMemPage;

typedef UWasmPNode UWasmMem;

typedef void *(*UWasmRawFunction)(void *params);

typedef struct UWasmLinkedFunction {
    uint32_t func_id;
    UWasmRawFunction func;
    struct UWasmLinkedFunction *next;
} UWasmLinkedFunction;

typedef struct UWasmModule {
    uint8_t *mod_base;
    uint8_t *mod_pos;
    uint8_t *mod_top;
    UWasmValue *stack_base;
    UWasmValue *stack_pos;
    UWasmValue *stack_top;
    UWasmValue *stack_return_pos;
    UWasmLinkedFunction *linked_func;
    UWasmMem *mem;
    void *extra;
    uint32_t func_base_id;
    uint32_t table_base_id;
    uint32_t mem_base_id;
    uint32_t glob_base_id;
} UWasmModule;

#endif // UWASM_CORE_H
