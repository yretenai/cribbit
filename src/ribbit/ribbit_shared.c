// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include <stdbool.h>
#include <stdio.h>

#include <cribbit/cribbit.h>
#include "ribbit_shared.h"
#include "../feature/cribbit_string.h"

#ifdef _WIN32
#include <winsock2.h>
#include <ws2def.h>
#include <ws2tcpip.h>
#define SOCKET_START {WSADATA _wsaData; WSAStartup(MAKEWORD(2, 2), &_wsaData);}
#define SOCKET_END WSACleanup()
#define SOCKET_TYPE SOCKET
#else
#define __USE_XOPEN2K
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#define SOCKET_START
#define SOCKET_END
#define SOCKET_TYPE int
#define INVALID_SOCKET -1
#define SOCKET_ERROR -1
#define closesocket close
#endif

#define BUFFER_SIZE 4096

int request_tcp_message(const char *host, int32_t port, const char *message, int32_t message_len, char **output) {
#if _WIN32
    SOCKET_START;
#endif

#ifndef _NDEBUG
    printf("cribbit: requesting tcp %s:%d/%s\n", host, port, message);
#endif

    *output = NULL;

    struct addrinfo *dns;
    struct addrinfo hints = {0};

    memset (&hints, 0, sizeof (hints));
    hints.ai_family = PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags |= AI_CANONNAME;

    int res = getaddrinfo(host, NULL, &hints, &dns);
    if(res != 0) {
        SOCKET_END;
        return -1;
    }

    SOCKET_TYPE sock = socket(dns->ai_family, dns->ai_socktype, dns->ai_protocol);
    if(sock == INVALID_SOCKET) {
        freeaddrinfo(dns);
        SOCKET_END;
        return -1;
    }

    ((struct sockaddr_in*) dns->ai_addr)->sin_port = htons(port);

    res = connect(sock, dns->ai_addr, (int32_t) dns->ai_addrlen);
    freeaddrinfo(dns);

    if(res == SOCKET_ERROR) {
        closesocket(sock);
        SOCKET_END;
        return -1;
    }

    if(send(sock, message, message_len, 0) == SOCKET_ERROR) {
        closesocket(sock);
        SOCKET_END;
        return -1;
    }

    if(send(sock, "\r\n", sizeof("\r\n"), 0) == SOCKET_ERROR) {
        closesocket(sock);
        SOCKET_END;
        return -1;
    }

    char* resp = NULL;
    char buffer[BUFFER_SIZE];
    int resp_size = 0;

    while(true) {
        int size = recv(sock, buffer, sizeof(buffer), 0);
        if(size <= 0) {
            break;
        }

        char* tmp = cribbit_realloc(resp, resp_size + size);
        if(tmp == NULL) {
            cribbit_free(resp);
            resp = NULL;
            break;
        }

        resp = tmp;

        memcpy(resp + resp_size, buffer, size);
        resp_size += size;
    }

    closesocket(sock);
    SOCKET_END;

    if(resp == NULL) {
        return -1;
    }

    *output = resp;
    return resp_size;
}
