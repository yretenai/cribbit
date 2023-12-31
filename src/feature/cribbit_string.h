// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_STRING_H
#define CRIBBIT_STRING_H

#define __USE_GNU
#include <string.h>

#ifdef _WIN32
#define strtok_r strtok_s
#define strcasecmp _stricmp
#else
#include <strings.h>
#define _strdup strdup
#endif

#endif //CRIBBIT_STRING_H
