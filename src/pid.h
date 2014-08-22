#ifndef pid_h
#define pid_h

#include "common.h"
#include "wap32/pid.h"

#include <array>
#include <memory>
#include <vector>

struct wap_pid {
    wap_pid_header header;
    std::vector<char> pixels;
    std::unique_ptr<std::array<wap_rgb, 256>> palette;
};

#endif
