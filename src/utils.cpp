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
    if (ret != Z_OK) {
        inflateEnd(&strm);
        return WAP_ERROR;
    }
    ret = inflate(&strm, Z_FINISH);
    inflateEnd(&strm);
    return ret == Z_STREAM_END ? WAP_OK : WAP_ERROR;
}

int wap_util_deflate(std::vector<char> &out_buffer, const char *in_buffer, size_t in_buffer_size)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = in_buffer_size;
    strm.next_in = (unsigned char*)in_buffer;
    strm.avail_out = 0;
    strm.next_out = Z_NULL;
    
    int ret = deflateInit(&strm, 9);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        return WAP_ERROR;
    }
    
    out_buffer.resize(0);
    
    do {
        constexpr size_t step = 32 * 1024; // 32 kB
        out_buffer.resize(out_buffer.size() + step);
        strm.avail_out = out_buffer.capacity() - strm.total_out;
        strm.next_out = (unsigned char*)out_buffer.data() + strm.total_out;
        ret = deflate(&strm, Z_FINISH);
    } while (ret != Z_STREAM_END);
    
    out_buffer.resize(out_buffer.capacity() - strm.avail_out);
    
    deflateEnd(&strm);
    return WAP_OK;
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
