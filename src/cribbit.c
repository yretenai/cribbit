//
// Created by chronovore on 7/16/23.
//

#include <cribbit/cribbit.h>

#include <string.h>

void* cribbit_alloc(size_t size) {
    // todo: make this configurable.
    return malloc(size);
}

void cribbit_clear(void* ptr, size_t size) {
    memset(ptr, 0, size);
}

void cribbit_free(void* ptr) {
    // todo: make this configurable.
    free(ptr);
}

void* cribbit_alloc_linked(void* addr, size_t size) {
    struct cribbit_linked_shim* entry = (struct cribbit_linked_shim*) addr;
    struct cribbit_linked_shim* new_addr = cribbit_alloc(size);
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
