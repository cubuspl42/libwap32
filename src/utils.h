#ifndef utils_h
#define utils_h

#include "common.h"
#include <cassert>
#include <cstdio>
#include <vector>

template<typename... Args>
void wap_util_buffer_printf(char *buffer, size_t buffer_size, const char *format, Args... args)
{
    size_t bytes_written = snprintf(buffer, buffer_size, format, args...);
    assert(bytes_written < buffer_size);
}

template<size_t buffer_size, typename... Args>
void wap_util_buffer_printf(char (&buffer)[buffer_size], const char *format, Args... args)
{
    wap_util_buffer_printf(buffer, buffer_size, format, args...);
}

unsigned wap_util_checksum(const char *buffer, size_t buffer_size);

#endif
