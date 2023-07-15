//
// Created by chronovore on 7/13/2023.
//

#include <cribbit/ribbit/ribbit_client.h>

int main(int argc, char** argv) {
    ribbit_response resp = ribbit_fetch(RIBBIT_REGION_US, RIBBIT_CLIENT_V2, RIBBIT_RESPONSE_SUMMARY, NULL, 0);
    ribbit_free(&resp);
}
