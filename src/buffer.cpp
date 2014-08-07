#include "buffer.h"
#include "wap32/errors.h"
#include <stdlib.h>

void wap_buffer_init(wap_buffer *buffer)
{
    new (buffer->_private) std::vector<char>;
}

char *wap_buffer_data(wap_buffer *buffer)
{
    return wap_buffer__vector(buffer).data();
}

size_t wap_buffer_size(wap_buffer *buffer)
{
    return wap_buffer__vector(buffer).size();
}

void wap_buffer_destroy(wap_buffer *buffer)
{
    wap_buffer__vector(buffer).~vector();
}

std::vector<char> &wap_buffer__vector(wap_buffer *buffer)
{
    return *reinterpret_cast<std::vector<char>*>(buffer->_private);
}
