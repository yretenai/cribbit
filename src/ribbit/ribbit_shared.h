// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_RIBBIT_SHARED_H
#define CRIBBIT_RIBBIT_SHARED_H

#include <stdint.h>
#include <stdbool.h>

int request_tcp_message(const char *host, int32_t port, const char *message, int32_t message_len, char **output);

#endif
