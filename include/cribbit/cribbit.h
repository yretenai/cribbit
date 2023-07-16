//
// Created by chronovore on 7/13/2023.
//

#ifndef CRIBBIT_CRIBBIT_H
#define CRIBBIT_CRIBBIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>

struct cribbit_linked_shim {
    void* next;
};

void* cribbit_alloc(size_t size);
void cribbit_free(void* ptr);
void* cribbit_alloc_linked(void* addr, size_t size);
void cribbit_free_linked(void* addr);
void* cribbit_skip_linked(void* addr, int32_t count);
void* cribbit_unlink(void* addr);

#endif // CRIBBIT_CRIBBIT_H
