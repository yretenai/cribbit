//
// Created by Chronovore on 7/16/2023.
//

#ifndef CRIBBIT_IO_H
#define CRIBBIT_IO_H

#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
#include <stdio.h>
#define cribbit_open(handle, path, flags) _sopen_s(&handle, path, flags, 0, 0)
#define cribbit_seek(handle, offset, whence) _lseek(handle, offset, whence)
#define cribbit_read(handle, buf, size) ((size_t) _read(handle, buf, size))
#define cribbit_close _close
#else
#include <unistd.h>
#define cribbit_open(handle, path, flags) handle = open(path, flags)
#define cribbit_seek(handle, offset, whence) lseek(handle, offset, whence)
#define cribbit_read(handle, buf, size) read(handle, buf, size)
#define cribbit_close close
#endif

#endif //CRIBBIT_IO_H
