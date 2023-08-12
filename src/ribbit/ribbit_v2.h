// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_RIBBIT_V2_H
#define CRIBBIT_RIBBIT_V2_H

#include <cribbit/ribbit/ribbit_client.h>
#include "../tact/tact_pipe_internal.h"

ribbit_response ribbit_v2_fetch(const char* host, size_t host_len, int32_t port, ribbit_response_type type, const char* param);

#endif //CRIBBIT_RIBBIT_V2_H
