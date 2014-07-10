#ifndef wwd_read_h
#define wwd_read_h

#include "wwd.h"
#include <vector>

// This function throws Wap32Exception on error.
void wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer);

#endif
