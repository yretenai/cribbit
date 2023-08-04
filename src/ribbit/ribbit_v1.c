// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include "ribbit_v1.h"

#include "ribbit_shared.h"
#include <cribbit/cribbit.h>

#define V1_SUMMARY "v1/summary"
#define V1_VERSIONS "v1/products/%s/versions"
#define V1_CDNS "v1/products/%s/cdns"
#define V1_BGDL "v1/products/%s/bgdl"
#define V1_CERT "v1/certs/%s"
#define V1_OSCP "v1/oscp/%s"

#define PARAM_BUFFER 128

#include <stdio.h>
#include <string.h>

ribbit_response ribbit_v1_fetch(const char* host, size_t host_len, int32_t port, ribbit_response_type type, const char* param) {
    ribbit_response response = {0};

    const char* message = NULL;
    char param_message[PARAM_BUFFER];
    int32_t message_len;

    if(type == RIBBIT_RESPONSE_SUMMARY) {
        message = V1_SUMMARY;
        message_len = sizeof(V1_SUMMARY);
    } else {
        if(param == NULL) {
            return response;
        }
        message = param_message;

        switch (type) {
            case RIBBIT_RESPONSE_VERSIONS:
                message_len = snprintf(param_message, sizeof(param_message), V1_VERSIONS, param);
                break;
            case RIBBIT_RESPONSE_CDNS:
                message_len = snprintf(param_message, sizeof(param_message), V1_CDNS, param);
                break;
            case RIBBIT_RESPONSE_BGDL:
                message_len = snprintf(param_message, sizeof(param_message), V1_BGDL, param);
                break;
            case RIBBIT_RESPONSE_CERT:
                message_len = snprintf(param_message, sizeof(param_message), V1_CERT, param);
                break;
            case RIBBIT_RESPONSE_OCSP:
                message_len = snprintf(param_message, sizeof(param_message), V1_OSCP, param);
                break;

            default:
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

    response.host = cribbit_alloc(host_len);
    memcpy((void*) response.host, host, host_len);
    response.port = port;
    response.version = RIBBIT_CLIENT_V1;

    response.type = type;
    response.storage = data;

    // todo: process MIME
    // todo: pass TACT pipe
    // todo: parse CERT

    return response;
}
