#include "wwd_read.h"
#include "io.h"
#include <vector>

enum {
	WWD_SIGNATURE = 0x05F4
};

void wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer)
{
    InputStream stream(input_buffer.data(), input_buffer.size());
    
    unsigned signature = 0;
    stream.read(signature);
    if(signature != WWD_SIGNATURE) {
        wap32_err__throw(WAP32_EINVALIDDATA, "Input buffer does not contain WWD data");
    }
    
    auto &d = wwd->data;
    unsigned num_planes, planes_offset, tile_properties_offset, main_block_length, checksum;
    stream.read(0, d.flags, 0,
                d.level_name, d.author, d.birth, d.rez_file, d.image_dir, d.pal_rez,
                d.start_x, d.start_y, 0,
                num_planes, planes_offset, tile_properties_offset, main_block_length, checksum, 0,
                d.launch_app,
                d.image_sets[0], d.image_sets[1], d.image_sets[2], d.image_sets[3],
                d.prefixes[0], d.prefixes[1], d.prefixes[2], d.prefixes[3]);
}
