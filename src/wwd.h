#ifndef wwd_h
#define wwd_h

#include "wap32/wwd.h"
#include "common.h"
#include <array>
#include <string>
#include <vector>

struct Wap32Object {
    Wap32ObjectProperties properties;
    std::string name;
    std::string logic;
    std::string image_set;
    std::string animation;
};

struct Wap32Plane {
    Wap32PlaneProperties properties;
    std::vector<unsigned> tiles;
	std::vector<std::string> image_sets;
    std::vector<Wap32Object> objects;
};

struct Wap32Wwd {
    Wap32WwdProperties properties;
	std::vector<Wap32Plane> planes;
    std::vector<Wap32TileDescription> tile_descriptions;
};

int wap32_wwd__read(Wap32Wwd &wwd, const std::vector<char> &input_buffer);

#endif