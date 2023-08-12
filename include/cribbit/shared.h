// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_SHARED_H
#define CRIBBIT_SHARED_H

#ifdef _WIN32
#define CRIBBIT_DECL __cdecl
#ifdef CRIBBIT_EXPORTING
#define CRIBBIT_SHARED __declspec(dllexport) __attribute__((unused))
#else
#define CRIBBIT_SHARED __declspec(dllimport) __attribute__((unused))
#endif
#else
#define CRIBBIT_DECL
#define CRIBBIT_SHARED __attribute__ ((visibility ("default"))) __attribute__((unused))
#endif

#endif //CRIBBIT_SHARED_H
