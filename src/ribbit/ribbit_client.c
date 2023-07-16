//
// Created by chronovore on 7/15/23.
//

#include <string.h>

#include <cribbit/cribbit.h>
#include <cribbit/ribbit/ribbit_client.h>

#include "ribbit_v1.h"
#include "ribbit_v2.h"

const char RIBBIT_HOST[20] = ".version.battle.net";
const int32_t RIBBIT_PORT = 1119;

ribbit_response ribbit_fetch(ribbit_region region, ribbit_version version, ribbit_response_type type, const char* param) {
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
            ribbit_response response = {0};
            return response;
        }
    }

    memcpy(&host[2], RIBBIT_HOST, sizeof(RIBBIT_HOST));

    return ribbit_fetch_direct((const char *) &host, RIBBIT_PORT, version, type, param);
}

ribbit_response ribbit_fetch_direct(const char* host, int32_t port, ribbit_version version, ribbit_response_type type, const char* param) {
    switch (version) {
        case RIBBIT_CLIENT_V1:
            return ribbit_v1_fetch(host, port, type, param);
        case RIBBIT_CLIENT_V2:
            return ribbit_v2_fetch(host, port, type, param);
        default:
        case RIBBIT_CLIENT_INVALID:
        case RIBBIT_CLIENT_MAX: {
            ribbit_response response = {0};
            return response;
        }
    }
}

bool ribbit_verify_oscp(ribbit_response* resp) {
    return false; // todo.
}

void ribbit_free(ribbit_response* response) {
    tact_pipe_free(&response->data);

    if (response->type == RIBBIT_RESPONSE_INVALID) {
        return;
    }

    cribbit_free_linked((struct cribbit_linked_shim *) response->mime);
    response->mime = NULL;

    if (response->cert != NULL) {
        cribbit_free(response->cert);
        response->cert = NULL;
    }

    response->type = RIBBIT_RESPONSE_INVALID;
}
