// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_CRIBBIT_H
#define CRIBBIT_CRIBBIT_H

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>

#include <cribbit/shared.h>

struct cribbit_linked_shim {
    void* next;
};

typedef void (*cribbit_linked_callback)(void *entry, void* userdata);

typedef void* (*cribbit_alloc_callback)(size_t elements, size_t size);
typedef void* (*cribbit_realloc_callback)(void* ptr, size_t size);
typedef void (*cribbit_clear_callback)(void* ptr, size_t size);
typedef void (*cribbit_free_callback)(void* ptr);
typedef int (*cribbit_log_calblack)(char const* const fmt, va_list args);

CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_alloc(size_t elements, size_t size);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_realloc(void* ptr, size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_clear(void* ptr, size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_free(void* ptr);
#ifdef _WIN32
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_log(_Printf_format_string_ const char* fmt, ...);
#else
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_log(const char* fmt, ...) __attribute__((format(printf, 1, 2)));
#endif

CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_libc(void);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_printf(void);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_alloc(cribbit_alloc_callback cb);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_realloc(cribbit_realloc_callback cb);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_clear(cribbit_clear_callback cb);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_free(cribbit_free_callback cb);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_set_log(cribbit_log_calblack cb);

CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_alloc_linked(void* addr, size_t size);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_free_linked(void* addr);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_skip_linked(void* addr, size_t count);
CRIBBIT_SHARED void CRIBBIT_DECL cribbit_iterate_linked(void* addr, cribbit_linked_callback cb, void* userdata);
CRIBBIT_SHARED void* CRIBBIT_DECL cribbit_unlink(void* addr);

#endif // CRIBBIT_CRIBBIT_H
