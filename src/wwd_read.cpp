#include "common.h"
#include "errors.h"
#include "wwd.h"
#include "io.h"
#include "wap32/buffer.h"
#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>

enum {
	WWD_HEADER_SIZE = 1524,
};

static void read_rect(InputStream &stream, wap_rect &rect)
{
    stream.read(rect.left, rect.top, rect.right, rect.bottom);
}

static void read_string(InputStream &stream, std::string &str, size_t len)
{
    str.resize(len);
    stream.read_buffer(&str[0], str.size());
}

static void read_nullterminated_string(InputStream &stream, std::string &str)
{
    str.erase();
    char c = '\0';
    while(1) {
        stream.read(c);
        if(!c)
            break;
        str += c;
    }
}

static void read_objects(InputStream &stream, std::vector<wap_object> &objects)
{
    for(wap_object &obj : objects) {
        auto &objp = obj.properties;

        unsigned name_len, logic_len, image_set_len, animation_len;
        stream.read(objp.id, name_len, logic_len, image_set_len, animation_len, objp.x, objp.y, objp.z, objp.i, objp.add_flags,
                    objp.dynamic_flags, objp.draw_flags, objp.user_flags, objp.score, objp.points, objp.powerup, objp.damage,
                    objp.smarts, objp.health);
        
        read_rect(stream, objp.move_rect);
        read_rect(stream, objp.hit_rect);
        read_rect(stream, objp.attack_rect);
        read_rect(stream, objp.clip_rect);
        read_rect(stream, objp.user_rects[0]);
        read_rect(stream, objp.user_rects[1]);
        
        stream.read(objp.user_values);
        stream.read(objp.x_min, objp.y_min, objp.x_max, objp.y_max, objp.speed_x, objp.speed_y, objp.x_tweak, objp.y_tweak,
                    objp.counter, objp.speed, objp.width, objp.height, objp.direction, objp.face_dir, objp.time_delay,
                    objp.frame_delay, objp.object_type, objp.hit_type_flags, objp.x_move_res, objp.y_move_res);
        
        read_string(stream, obj.name, name_len);
        read_string(stream, obj.logic, logic_len);
        read_string(stream, obj.image_set, image_set_len);
        read_string(stream, obj.animation, animation_len);
    }
}

static void read_planes(InputStream &stream, std::vector<wap_plane> &planes)
{
    for(wap_plane &plane : planes) {
        auto &planep = plane.properties;
        unsigned height_px, width_px; // TODO: These values are not actually checked
        unsigned num_image_sets, num_objects;
        unsigned tiles_offset, image_sets_offset, objects_offset;
        
        stream.read(160, 0, planep.flags, 0, planep.name, width_px, height_px, planep.tile_width, planep.tile_height,
                    planep.tiles_wide, planep.tiles_high, 0, 0, planep.movement_x_percent, planep.movement_y_percent,
                    planep.fill_color, num_image_sets, num_objects, tiles_offset, image_sets_offset, objects_offset,
                    planep.z_coord, 0, 0, 0);
        
        plane.tiles.resize(planep.tiles_wide * planep.tiles_high);
        plane.image_sets.resize(num_image_sets);
        plane.objects.resize(num_objects);

        InputStream substream(stream);
        
        substream.seek(tiles_offset);
        for(unsigned &tile : plane.tiles)
            substream.read(tile);
        
        substream.seek(image_sets_offset);
        for(auto &image_set : plane.image_sets)
            read_nullterminated_string(substream, image_set);
        
        substream.seek(objects_offset);
        read_objects(substream, plane.objects);
    }
}

static void read_tile_descriptions(InputStream &stream, std::vector<wap_tile_description> &tile_descriptions)
{
    unsigned num_tile_descriptions;
    stream.read(32, 0, num_tile_descriptions, 0, 0, 0, 0, 0);
    
    tile_descriptions.resize(num_tile_descriptions);
    
    for(wap_tile_description &desc : tile_descriptions) {
        stream.read(desc.type, 0, desc.width, desc.height);
        if(desc.type == WAP_WWD_TILE_TYPE_SINGLE) {
            stream.read(desc.inside_attrib);
        } else {
            stream.read(desc.outside_attrib, desc.inside_attrib);
            read_rect(stream, desc.rect);
        }
    }
}

