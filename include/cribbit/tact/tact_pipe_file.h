//
// Created by chronovore on 7/15/23.
//

#ifndef CRIBBIT_TACT_PIPE_FILE_H
#define CRIBBIT_TACT_PIPE_FILE_H

#include <stdint.h>
#include <stdlib.h>

typedef enum TACT_PIPE_COLUMN_TYPE {
    TACT_PIPE_COLUMN_INVALID,

    TACT_PIPE_COLUMN_STRING,
    TACT_PIPE_COLUMN_HEX,
    TACT_PIPE_COLUMN_DEC,

    TACT_PIPE_COLUMN_MAX,
} tact_pipe_column_type;

typedef struct TACT_PIPE_COLUMN {
    struct TACT_PIPE_COLUMN* next;
    tact_pipe_column_type type;
    int32_t width;
    const char* name;
} tact_pipe_column;

typedef struct TACT_PIPE_ROW {
    struct TACT_PIPE_ROW* next;
    char** columns; // off_t :modcheck:
} tact_pipe_row;

typedef struct TACT_PIPE_FILE {
    char* storage;
    size_t column_count;
    tact_pipe_column* columns;
    tact_pipe_row* rows;
    int64_t seqn;
} tact_pipe_file;

tact_pipe_file tact_pipe_parse(char* data);
tact_pipe_column_type tact_pipe_get(tact_pipe_file* file, const char* column, int32_t row, void** data, size_t* data_len);
tact_pipe_column_type tact_pipe_get_idx(tact_pipe_file* file, int32_t column, int32_t row, void** data, size_t* data_len);
void tact_pipe_free_value(void* data);
void tact_pipe_free(tact_pipe_file* file);

#endif //CRIBBIT_TACT_PIPE_FILE_H
