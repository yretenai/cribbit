//
// Created by chronovore on 7/13/2023.
//

#include <cribbit/tact/tact_pipe_file.h>
#include <cribbit/cribbit.h>

#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#define cribbit_open(handle, path, flags) _sopen_s(&handle, path, flags, SH_DENYNO, 0)
#define cribbit_seek(handle, offset, whence) _lseek(handle, offset, whence)
#define cribbit_read(handle, buf, size) ((size_t) _read(handle, buf, size))
#define cribbit_close _close
#else
#include <unistd.h>
#define cribbit_open(handle, path, flags) handle = open(path, flags)
#define cribbit_seek(handle, offset, whence) lseek(handle, offset, whence)
#define cribbit_read(handle, buf, size) read(handle, buf, size)
#define cribbit_close close
#define O_TEXT 0
#endif

void print_column(tact_pipe_column* column, tact_pipe_file* file) {
    printf("%s is %s (%d)\n", column->name, TACT_PIPE_COLUMN_NAME[column->type], column->width);
}

void print_row(tact_pipe_row* row, tact_pipe_file* file) {
    for(size_t i = 0; i < file->column_count; ++i) {
        tact_pipe_column *column_entry = cribbit_skip_linked(file->columns, i);
        void* data;
        size_t data_len;
        tact_pipe_column_type type = tact_pipe_convert(i, column_entry, row, &data, &data_len);
        printf("%lld ", (int64_t) data_len);
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

int main(int argc, char** argv) {
    int handle;
    cribbit_open(handle, "sample/v2_cdns", O_RDONLY | O_TEXT);
    size_t len = cribbit_seek(handle, 0, SEEK_END);
    cribbit_seek(handle, 0, SEEK_SET);
    char* data = malloc(len + 1);
    data[len] = 0;

    char* ofs = data;
    do {
        size_t bytes = cribbit_read(handle, ofs, len);
        len -= bytes;
        ofs += bytes;
    } while(len > 0);

    cribbit_close(handle);
    tact_pipe_file file = tact_pipe_parse(data);

    printf("Seqn: %lld\n", file.seqn);
    printf("Columns:\n");
    cribbit_iterate_linked(file.columns, (cribbit_linked_callback) print_column, &file);
    printf("Rows:\n");
    cribbit_iterate_linked(file.rows, (cribbit_linked_callback) print_row, &file);


    tact_pipe_free(&file);
}
