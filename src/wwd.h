#ifndef wwd_h
#define wwd_h

#include "wap32/wwd.h"
#include "common.h"
#include <array>
#include <string>
#include <vector>

struct wap_object {
    wap_object_properties properties;
    std::string name;
    std::string logic;
    std::string image_set;
    std::string animation;
};

struct wap_plane {
    uint32_t tiles_wide = 0;
    uint32_t tiles_high = 0;
    wap_plane_properties properties;
    std::vector<uint32_t> tiles;
    std::vector<std::string> image_sets;
    std::vector<wap_object> objects;
};

struct wap_wwd {
    uint32_t checksum = 0;
    wap_wwd_properties properties;
    std::vector<wap_plane> planes;
    std::vector<wap_tile_description> tile_descriptions;
};

#endif
