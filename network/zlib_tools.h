//
// Created by protobit on 18/10/2020.
//

#ifndef EPISIM_ZLIB_TOOLS_H
#define EPISIM_ZLIB_TOOLS_H

#include "../libs/zlib/zlib.h"
#include "cstring"
#include "cassert"
#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 262144

#define setupDeflate

// Defined in zlib_tools.cpp
extern int deflateSerialData(char* data, char* dest, int lvl, int len);
extern int inflateSerialData(char* data, char* dest, int lvl, int len);
extern int deflateSerialData(char* data, int lvl, int len);
extern int inflateSerialData(char* data, int lvl, int len);

#endif //EPISIM_ZLIB_TOOLS_H
