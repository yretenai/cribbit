//
// Created by chronovore on 7/15/23.
//

#ifndef CRIBBIT_RIBBIT_V2_H
#define CRIBBIT_RIBBIT_V2_H

#include <cribbit/ribbit/ribbit_client.h>

ribbit_response ribbit_v2_fetch(const char* host, int32_t port, ribbit_response_type type, const char* param);

#endif //CRIBBIT_RIBBIT_V2_H
