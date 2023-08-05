// cribbit: interopability with ribbit et al
// Copyright (c) 2023 <https://github.com/yretenai/cribbit>
// SPDX-License-Identifier: MPL-2.0

#ifndef CRIBBIT_IO_H
#define CRIBBIT_IO_H

#include <fcntl.h>

#ifdef _WIN32
#include <io.h>
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

#endif //CRIBBIT_IO_H
