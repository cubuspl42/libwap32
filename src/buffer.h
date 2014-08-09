#ifndef buffer_h
#define buffer_h

#include "common.h"
#include "wap32/buffer.h"
#include <vector>

static_assert(sizeof(wap_buffer) >= sizeof(std::vector<char>), "wap_buffer is not big enough to hold std::vector");

namespace wap {
    inline std::vector<char> *cast_wap_buffer_to_vector(wap_buffer *buffer) {
        return reinterpret_cast<std::vector<char>*>(buffer);
    }
    inline wap_buffer *cast_vector_to_wap_buffer(std::vector<char> *buffer) {
        return reinterpret_cast<wap_buffer*>(buffer);
    }
}

#endif