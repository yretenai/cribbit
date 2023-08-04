// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include <cribbit/cribbit.h>
#include <cribbit/tact/tact_pipe_file.h>

#include "../feature/string.h"

const char CS_SEQN[] = "## seqn = ";

const char NAME_TYPE_SEPARATOR = '!';
const char TYPE_WIDTH_SEPARATOR = ':';
const char COLUMN_SEPARATOR = '|';
const char ABORT_SEPARATOR = '#';

const char NEWLINE[3] = "\r\n";

const char* TACT_PIPE_COLUMN_NAME[] = {
        "INVALID",
        "STRING",
        "HEX",
        "DEC",
        "MAX"
};

tact_pipe_file tact_pipe_parse(char* data) {
    tact_pipe_file file = {0};

    file.columns = cribbit_alloc_linked(NULL, sizeof(tact_pipe_column));
    file.rows = cribbit_alloc_linked(NULL, sizeof(tact_pipe_row));

    bool has_columns = false;

    char* next_line = NULL;
    char* line = strtok_r(data, NEWLINE, &next_line);
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
            char* chunk = strchr(line, COLUMN_SEPARATOR);
            if(chunk != NULL) {
                chunk = line;

                if (has_columns) {
                    current_row = cribbit_alloc_linked(current_row, sizeof(tact_pipe_row));
                    current_row->columns = cribbit_alloc(sizeof(char*) * file.column_count);
                }

                int idx = 0;
                while (chunk != NULL && chunk[0] != 0) {
                    if (has_columns) {
                        current_row->columns[idx++] = chunk;

                        if(idx >= (int) file.column_count) {
                            break;
                        }

                        chunk = strchr(chunk, COLUMN_SEPARATOR);
                        if(chunk != NULL) {
                            chunk[0] = 0;
                            chunk += 1;
                        }
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
                        name_sep++;
                        *width_sep = 0;
                        width_sep++;

                        file.column_count++;

                        current_column = cribbit_alloc_linked(current_column, sizeof(tact_pipe_column));
                        current_column->name = chunk;
                        current_column->width = (int32_t) strtoll(width_sep, NULL, 10);

                        chunk = strchr(width_sep, COLUMN_SEPARATOR);
                        if(chunk != NULL) {
                            chunk[0] = 0;
                            chunk += 1;
                        }

                        if(stricmp(name_sep, TACT_PIPE_COLUMN_NAME[TACT_PIPE_COLUMN_STRING]) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_STRING;
                        } else if(stricmp(name_sep, TACT_PIPE_COLUMN_NAME[TACT_PIPE_COLUMN_HEX]) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_HEX;
                        } else if(stricmp(name_sep, TACT_PIPE_COLUMN_NAME[TACT_PIPE_COLUMN_DEC]) == 0) {
                            current_column->type = TACT_PIPE_COLUMN_DEC;
                        }  else {
                            current_column->type = TACT_PIPE_COLUMN_INVALID;
                        }
                        idx = -1;
                    }
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

tact_pipe_column_type tact_pipe_get(tact_pipe_file* file, const char* column, size_t row, void** data, size_t* data_len) {
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

uint8_t parse_octet(const char value) {
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

tact_pipe_column_type tact_pipe_get_idx(tact_pipe_file* file, size_t column, size_t row, void** data, size_t* data_len) {
    if (data == NULL || data_len == NULL) {
        return TACT_PIPE_COLUMN_INVALID;
    }

    tact_pipe_column *column_entry = cribbit_skip_linked(file->columns, column);
    tact_pipe_row *row_entry = cribbit_skip_linked(file->rows, row);

    return tact_pipe_convert(column, column_entry, row_entry, data, data_len);
}

tact_pipe_column_type tact_pipe_convert(size_t column, tact_pipe_column* column_entry, tact_pipe_row* row_entry, void** data, size_t* data_len) {
    if(column_entry == NULL || row_entry == NULL) {
        *data = NULL;
        *data_len = 0;
        return TACT_PIPE_COLUMN_INVALID;
    }

    char* column_value = row_entry->columns[column];

    switch(column_entry->type) {
        case TACT_PIPE_COLUMN_STRING: {
            if(column_value != NULL) {
                *data_len = strlen(column_value);
            } else {
                *data_len = 0;
            }

            *data = column_value;
            break;
        }
        case TACT_PIPE_COLUMN_HEX: {
            if (column_entry->width == 0) {
                *data = NULL;
                *data_len = 0;
                return TACT_PIPE_COLUMN_INVALID;
            }

            uint8_t* hex = cribbit_alloc(sizeof(uint8_t) * column_entry->width);
            *data = hex;
            *data_len = column_entry->width;

            if(column_value != NULL) {
                int width = (int) strlen(column_value) >> 1;
                if(width > column_entry->width) {
                    width = column_entry->width;
                }

                for (int i = 0; i < width; ++i) {
                    hex[i] = (parse_octet(column_value[i << 1]) << 4) | parse_octet(column_value[(i << 1) + 1]);
                }
            }
            break;
        }
        case TACT_PIPE_COLUMN_DEC: {
            if (column_entry->width > 8) {
                *data = NULL;
                *data_len = 0;
                return TACT_PIPE_COLUMN_INVALID;
            }
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
    cribbit_free_linked((struct cribbit_linked_shim *) file->columns);
    file->columns = NULL;

    cribbit_free_linked((struct cribbit_linked_shim *) file->rows);
    file->rows = NULL;

    if(file->storage != NULL) {
        cribbit_free((void*) file->storage);
        file->storage = NULL;
    }
}
