//
// Created by chronovore on 7/15/23.
//

#ifndef CRIBBIT_TACT_PIPE_FILE_H
#define CRIBBIT_TACT_PIPE_FILE_H

#include <stdint.h>
#include <stdlib.h>

#include <cribbit/shared.h>

typedef enum TACT_PIPE_COLUMN_TYPE {
    TACT_PIPE_COLUMN_INVALID,

    TACT_PIPE_COLUMN_STRING,
    TACT_PIPE_COLUMN_HEX,
    TACT_PIPE_COLUMN_DEC,

    TACT_PIPE_COLUMN_MAX,
} tact_pipe_column_type;

extern CRIBBIT_SHARED const char* TACT_PIPE_COLUMN_NAME[TACT_PIPE_COLUMN_MAX + 1];

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

CRIBBIT_SHARED tact_pipe_file CRIBBIT_DECL tact_pipe_parse(char* data);
CRIBBIT_SHARED tact_pipe_column_type CRIBBIT_DECL tact_pipe_get(tact_pipe_file* file, const char* column, size_t row, void** data, size_t* data_len);
CRIBBIT_SHARED tact_pipe_column_type CRIBBIT_DECL tact_pipe_get_idx(tact_pipe_file* file, size_t column, size_t row, void** data, size_t* data_len);
CRIBBIT_SHARED tact_pipe_column_type CRIBBIT_DECL tact_pipe_convert(size_t column, tact_pipe_column* column_entry, tact_pipe_row* row_entry, void** data, size_t* data_len);
CRIBBIT_SHARED void CRIBBIT_DECL tact_pipe_free_value(void* data);
CRIBBIT_SHARED void CRIBBIT_DECL tact_pipe_free(tact_pipe_file* file);

#endif //CRIBBIT_TACT_PIPE_FILE_H
