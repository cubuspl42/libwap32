#ifndef wwd_io_h
#define wwd_io_h

#include <vector>

enum {
    WAP_WWD_HEADER_SIZE = 1524,
    WAP_WWD_PLANE_DESCRIPTION_SIZE = 160,
    WAP_WWD_OBJECT_DESCRIPTION_SIZE = 284,
};

struct wwd_plane_offsets {
    unsigned tiles_offset, image_sets_offset, objects_offset;
};

struct wwd_offsets {
    unsigned main_block_offset;
    unsigned tile_descriptions_offset;
    unsigned eof_offset;
};

#endif
