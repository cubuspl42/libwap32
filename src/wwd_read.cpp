#include "common.h"
#include "wwd.h"

#include "buffer.h"
#include "errors.h"
#include "io.h"
#include "wwd_io.h"

#include <zlib.h>

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>

void decompress_buffer(char *out_buffer, size_t out_buffer_size, const char *in_buffer, size_t in_buffer_size)
{
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = in_buffer_size;
    strm.next_in = (unsigned char*)in_buffer;
    strm.avail_out = out_buffer_size;
    strm.next_out = (unsigned char*)out_buffer;
    
    int ret = inflateInit(&strm);
    if (ret != Z_OK) {
        inflateEnd(&strm);
        throw wap::Error::from_zlib_error(ret);
    }
    
    ret = inflate(&strm, Z_FINISH);
    inflateEnd(&strm);
    
    if(ret != Z_STREAM_END) {
        throw wap::Error::from_zlib_error(ret);
    }
}

static void read_rect(wap::InputStream &stream, wap_rect &rect)
{
    stream.read(rect.left, rect.top, rect.right, rect.bottom);
}

static void read_string(wap::InputStream &stream, std::string &str, size_t len)
{
    str.resize(len);
    stream.read_buffer(&str[0], str.size());
}

static void read_nullterminated_string(wap::InputStream &stream, std::string &str)
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

