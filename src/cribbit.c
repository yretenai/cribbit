// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include <cribbit/cribbit.h>
#include <stdio.h>

#include "feature/cribbit_string.h"

cribbit_alloc_callback cribbit_alloc_cb = NULL;
cribbit_realloc_callback cribbit_realloc_cb = NULL;
cribbit_clear_callback cribbit_clear_cb = NULL;
cribbit_free_callback cribbit_free_cb = NULL;
cribbit_log_calblack cribbit_log_cb = NULL;

void* cribbit_alloc_libc(size_t elements, size_t size) {
    // todo: make this configurable.
    return calloc(elements, size);
}

void* cribbit_realloc_libc(void* ptr, size_t size) {
    return realloc(ptr, size);
}

void cribbit_clear_libc(void* ptr, size_t size) {
    if(ptr == NULL) {
        return;
    }

    memset(ptr, 0, size);
}

void cribbit_free_libc(void* ptr) {
    // todo: make this configurable.
    free(ptr);
}

void* cribbit_alloc(size_t elements, size_t size) {
    if(cribbit_alloc_cb == NULL) {
        cribbit_set_libc();
    }

    return cribbit_alloc_cb(elements, size);
}

void* cribbit_realloc(void* ptr, size_t size) {
    if(cribbit_realloc_cb == NULL) {
        cribbit_set_libc();
    }

    return cribbit_realloc_cb(ptr, size);
}

void cribbit_clear(void* ptr, size_t size) {
    if(cribbit_clear_cb == NULL) {
        cribbit_set_libc();
    }

    cribbit_clear_cb(ptr, size);
}

void cribbit_free(void* ptr) {
    if(cribbit_free_cb == NULL) {
        cribbit_set_libc();
    }

    cribbit_free_cb(ptr);
}

void cribbit_log(const char* fmt, ...) {
    if(cribbit_log_cb == NULL) {
        return;
    }

    va_list args;
    va_start(args, fmt);
    cribbit_log_cb(fmt, args);
    va_end(args);
}

void cribbit_set_libc(void) {
    cribbit_alloc_cb = cribbit_alloc_libc;
    cribbit_realloc_cb = cribbit_realloc_libc;
    cribbit_clear_cb = cribbit_clear_libc;
    cribbit_free_cb = cribbit_free_libc;
}

void cribbit_set_printf(void) {
    cribbit_log_cb = vprintf;
}

void cribbit_set_alloc(cribbit_alloc_callback cb) {
    cribbit_alloc_cb = cb;
}

void cribbit_set_realloc(cribbit_realloc_callback cb) {
    cribbit_realloc_cb = cb;
}

void cribbit_set_clear(cribbit_clear_callback cb) {
    cribbit_clear_cb = cb;
}

void cribbit_set_free(cribbit_free_callback cb) {
    cribbit_free_cb = cb;
}

void cribbit_set_log(cribbit_log_calblack cb) {
    cribbit_log_cb = cb;
}

void* cribbit_alloc_linked(void* addr, size_t size) {
    struct cribbit_linked_shim* entry = (struct cribbit_linked_shim*) addr;
    struct cribbit_linked_shim* new_addr = cribbit_alloc(1, size);
    if(new_addr == NULL) {
        return NULL;
    }

    new_addr->next = NULL;

    if(addr != NULL) {
        if (entry->next != NULL) {
            new_addr->next = entry->next;
        }

        entry->next = new_addr;
    }

    return new_addr;
}

void cribbit_free_linked(void* addr) {
    struct cribbit_linked_shim* entry = (struct cribbit_linked_shim*) addr;
    while (entry != NULL) {
        struct cribbit_linked_shim* current = entry;
        entry = entry->next;
        cribbit_free(current);
    }
}

void* cribbit_skip_linked(void* addr, size_t count) {
    struct cribbit_linked_shim* entry = (struct cribbit_linked_shim*) addr;
    while(entry != NULL && count > 0) {
        entry = entry->next;
        count--;
    }

    return entry;
}

void cribbit_iterate_linked(void* addr, cribbit_linked_callback cb, void* userdata) {
    struct cribbit_linked_shim* entry = (struct cribbit_linked_shim*) addr;
    while(entry != NULL) {
        cb(entry, userdata);
        entry = entry->next;
    }
}

void* cribbit_unlink(void* addr) {
    void* next = ((struct cribbit_linked_shim*) addr)->next;
    cribbit_free(addr);
    return next;
}
