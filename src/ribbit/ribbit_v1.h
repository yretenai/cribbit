// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_RIBBIT_V1_H
#define CRIBBIT_RIBBIT_V1_H

#include <cribbit/ribbit/ribbit_client.h>

ribbit_response ribbit_v1_fetch(const char* host, int32_t port, ribbit_response_type type, const char* param);

#endif //CRIBBIT_RIBBIT_V1_H
