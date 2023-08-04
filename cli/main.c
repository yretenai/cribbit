// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#include <cribbit/ribbit/ribbit_client.h>
#include <cribbit/cribbit.h>

#include <stdio.h>

void print_column(void *entry) {
    tact_pipe_column* column = entry;
    printf("%s is %s (%d)\n", column->name, TACT_PIPE_COLUMN_NAME[column->type], column->width);
}

void print_row(void* entry, void* userdata) {
    tact_pipe_row* row = entry;
    tact_pipe_file* file = userdata;
    for(size_t i = 0; i < file->column_count; ++i) {
        tact_pipe_column *column_entry = cribbit_skip_linked(file->columns, i);
        void* data;
        size_t data_len;
        tact_pipe_column_type type = tact_pipe_convert(i, column_entry, row, &data, &data_len);
        if(data != NULL) {
            switch (type) {
                case TACT_PIPE_COLUMN_STRING:
                    printf("%s ", (char *) data);
                    break;
                case TACT_PIPE_COLUMN_HEX:
                    for (size_t j = 0; j < data_len; ++j) {
                        printf("%02x", ((uint8_t *) data)[j]);
                    }
                    printf(" ");
                    break;
                case TACT_PIPE_COLUMN_DEC:
                    printf("%lld ", (int64_t) data);
                    break;

                default:
                case TACT_PIPE_COLUMN_INVALID:
                case TACT_PIPE_COLUMN_MAX:
                    break;
            }

            if (type == TACT_PIPE_COLUMN_HEX) {
                cribbit_free(data);
            }
        } else {
            printf("<null> ");
        }
    }
    printf("\n");
}

void print_tact(tact_pipe_file file) {
    printf("Seqn: %lld\n", file.seqn);
    printf("Columns:\n");
    cribbit_iterate_linked(file.columns, (cribbit_linked_callback) print_column, &file);
    printf("Rows:\n");
    cribbit_iterate_linked(file.rows, (cribbit_linked_callback) print_row, &file);
}

int main(void) {
    ribbit_response resp = ribbit_fetch(RIBBIT_REGION_US, RIBBIT_CLIENT_V2, RIBBIT_RESPONSE_VERSIONS, "wow");

    print_tact(resp.data);

    ribbit_free(&resp);
}