static void read_main_block(InputStream &stream, unsigned planes_offset, std::vector<wap_plane> &planes,
                            unsigned tiles_descriptions_offset, std::vector<wap_tile_description> &tile_descriptions)
{
    stream.seek(planes_offset);
    read_planes(stream, planes);
    
    stream.seek(tiles_descriptions_offset);
    read_tile_descriptions(stream, tile_descriptions);
}


int wap_wwd_read(wap_wwd *wwd, const char *wwd_buffer, size_t wwd_buffer_size)
{
    try {
        wap_error_context errctx("reading wwd buffer");
        InputStream stream(wwd_buffer, wwd_buffer_size);

        auto &wwdp = wwd->properties;
        unsigned signature;
        stream.read(signature);
        
        if(signature != WWD_HEADER_SIZE) {
            wap_err__critical("input buffer does not have WWD signature");
            return WAP_EINVALIDDATA;
        }
        
        unsigned num_planes, planes_offset, tiles_descriptions_offset, decompressed_main_block_size, their_checksum;
        stream.read(0, wwdp.flags, 0, wwdp.level_name, wwdp.author, wwdp.birth, wwdp.rez_file, wwdp.image_dir, wwdp.pal_rez,
                    wwdp.start_x, wwdp.start_y, 0, num_planes, planes_offset, tiles_descriptions_offset,
                    decompressed_main_block_size, their_checksum, 0, wwdp.launch_app, wwdp.image_sets[0], wwdp.image_sets[1],
                    wwdp.image_sets[2], wwdp.image_sets[3], wwdp.prefixes[0], wwdp.prefixes[1], wwdp.prefixes[2],
                    wwdp.prefixes[3]);
        
        assert(stream.tell() == WWD_HEADER_SIZE);
        wwd->planes.resize(num_planes);
                
        if(wwdp.flags & WAP_WWD_FLAG_COMPRESS) {
            std::vector<char> decompressed_buffer(WWD_HEADER_SIZE + decompressed_main_block_size);
            memcpy(&decompressed_buffer[0], wwd_buffer, WWD_HEADER_SIZE);
            const char *main_block = wwd_buffer + WWD_HEADER_SIZE;
            const size_t main_block_size = wwd_buffer_size - WWD_HEADER_SIZE;
            char *decompressed_main_block = &decompressed_buffer[WWD_HEADER_SIZE];
            int error = wap_util_inflate(decompressed_main_block, decompressed_main_block_size, main_block, main_block_size);
            if(error < 0)
                return WAP_EINVALIDDATA;
            InputStream stream_decompressed(&decompressed_buffer[0], decompressed_buffer.size(), stream.tell());
            read_main_block(stream_decompressed, planes_offset, wwd->planes, tiles_descriptions_offset, wwd->tile_descriptions);
        } else {
            read_main_block(stream, planes_offset, wwd->planes, tiles_descriptions_offset, wwd->tile_descriptions);
        }
    } catch (InputStream::EndOfBuffer&) {
        return WAP_EINVALIDDATA;
    }
    return WAP_OK;
}

int wap_wwd_open(wap_wwd *wwd, const char *file_path)
{
    try {
        wap_error_context errctx("opening file '%s'", file_path);
        std::ifstream file(file_path, std::ios::binary);
        std::vector<char> wwd_buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if(!file.good())
        return WAP_EFILE;
        int error = wap_wwd_read(wwd, wwd_buffer.data(), wwd_buffer.size());
        if(error < 0)
        return error;
        return WAP_OK;
    } catch(std::bad_alloc&) {
        return WAP_ENOMEMORY;
    }
}
