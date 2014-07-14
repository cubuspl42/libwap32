#ifndef utils_h
#define utils_h

#include "common.h"
#include <cassert>
#include <cstdio>
#include <vector>

void wap32_util_buffer_assign(char *buffer, size_t size, const char *source);

template<size_t size>
void wap32_util_buffer_assign(char(&buffer)[size], const char *source)
{
	wap32_util_buffer_assign(buffer, size, source);
}

template<typename... Args>
void wap32_util_buffer_printf(char *buffer, size_t buffer_size, const char *format, Args... args)
{
    size_t bytes_written = snprintf(buffer, buffer_size, format, args...);
    assert(bytes_written < buffer_size);
}

template<size_t buffer_size, typename... Args>
void wap32_util_buffer_printf(char (&buffer)[buffer_size], const char *format, Args... args)
{
    wap32_util_buffer_printf(buffer, buffer_size, format, args...);
}

int wap32_util_inflate(char *out_buffer, size_t out_buffer_size,
                       const char *in_buffer, size_t in_buffer_size);
unsigned wap32_util_checksum(const char *buffer, size_t buffer_size);

#endif