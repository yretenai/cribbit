//
// Created by chronovore on 7/15/23.
//

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <cribbit/cribbit.h>
#include <cribbit/tact/tact_pipe_file.h>

#include "../cribbit_empty.h"

const char CS_TACT_TYPE_STRING[] = "STRING";
const char CS_TACT_TYPE_HEX[] = "HEX";
const char CS_TACT_TYPE_DEC[] = "DEC";
const char CS_SEQN[] = "## seqn = ";

const char NAME_TYPE_SEPARATOR = '!';
const char TYPE_WIDTH_SEPARATOR = ':';
const char COLUMN_SEPARATOR[2] = "|";
const char ABORT_SEPARATOR = '#';

const char NEWLINE[3] = "\r\n";


tact_pipe_file tact_pipe_parse(char* data) {
    tact_pipe_file file = CRIBBIT_EMPTY_TACT_PIPE_FILE;

    file.columns = cribbit_alloc_linked(NULL, sizeof(tact_pipe_column));
    file.rows = cribbit_alloc_linked(NULL, sizeof(tact_pipe_row));

    bool has_columns = false;

    char* next_line = NULL;
    char* line = strtok_r(data, NEWLINE, &next_line); // PLEASE ADD STRTOK_S!!!
    tact_pipe_row* current_row = file.rows;
    tact_pipe_column* current_column = file.columns;
    while (line != NULL) {
        char* abort = strchr(line, ABORT_SEPARATOR);
        if (abort != NULL) {
            if (strstr(line, CS_SEQN) == line) {
                file.seqn = strtoll(line + sizeof(CS_SEQN), NULL, 10);
            }

            *abort = 0;
        }

        if (strlen(line) > 1) {
            char* next_chunk = NULL;
            char* chunk = strtok_r(line, COLUMN_SEPARATOR, &next_chunk);
            if(chunk != NULL) {
                if (has_columns) {
                    current_row = cribbit_alloc_linked(current_row, sizeof(tact_pipe_row));
                    current_row->columns = cribbit_alloc(sizeof(char*) * file.column_count);
                }

                int idx = 0;
                while (chunk != NULL) {
                    if(next_chunk != NULL && next_chunk[-1] == COLUMN_SEPARATOR[0]) {
                        next_chunk[-1] = 0;
                    }

                    if (has_columns) {
                        current_row->columns[idx++] = chunk;
                    } else {
                        char* name_sep = strchr(chunk, NAME_TYPE_SEPARATOR);
                        if(name_sep == NULL) {
                            // invalid.
                            break;
                        }
                        char* width_sep = strchr(name_sep, TYPE_WIDTH_SEPARATOR);
                        if(width_sep == NULL) {
                            // invalid.
                            break;
                        }
                        *name_sep = 0;
                        *width_sep = 0;

                        current_column = cribbit_alloc_linked(current_column, sizeof(tact_pipe_column));
                        file.column_count++;

                        current_column->name = chunk;
                        current_column->width = (int32_t) strtoll(width_sep + 1, NULL, 10);
                        if(strcmp(name_sep + 1, CS_TACT_TYPE_STRING) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_STRING;
                        }
                        if(strcmp(name_sep + 1, CS_TACT_TYPE_HEX) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_HEX;
                        }
                        if(strcmp(name_sep + 1, CS_TACT_TYPE_DEC) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_DEC;
                        }
                        idx = -1;
                    }
                    chunk = strtok_r(NULL, COLUMN_SEPARATOR, &next_chunk);
                }

                if(idx == -1) {
                    has_columns = true;
                }
            }
        }

        line = strtok_r(NULL, NEWLINE, &next_line);
        if (abort != NULL) {
            *abort = ABORT_SEPARATOR;
        }
    }

    file.columns = cribbit_unlink(file.columns);
    file.rows = cribbit_unlink(file.rows);
    return file;
}

tact_pipe_column_type tact_pipe_get(tact_pipe_file* file, const char* column, int32_t row, void** data, size_t* data_len) {
    if (data == NULL || data_len == NULL) {
        return TACT_PIPE_COLUMN_INVALID;
    }

    tact_pipe_column* column_entry = file->columns;
    int idx = 0;
    while(column_entry != NULL) {
        if(strstr(column_entry->name, column) == 0) {
            return tact_pipe_get_idx(file, idx, row, data, data_len);
        }

        column_entry = column_entry->next;
    }

    *data = NULL;
    *data_len = 0;
    return TACT_PIPE_COLUMN_INVALID;
}

uint8_t ParseOctet(const char value) {
    if (value >= '0' && value <= '9') {
        return value - '0';
    } else if (value >= 'A' && value <= 'F') {
        return 10 + value - 'A';
    } else if (value >= 'a' && value <= 'f') {
        return 10 + value - 'a';
    } else {
        return 0;
    }
}

tact_pipe_column_type tact_pipe_get_idx(tact_pipe_file* file, int32_t column, int32_t row, void** data, size_t* data_len) {
    if (data == NULL || data_len == NULL) {
        return TACT_PIPE_COLUMN_INVALID;
    }

    tact_pipe_column* column_entry = cribbit_skip_linked(file->columns, column);
    tact_pipe_row* row_entry = cribbit_skip_linked(file->rows, row);
    if(column_entry == NULL || row_entry == NULL) {
        *data = NULL;
        *data_len = 0;
        return TACT_PIPE_COLUMN_INVALID;
    }

    char* column_value = row_entry->columns[column];

    switch(column_entry->type) {
        case TACT_PIPE_COLUMN_STRING: {
            *data_len = strlen(column_value);
            *data = malloc((*data_len) + 1);
            memcpy(*data, column_value, *data_len);
            break;
        }
        case TACT_PIPE_COLUMN_HEX: {
            if (column_entry->width == 0 || ((uint32_t) column_entry->width << 1) > strlen(column_value)) {
                *data = NULL;
                *data_len = 0;
                return TACT_PIPE_COLUMN_INVALID;
            }

            *data = cribbit_alloc(sizeof(uint8_t) * column_entry->width);
            *data_len = column_entry->width;
            for(int i = 0; i < column_entry->width; ++i) {
                ((uint8_t*) data)[i] = (ParseOctet(column_value[i << 1]) << 4) | ParseOctet(column_value[(i << 1) + 1]);
            }
            break;
        }
        case TACT_PIPE_COLUMN_DEC: {
            if (column_entry->width > 8) {
                *data = NULL;
                *data_len = 0;
                return TACT_PIPE_COLUMN_INVALID;
            }
            *data = cribbit_alloc(sizeof(uint64_t));
            *data_len = column_entry->width;
            *data = (void *) strtoll(column_value, NULL, 10);
            break;
        }
        default:
        case TACT_PIPE_COLUMN_INVALID:
        case TACT_PIPE_COLUMN_MAX: // realistically unreachable.
            *data = NULL;
            *data_len = 0;
            return TACT_PIPE_COLUMN_INVALID;
    }

    return column_entry->type;
}

void tact_pipe_free_value(void* data) {
    cribbit_free(data);
}

void tact_pipe_free(tact_pipe_file* file) {
    if(file->storage != NULL) {
        cribbit_free(file->storage);
        file->storage = NULL;
    }

    cribbit_free_linked((struct cribbit_linked_shim *) file->columns);
    file->columns = NULL;

    cribbit_free_linked((struct cribbit_linked_shim *) file->rows);
    file->rows = NULL;
}
