#include "uwasm_port.h"
#include <stdio.h>

#define MAX_MODULE_SIZE (65536)
#define MODULE_STACK_SIZE (4096)

bool uwm_port_module_open(UWasmModule *module, const char8_t *path) {
    FILE *fp = fopen(path, "rb");
    if (fp == NULL) {
        return false;
    }
    int res = fseek(fp, 0, SEEK_END);
    if (res != 0) {
        fclose(fp);
        return false;
    }
    // uwm_log("seek %d\n", res);
    long f_size = ftell(fp);
    // uwm_log("size %lu\n", f_size);
    if (f_size > 0) {
        f_size = ((size_t)f_size > MAX_MODULE_SIZE) ? (long)MAX_MODULE_SIZE : f_size;
        res = fseek(fp, 0, SEEK_SET);
        // uwm_log("seek %d\n", res);
        if (res != 0) {
            fclose(fp);
            return false;
        }
        uint8_t *buffer = UWASM_MALLOC(MAX_MODULE_SIZE);
        size_t count = fread(buffer, 1, f_size, fp);
        // uwm_log("read %d\n", res);
        if (count != (size_t) f_size) {
            fclose(fp);
            UWASM_FREE(buffer);
            return false;
        }
        module->mod_base = buffer;
        module->mod_pos = buffer;
        module->mod_top = buffer + f_size;
        module->extra = fp;
        return true;
    } else {
        fclose(fp);
        return false;
    }
}

void uwm_port_module_close(UWasmModule *module) {
    if (module->mod_base != NULL) {
        UWASM_FREE(module->mod_base);
        module->mod_base = NULL;
        module->mod_pos = NULL;
        module->mod_top = NULL;
    }
    if (module->extra != NULL) {
        FILE *fp = module->extra;
        fclose(fp);
        module->extra = NULL;
    }
}

bool uwm_port_stack_create(UWasmModule *module) {
    UWasmValue *stack = UWASM_MALLOC(sizeof(UWasmValue) * MODULE_STACK_SIZE);
    if (stack == NULL) {
        return false;
    }
    module->stack_base = stack;
    module->stack_pos = stack;
    module->stack_top = stack + MODULE_STACK_SIZE;
    module->stack_return_pos = stack;
    return true;
}

void uwm_port_stack_close(UWasmModule *module) {
    if (module->stack_base != NULL) {
        UWASM_FREE(module->stack_base);
        module->stack_base = NULL;
        module->stack_pos = NULL;
        module->stack_top = NULL;
        module->stack_return_pos = NULL;
    }
}
