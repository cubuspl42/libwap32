#include "utils.h"
#include "errors.h"
#include <iostream>

unsigned wap_util_checksum(const char *buffer, size_t buffer_size)
{
    unsigned checksum = UINT32_MAX - buffer_size - 159;
    for(size_t i = 0; i < buffer_size; ++i) {
        unsigned delta = (i - (unsigned char)buffer[i]);
        checksum -= delta;
    }
    return checksum;
}
