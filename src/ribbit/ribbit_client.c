//
// Created by chronovore on 7/15/23.
//

#include <cribbit/ribbit/ribbit_client.h>

#include <stdlib.h>
#include <string.h>

#include "ribbit_v1.h"
#include "ribbit_v2.h"

const char RIBBIT_HOST[19] = ".version.battle.net";

ribbit_response ribbit_fetch(ribbit_region region, ribbit_version version, ribbit_response_type type, char* param, size_t param_len) {
    char host[2 + sizeof(RIBBIT_HOST)];

    switch (region) {
        case RIBBIT_REGION_US:
            host[0] = 'u';
            host[1] = 's';
            break;
        case RIBBIT_REGION_EU: {
            host[0] = 'e';
            host[1] = 'u';
            break;
        }
        default:
        case RIBBIT_REGION_INVALID:
        case RIBBIT_REGION_MAX: {
            ribbit_response response = {RIBBIT_RESPONSE_INVALID, 0, NULL, NULL, NULL, 0};
            return response;
        }
    }

    memcpy((char*) &host[2], RIBBIT_HOST, sizeof(RIBBIT_HOST));

    return ribbit_fetch_direct((char*) &host, sizeof(host), 1119, version, type, param, param_len);
}

ribbit_response ribbit_fetch_direct(char* host, size_t host_len, int32_t port, ribbit_version version, ribbit_response_type type, char* param, size_t param_len) {
    switch (version) {
        case RIBBIT_CLIENT_V1:
            return ribbit_v1_fetch(host, host_len, port, type, param, param_len);
        case RIBBIT_CLIENT_V2:
            return ribbit_v2_fetch(host, host_len, port, type, param, param_len);
        default:
        case RIBBIT_CLIENT_INVALID:
        case RIBBIT_CLIENT_MAX: {
            ribbit_response response = {RIBBIT_RESPONSE_INVALID, 0, NULL, NULL, NULL, 0};
            return response;
        }
    }
}

bool ribbit_verify_oscp(ribbit_response resp) {
    return false; // todo.
}

void ribbit_free(ribbit_response* response) {
    if (response->type == RIBBIT_RESPONSE_INVALID) {
        return;
    }

    ribbit_mime* mime = response->mime;
    while (mime != NULL) {
        void* current = mime;
        mime = mime->next;
        free(current);
    }
    response->mime = NULL;

    if (response->cert != NULL) {
        free(response->cert);
        response->cert = NULL;
    }

    if (response->storage != NULL) {
        free(response->storage);
        response->storage = NULL;
    }

    response->type = RIBBIT_RESPONSE_INVALID;
}
