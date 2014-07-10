#include "wwd.h"
#include "io.h"
#include <vector>

enum {
	WWD_SIGNATURE = 0x05F4
};

struct WwdIoData {
    unsigned num_planes, planes_offset, tile_properties_offset, main_block_length, checksum;
};

static void read_header(InputStream &stream, Wap32Wwd &wwd, WwdIoData &data)
{
    stream.read(0, wwd.flags, 0,
                wwd.level_name, wwd.author, wwd.birth, wwd.rez_file,
                wwd.image_dir, wwd.pal_rez, wwd.start_x, wwd.start_y, 0,
                data.num_planes, data.planes_offset, data.tile_properties_offset,
                data.main_block_length, data.checksum, 0, wwd.launch_app,
                wwd.image_sets[0], wwd.image_sets[1], wwd.image_sets[2],
                wwd.image_sets[3], wwd.prefixes[0], wwd.prefixes[1],
                wwd.prefixes[2], wwd.prefixes[3]);
}

int wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer)
{
    Wap32ErrorContext errctx("reading wwd buffer");
    InputStream stream(input_buffer);
    
    try {
        unsigned signature = 0;
        stream.read(signature);
        if(signature != WWD_SIGNATURE) {
            wap32_err__critical("input buffer does not have WWD signature");
            return WAP32_EINVALIDDATA;
        }
        
        WwdIoData data;
        read_header(stream, *wwd, data);
        wap32_wwd_set_planes_count(wwd, data.num_planes);
        
    } catch (InputStream::EndOfBuffer&) {
        wap32_err__critical("unexpected end of input buffer");
        return WAP32_EINVALIDDATA;
    }
    return 0;
}
