//
// Created by chronovore on 7/13/2023.
//

#ifndef CRIBBIT_CRIBBIT_H
#define CRIBBIT_CRIBBIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

#include <cribbit/shared.h>

struct cribbit_linked_shim {
    void* next;
};


typedef void (*cribbit_linked_callback)(void *entry, void* userdata);

CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_alloc(size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_clear(void* ptr, size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_free(void* ptr);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_alloc_linked(void* addr, size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_free_linked(void* addr);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_skip_linked(void* addr, size_t count);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_iterate_linked(void* addr, cribbit_linked_callback cb, void* userdata);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_unlink(void* addr);

#endif // CRIBBIT_CRIBBIT_H
