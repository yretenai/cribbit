//
// Created by chronovore on 7/16/23.
//

#ifndef CRIBBIT_CRIBBIT_EMPTY_H
#define CRIBBIT_CRIBBIT_EMPTY_H

#include <cribbit/ribbit/ribbit_client.h>
#include <cribbit/tact/tact_pipe_file.h>

#define CRIBBIT_EMPTY_TACT_PIPE_FILE {NULL, 0, NULL, NULL, 0}
#define CRIBBIT_EMPTY_RIBBIT_RESPONSE {RIBBIT_RESPONSE_INVALID, CRIBBIT_EMPTY_TACT_PIPE_FILE, NULL, NULL, 0}

#endif //CRIBBIT_CRIBBIT_EMPTY_H
