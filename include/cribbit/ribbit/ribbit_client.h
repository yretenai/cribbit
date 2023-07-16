//
// Created by chronovore on 7/15/23.
//

#ifndef CRIBBIT_RIBBIT_CLIENT_H
#define CRIBBIT_RIBBIT_CLIENT_H

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

#include <cribbit/tact/tact_pipe_file.h>

typedef enum RIBBIT_CLIENT_VERSION {
    RIBBIT_CLIENT_INVALID = 0,

    RIBBIT_CLIENT_V1,
    RIBBIT_CLIENT_V2,

    RIBBIT_CLIENT_MAX,
} ribbit_version;

typedef enum RIBBIT_RESPONSE_TYPE {
    RIBBIT_RESPONSE_INVALID,

    RIBBIT_RESPONSE_SUMMARY,
    RIBBIT_RESPONSE_VERSIONS,
    RIBBIT_RESPONSE_CDNS,
    RIBBIT_RESPONSE_BGDL,
    RIBBIT_RESPONSE_CERT,
    RIBBIT_RESPONSE_OCSP,

    RIBBIT_RESPONSE_MAX,
} ribbit_response_type;

typedef enum RIBBIT_REGION {
    RIBBIT_REGION_INVALID,

    RIBBIT_REGION_US,
    RIBBIT_REGION_EU,

    RIBBIT_REGION_MAX,
} ribbit_region;

typedef struct RIBBIT_MIME {
    struct RIBBIT_MIME* next;
    const char* key;
    const char* value;
} ribbit_mime;

typedef struct RIBBIT_RESPONSE {
    ribbit_response_type type;
    tact_pipe_file data;
    ribbit_mime* mime;
    uint8_t* cert;
    size_t cert_len;
} ribbit_response;

ribbit_response ribbit_fetch(ribbit_region region, ribbit_version version, ribbit_response_type type, const char* param);
ribbit_response ribbit_fetch_direct(const char* host, int32_t port, ribbit_version version, ribbit_response_type type, const char* param);
bool ribbit_verify_oscp(ribbit_response* resp);
void ribbit_free(ribbit_response* response);

#endif //CRIBBIT_RIBBIT_CLIENT_H