static void read_objects(wap::InputStream &stream, std::vector<wap_object> &objects)
{
    for(wap_object &obj : objects) {
        auto &objp = obj.properties;
        
        uint32_t name_len, logic_len, image_set_len, animation_len;
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

static void read_planes(wap::InputStream &stream, std::vector<wap_plane> &planes)
{
    std::vector<wwd_plane_offsets> planes_offsets(planes.size());
    
    auto plane_offsets = planes_offsets.begin();
    for(wap_plane &plane : planes) {
        auto &planep = plane.properties;
        uint32_t width_px, height_px; // These values are not actually checked
        uint32_t num_image_sets, num_objects;
        
        stream.read(160, 0, planep.flags, 0, planep.name, width_px, height_px, planep.tile_width, planep.tile_height,
                    plane.tiles_wide, plane.tiles_high, 0, 0, planep.movement_x_percent, planep.movement_y_percent,
                    planep.fill_color, num_image_sets, num_objects, plane_offsets->tiles_offset,
                    plane_offsets->image_sets_offset, plane_offsets->objects_offset, planep.z_coord, 0, 0, 0);
        
        plane.tiles.resize(plane.tiles_wide * plane.tiles_high);
        plane.image_sets.resize(num_image_sets);
        plane.objects.resize(num_objects);
        ++plane_offsets;
    }
    
    plane_offsets = planes_offsets.begin();
    for(wap_plane &plane : planes) {
        if(!plane.tiles.empty()) {
            stream.seek(plane_offsets->tiles_offset);
            for(uint32_t &tile : plane.tiles)
                stream.read(tile);
        }
        ++plane_offsets;
    }
    
    plane_offsets = planes_offsets.begin();
    for(wap_plane &plane : planes) {
        if(!plane.image_sets.empty()) {
            stream.seek(plane_offsets->image_sets_offset);
            for(std::string &image_set : plane.image_sets)
                read_nullterminated_string(stream, image_set);
        }
        ++plane_offsets;
    }
    
    plane_offsets = planes_offsets.begin();
    for(wap_plane &plane : planes) {
        if(!plane.objects.empty()) {
            stream.seek(plane_offsets->objects_offset);
            read_objects(stream, plane.objects);
        }
        ++plane_offsets;
    }
}

static void read_tile_descriptions(wap::InputStream &stream, std::vector<wap_tile_description> &tile_descriptions)
{
    uint32_t num_tile_descriptions;
    stream.read(32, 0, num_tile_descriptions, 0, 0, 0, 0, 0);
    
    tile_descriptions.resize(num_tile_descriptions);
    
    for(wap_tile_description &desc : tile_descriptions) {
        stream.read(desc.type, 0, desc.width, desc.height);
        if(desc.type == WAP_TILE_TYPE_SINGLE) {
            stream.read(desc.inside_attrib);
        } else {
            stream.read(desc.outside_attrib, desc.inside_attrib);
            read_rect(stream, desc.rect);
        }
    }
}

static void read_main_block(wap::InputStream &stream, std::vector<wap_plane> &planes,
                            std::vector<wap_tile_description> &tile_descriptions, const wwd_offsets &offsets)
{
    stream.seek(offsets.main_block_offset);
    read_planes(stream, planes);
    
    stream.seek(offsets.tile_descriptions_offset);
    read_tile_descriptions(stream, tile_descriptions);
}

static void read_header(wap::InputStream &stream, wap_wwd &wwd, wwd_offsets &offsets, uint32_t &decompressed_main_block_size)
{
    uint32_t signature;
    stream.read(signature);
    
    if(signature != WAP_WWD_HEADER_SIZE)
        throw wap::Error(WAP_EINVALIDDATA);
    
    wap_wwd_properties &wwdp = wwd.properties;
    uint32_t num_planes;
    
    stream.read(0, wwdp.flags, 0, wwdp.level_name, wwdp.author, wwdp.birth, wwdp.rez_file, wwdp.image_dir, wwdp.pal_rez,
                wwdp.start_x, wwdp.start_y, 0, num_planes, offsets.main_block_offset, offsets.tile_descriptions_offset,
                decompressed_main_block_size, wwd.checksum, 0, wwdp.launch_app, wwdp.image_sets[0], wwdp.image_sets[1],
                wwdp.image_sets[2], wwdp.image_sets[3], wwdp.prefixes[0], wwdp.prefixes[1], wwdp.prefixes[2],
                wwdp.prefixes[3]);
    
    wwd.planes.resize(num_planes);
}

void wwd_read(wap_wwd &out_wwd, const char *wwd_buffer, size_t wwd_buffer_size)
{
    wap_error_context errctx("reading wwd buffer");
    wap_wwd wwd;
    
    wap::InputStream stream(wwd_buffer, wwd_buffer_size);
    
    wap_wwd_properties &wwdp = wwd.properties;
    wwd_offsets offsets;
    uint32_t decompressed_main_block_size;
    read_header(stream, wwd, offsets, decompressed_main_block_size);
    
    if(wwdp.flags & WAP_WWD_FLAG_COMPRESS) {
        std::vector<char> decompressed_buffer(offsets.main_block_offset + decompressed_main_block_size);
        memcpy(decompressed_buffer.data(), wwd_buffer, offsets.main_block_offset);
        const char *compressed_main_block = wwd_buffer + offsets.main_block_offset;
        size_t compressed_main_block_size = wwd_buffer_size - offsets.main_block_offset;
        char *decompressed_main_block = decompressed_buffer.data() + offsets.main_block_offset;
        decompress_buffer(decompressed_main_block, decompressed_main_block_size,
                          compressed_main_block, compressed_main_block_size);
        wap::InputStream stream_decompressed(&decompressed_buffer[0], decompressed_buffer.size(), stream.tell());
        read_main_block(stream_decompressed, wwd.planes, wwd.tile_descriptions, offsets);
    } else {
        read_main_block(stream, wwd.planes, wwd.tile_descriptions, offsets);
    }
    
    std::swap(wwd, out_wwd);
}

void wwd_open(wap_wwd &wwd, const char *file_path)
{
    std::vector<char> wwd_buffer = wap::read_whole_file(file_path);
    wwd_read(wwd, wwd_buffer.data(), wwd_buffer.size());
}

int wap_wwd_read(wap_wwd *out_wwd, const char *wwd_buffer, size_t wwd_buffer_size)
{
    return wap::handle_exceptions(wwd_read, *out_wwd, wwd_buffer, wwd_buffer_size);
}

int wap_wwd_open(wap_wwd *wwd, const char *file_path)
{
    return wap::handle_exceptions(wwd_open, *wwd, file_path);
}
