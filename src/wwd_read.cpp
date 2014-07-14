#include "common.h"
#include "errors.h"
#include "wwd.h"
#include "io.h"
#include <algorithm>
#include <cstdlib>
#include <vector>

enum {
	WWD_HEADER_SIZE = 1524,
};

static void read_rect(InputStream &stream, Wap32Rect &rect)
{
    stream.read(rect.left, rect.top, rect.right, rect.bottom);
}

static void read_string(InputStream &stream, std::string &str, size_t len)
{
    str.resize(len);
    for(size_t i = 0; i < len; ++i)
        stream.read(str[i]);
}

static void read_nullterminated_string(InputStream &stream, std::string &str)
{
    char c = '\0';
    while(1) {
        stream.read(c);
        if(!c)
            break;
        str += c;
    }
}

static void read_objects(InputStream &stream, std::vector<Wap32WwdObject> &objects)
{
    for(Wap32WwdObject &obj : objects) {
        unsigned name_len, logic_len, image_set_len, animation_len;
        stream.read(obj.id, name_len, logic_len, image_set_len, animation_len, obj.x, obj.y, obj.z, obj.i, obj.add_flags,
                    obj.dynamic_flags, obj.draw_flags, obj.user_flags, obj.score, obj.points, obj.powerup, obj.damage,
                    obj.smarts, obj.health);
        
        read_rect(stream, obj.move_rect);
        read_rect(stream, obj.hit_rect);
        read_rect(stream, obj.attack_rect);
        read_rect(stream, obj.clip_rect);
        read_rect(stream, obj.user_rects[0]);
        read_rect(stream, obj.user_rects[1]);
        
        stream.read(obj.user_values);
        stream.read(obj.x_min, obj.y_min, obj.x_max, obj.y_max, obj.speed_x, obj.speed_y, obj.x_tweak, obj.y_tweak, obj.counter,
                    obj.speed, obj.width, obj.health, obj.direction, obj.face_dir, obj.time_delay, obj.frame_delay,
                    obj.object_type, obj.hit_type_flags, obj.x_move_res, obj.y_move_res);
        
        read_string(stream, obj.name, name_len);
        read_string(stream, obj.logic, logic_len);
        read_string(stream, obj.image_set, image_set_len);
        read_string(stream, obj.animation, animation_len);
    }
}

static void read_planes(InputStream &stream, Wap32Wwd *wwd)
{
    for(Wap32WwdPlane &plane : wwd->planes) {
        unsigned height_px, width_px; // TODO: These values are not actually checked
        unsigned num_image_sets, num_objects;
        unsigned tiles_offset, image_sets_offset, objects_offset;
        
        stream.read(160, 0, plane.flags, 0, plane.name, height_px, width_px, plane.tile_width, plane.tile_height,
                    plane.tiles_wide, plane.tiles_high, 0, 0, plane.movement_x_percent, plane.movement_y_percent,
                    plane.fill_color, num_image_sets, num_objects, tiles_offset, image_sets_offset, objects_offset,
                    plane.z_coord, 0, 0, 0);
        
        plane.tiles.resize(plane.tiles_wide * plane.tiles_high);
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

static void read_tiles_descriptions(InputStream &stream, Wap32Wwd *wwd)
{
    unsigned num_tiles_descriptions;
    stream.read(32, 0, num_tiles_descriptions, 0, 0, 0, 0, 0);
    
    wwd->tile_descriptions.resize(num_tiles_descriptions);
    
    int i = 0;
    for(Wap32WwdTileDescription &desc : wwd->tile_descriptions) {
        stream.read(desc.type, 0, desc.width, desc.height);
        if(desc.type == WAP32_WWD_TILE_TYPE_SINGLE) {
            stream.read(desc.inside_attrib);
        } else {
            stream.read(desc.outside_attrib, desc.inside_attrib);
            read_rect(stream, desc.rect);
        }
        ++i;
    }
}

static void read_main_block(InputStream &stream, Wap32Wwd *wwd, unsigned planes_offset,
                            unsigned tiles_descriptions_offset)
{
    stream.seek(planes_offset);
    read_planes(stream, wwd);
    
    stream.seek(tiles_descriptions_offset);
    read_tiles_descriptions(stream, wwd);
}


int wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer)
{
    try {
        Wap32ErrorContext errctx("reading wwd buffer");
        
        InputStream stream(input_buffer.data(), input_buffer.size());
        unsigned signature;
        stream.read(signature);
        
        if(signature != WWD_HEADER_SIZE) {
            wap32_err__critical("input buffer does not have WWD signature");
            return WAP32_EINVALIDDATA;
        }
        
        unsigned num_planes, planes_offset, tiles_descriptions_offset, decompressed_main_block_size, their_checksum;
        stream.read(0, wwd->flags, 0, wwd->level_name, wwd->author, wwd->birth, wwd->rez_file, wwd->image_dir, wwd->pal_rez,
                    wwd->start_x, wwd->start_y, 0, num_planes, planes_offset, tiles_descriptions_offset,
                    decompressed_main_block_size, their_checksum, 0, wwd->launch_app, wwd->image_sets[0], wwd->image_sets[1],
                    wwd->image_sets[2], wwd->image_sets[3], wwd->prefixes[0], wwd->prefixes[1], wwd->prefixes[2],
                    wwd->prefixes[3]);
        
        assert(stream.tell() == WWD_HEADER_SIZE);
        wwd->planes.resize(num_planes);
                
        if(wwd->flags & WAP32_WWD_COMPRESS) {
            std::vector<char> decompressed_buffer(WWD_HEADER_SIZE + decompressed_main_block_size);
            memcpy(&decompressed_buffer[0], &input_buffer[0], WWD_HEADER_SIZE);
            const char *main_block = &input_buffer[WWD_HEADER_SIZE];
            const size_t main_block_size = input_buffer.size() - WWD_HEADER_SIZE;
            char *decompressed_main_block = &decompressed_buffer[WWD_HEADER_SIZE];
            int error = wap32_util_inflate(decompressed_main_block, decompressed_main_block_size, main_block, main_block_size);
            if(error < 0)
                return WAP32_EINVALIDDATA;
            InputStream stream_decompressed(&decompressed_buffer[0], decompressed_buffer.size(), stream.tell());
            read_main_block(stream_decompressed, wwd, planes_offset, tiles_descriptions_offset);
        } else {
            read_main_block(stream, wwd, planes_offset, tiles_descriptions_offset);
        }
    } catch (InputStream::EndOfBuffer&) {
        return WAP32_EINVALIDDATA;
    }
    return WAP32_OK;
}
