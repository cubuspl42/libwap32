#ifndef utils_h
#define utils_h

#include "common.h"
#include <cassert>
#include <cstdio>
#include <vector>

namespace wap {
inline bool system_is_big_endian() {
    int n = 1;
    return *(char *)&n != 1;
}
}

#endif
