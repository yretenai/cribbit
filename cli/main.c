//
// Created by chronovore on 7/13/2023.
//

#include <cribbit/tact/tact_pipe_file.h>

#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
    int handle = open("../../sample/v2_summary", O_RDONLY);
    size_t len = lseek(handle, 0, SEEK_END);
    lseek(handle, 0, SEEK_SET);
    char* data = malloc(len + 1);
    data[len] = 0;
    size_t data_len = len;

    char* ofs = data;
    do {
        size_t bytes = read(handle, ofs, len);
        len -= bytes;
        ofs += bytes;
    } while(len > 0);

    close(handle);
    tact_pipe_file file = tact_pipe_parse(data);
    tact_pipe_free(&file);
}
