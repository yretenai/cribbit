//
// Created by chronovore on 7/15/23.
//

#ifndef CRIBBIT_RIBBIT_V1_H
#define CRIBBIT_RIBBIT_V1_H

#include <cribbit/ribbit/ribbit_client.h>

ribbit_response ribbit_v1_fetch(char* host, size_t host_len, int32_t port, ribbit_response_type type, char* param, size_t param_len);


#endif //CRIBBIT_RIBBIT_V1_H
