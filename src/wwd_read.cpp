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

static void read_rect(Wap32Rect &rect, InputStream &stream)
{
    stream.read(rect.left, rect.top, rect.right, rect.bottom);
}

template<size_t size>
static void read_object_string(char (&buffer)[size], size_t str_len, InputStream &stream)
{
    size_t n = std::min(size-1, str_len);
    stream.read_buffer(buffer, n);
    if(n < str_len)
        stream.seek(stream.tell() + (str_len - n));
    buffer[n] = '\0';
}

static void read_string_nullterminated(char *buffer, size_t buffer_size, InputStream &stream)
{
    char c;
    size_t i = 0;
    while(1) {
        stream.read(c);
        if(i < buffer_size-1)
            buffer[i] = c;
        else buffer[buffer_size-1] = '\0';
        if(!c)
            break;
        ++i;
    }
}

static void read_objects(std::vector<Wap32WwdObject> &objects, InputStream &stream)
{
    for(Wap32WwdObject &obj : objects) {
        unsigned name_len, logic_len, image_set_len, animation_len;
        stream.read(obj.id, name_len, logic_len, image_set_len, animation_len,
                    obj.x, obj.y, obj.z, obj.i, obj.add_flags, obj.dynamic_flags,
                    obj.draw_flags, obj.user_flags, obj.score, obj.points,
                    obj.powerup, obj.damage, obj.smarts, obj.health);
        
        read_rect(obj.move_rect, stream);
        read_rect(obj.hit_rect, stream);
        read_rect(obj.attack_rect, stream);
        read_rect(obj.clip_rect, stream);
        read_rect(obj.user_rects[0], stream);
        read_rect(obj.user_rects[1], stream);
        
        stream.read(obj.user_values);
        stream.read(obj.x_min, obj.y_min, obj.x_max, obj.y_max, obj.speed_x,
                    obj.speed_y, obj.x_tweak, obj.y_tweak, obj.counter,
                    obj.speed, obj.width, obj.health, obj.direction, obj.face_dir,
                    obj.time_delay, obj.frame_delay, obj.object_type,
                    obj.hit_type_flags, obj.x_move_res, obj.y_move_res);
        
        read_object_string(obj.name, name_len, stream);
        read_object_string(obj.logic, logic_len, stream);
        read_object_string(obj.image_set, image_set_len, stream);
        read_object_string(obj.animation, animation_len, stream);
    }
}

static void read_planes(Wap32Wwd *wwd, InputStream &stream)
{
    for(Wap32WwdPlane &plane : wwd->planes) {
        unsigned height_px, width_px; // TODO: These values are not actually checked
        unsigned num_image_sets, num_objects;
        unsigned tiles_offset, image_sets_offset, objects_offset;
        
        stream.read(160, 0, plane.flags, 0, plane.name, height_px, width_px,
                    plane.tile_width, plane.tile_height, plane.tiles_wide, plane.tiles_high, 0, 0,
                    plane.movement_x_percent, plane.movement_y_percent, plane.fill_color,
                    num_image_sets, num_objects, tiles_offset, image_sets_offset, objects_offset,
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
            read_string_nullterminated(image_set.data(), image_set.size(), substream);
        
        substream.seek(objects_offset);
        read_objects(plane.objects, substream);
    }
}

static void read_tiles_properties(Wap32Wwd *wwd, InputStream &stream)
{
    stream.read(32, 0);
    unsigned num_tiles_descriptions;
    stream.read(num_tiles_descriptions);
    stream.read(0, 0, 0, 0, 0);
    
    wwd->tile_descriptions.resize(num_tiles_descriptions);
    
    for(Wap32WwdTileDescription &desc : wwd->tile_descriptions) {
        stream.read(desc.kind, desc.width, desc.height);
        // TODO:
        // single: stream.read(desc.inside);
        // double:  stream.read(desc.outside, desc.inside);
        //          read_rect(desc.rect, stream);
    }
    
    
}

static void read_main_block(Wap32Wwd *wwd, InputStream &stream, unsigned planes_offset,
                            unsigned tiles_properties_offset)
{
    stream.seek(planes_offset);
    read_planes(wwd, stream);
    
    stream.seek(tiles_properties_offset);
    read_tiles_properties(wwd, stream);
}


int wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer)
{
    Wap32ErrorContext errctx("reading wwd buffer");
    try {
        InputStream stream(input_buffer.data(), input_buffer.size());
        unsigned signature;
        stream.read(signature);
        
        if(signature != WWD_HEADER_SIZE) {
            wap32_err__critical("input buffer does not have WWD signature");
            return WAP32_EINVALIDDATA;
        }
        
        unsigned num_planes, planes_offset, tiles_properties_offset, decompressed_main_block_size, their_checksum;
        stream.read(0, wwd->flags, 0,
                    wwd->level_name, wwd->author, wwd->birth, wwd->rez_file,
                    wwd->image_dir, wwd->pal_rez, wwd->start_x, wwd->start_y, 0,
                    num_planes, planes_offset, tiles_properties_offset,
                    decompressed_main_block_size, their_checksum, 0, wwd->launch_app,
                    wwd->image_sets[0], wwd->image_sets[1], wwd->image_sets[2], wwd->image_sets[3],
                    wwd->prefixes[0], wwd->prefixes[1], wwd->prefixes[2], wwd->prefixes[3]);
        
        wwd->planes.resize(num_planes);
        
        assert(stream.tell() == WWD_HEADER_SIZE);
        
        const char *main_block = &input_buffer[WWD_HEADER_SIZE];
        const size_t main_block_size = input_buffer.size() - WWD_HEADER_SIZE;
        
        unsigned our_checksum = wap32_util_checksum(main_block, main_block_size);
        //if(our_checksum != their_checksum) return WAP32_EINVALIDDATA;
        std::cerr << "checksum: their, our, diff: " << their_checksum << " " << our_checksum << " " << int(their_checksum)-int(our_checksum) << std::endl;
        
        if(wwd->flags & WAP32_WWD_COMPRESS) {
            std::vector<char> decompressed_buffer(WWD_HEADER_SIZE + decompressed_main_block_size);
            memcpy(&decompressed_buffer[0], &input_buffer[0], WWD_HEADER_SIZE);
            
            char *decompressed_main_block = &decompressed_buffer[WWD_HEADER_SIZE];
            int error = wap32_util_inflate(decompressed_main_block, decompressed_main_block_size,
                                           main_block, main_block_size);
            if(error < 0)
                return WAP32_EINVALIDDATA;
            
            InputStream stream_decompressed(&decompressed_buffer[0], decompressed_buffer.size(), stream.tell());
            read_main_block(wwd, stream_decompressed, planes_offset, tiles_properties_offset);
        } else {
            read_main_block(wwd, stream, planes_offset, tiles_properties_offset);
        }
    } catch (InputStream::EndOfBuffer&) {
        return WAP32_EINVALIDDATA;
    }
    return 0;
}
