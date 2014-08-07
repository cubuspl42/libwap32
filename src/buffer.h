#ifndef buffer_h
#define buffer_h

#include "common.h"
#include "wap32/buffer.h"
#include <vector>

static_assert(sizeof(wap_buffer) >= sizeof(std::vector<char>), "wap_buffer is not big enough to hold std::vector");

std::vector<char> &wap_buffer__vector(wap_buffer *buffer);

#endif