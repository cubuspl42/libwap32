#include "utils.h"
#include "errors.h"
#include <iostream>
#include <zlib.h>

int wap_util_inflate(char *out_buffer, size_t out_buffer_size,
                       const char *in_buffer, size_t in_buffer_size)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = in_buffer_size;
    strm.next_in = (unsigned char*)in_buffer;
    strm.avail_out = out_buffer_size;
    strm.next_out = (unsigned char*)out_buffer;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK)
        return WAP_ERROR;
    ret = inflate(&strm, Z_FINISH);
    (void)inflateEnd(&strm);

    return ret == Z_STREAM_END ? WAP_OK : WAP_ERROR;
}

unsigned wap_util_checksum(const char *buffer, size_t buffer_size)
{
    unsigned checksum = UINT32_MAX - buffer_size - 159;
    for(size_t i = 0; i < buffer_size; ++i) {
        unsigned delta = (i - (unsigned char)buffer[i]);
        checksum -= delta;
    }
    return checksum;
}
