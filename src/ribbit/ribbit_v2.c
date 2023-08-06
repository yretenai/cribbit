// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include <stdio.h>

#include <cribbit/cribbit.h>
#include <cribbit/tact/tact_pipe_file.h>
#include "ribbit_v2.h"
#include "ribbit_shared.h"
#include "../feature/cribbit_string.h"


#define V2_SUMMARY "v2/summary"
#define V2_VERSIONS "v2/products/%s/versions"
#define V2_CDNS "v2/products/%s/cdns"
#define V2_BGDL "v2/products/%s/bgdl"

#define PARAM_BUFFER 128

ribbit_response ribbit_v2_fetch(const char* host, size_t host_len, int32_t port, ribbit_response_type type, const char* param) {
    ribbit_response response = {0};

    const char* message = NULL;
    char param_message[PARAM_BUFFER];
    int32_t message_len;

    if(type == RIBBIT_RESPONSE_SUMMARY) {
        message = V2_SUMMARY;
        message_len = sizeof(V2_SUMMARY);
    } else {
        if(param == NULL) {
            return response;
        }
        message = param_message;

        switch (type) {
            case RIBBIT_RESPONSE_VERSIONS:
                message_len = snprintf(param_message, sizeof(param_message), V2_VERSIONS, param);
                break;
            case RIBBIT_RESPONSE_CDNS:
                message_len = snprintf(param_message, sizeof(param_message), V2_CDNS, param);
                break;
            case RIBBIT_RESPONSE_BGDL:
                message_len = snprintf(param_message, sizeof(param_message), V2_BGDL, param);
                break;
            default:
            case RIBBIT_RESPONSE_CERT:
            case RIBBIT_RESPONSE_OCSP:
            case RIBBIT_RESPONSE_INVALID:
            case RIBBIT_RESPONSE_MAX:
                return response;
        }
    }

    if(message == NULL) {
        return response;
    }

    char* data;
    if(request_tcp_message(host, port, message, message_len, &data) == -1) {
        return response;
    }

    response.host = cribbit_alloc(host_len, sizeof(char));
    memcpy((void*) response.host, host, host_len);
    response.port = port;
    response.version = RIBBIT_CLIENT_V2;

    response.type = type;
    response.storage = data;
    response.data = tact_pipe_parse(data);

    return response;
}
