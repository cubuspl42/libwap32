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

std::vector<char> compress_buffer(const char *in_buffer, size_t in_buffer_size)
{
    std::vector<char> out_buffer;
    
    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = in_buffer_size;
    strm.next_in = (unsigned char*)in_buffer;
    
    int ret = deflateInit(&strm, 9);
    if (ret != Z_OK) {
        deflateEnd(&strm);
        throw wap::Error::from_zlib_error(ret);
    }
    
    out_buffer.resize(0);
    
    do {
        constexpr size_t step = 64 * 1024;
        out_buffer.resize(out_buffer.size() + step);
        strm.avail_out = out_buffer.capacity() - strm.total_out;
        strm.next_out = (unsigned char*)out_buffer.data() + strm.total_out;
        ret = deflate(&strm, Z_FINISH);
    } while (ret != Z_STREAM_END);
    
    deflateEnd(&strm);
    out_buffer.resize(out_buffer.capacity() - strm.avail_out);
    return out_buffer;
}

static void calculate_offsets(wwd_offsets &offsets, std::vector<wwd_plane_offsets> &planes_offsets, const wap_wwd &wwd)
{
    size_t num_planes = wwd.planes.size();
    planes_offsets.resize(num_planes);
    
    uint32_t offset = WAP_WWD_HEADER_SIZE;
    offsets.main_block_offset = offset;
    
    offset += num_planes * WAP_WWD_PLANE_DESCRIPTION_SIZE;
    offsets.tile_descriptions_offset = offset;
    
    for(size_t i = 0; i < num_planes; ++i) {
        planes_offsets[i].tiles_offset = offset;
        offset += wwd.planes[i].tiles.size() * 4;
    }
    
    for(size_t i = 0; i < num_planes; ++i) {
        planes_offsets[i].image_sets_offset = offset;
        for(const std::string &image_set : wwd.planes[i].image_sets)
            offset += image_set.size() + sizeof('\0');
    }
    
    for(size_t i = 0; i < num_planes; ++i) {
        planes_offsets[i].objects_offset = offset;
        for(const wap_object &object : wwd.planes[i].objects) {
            offset += WAP_WWD_OBJECT_DESCRIPTION_SIZE;
            offset += object.name.size() + object.logic.size() + object.image_set.size() + object.animation.size();
        }
    }
    
    offsets.tile_descriptions_offset = offset;
    offset += 8 * sizeof(uint32_t);
    
    for(const wap_tile_description &desc : wwd.tile_descriptions) {
        if(desc.type == WAP_TILE_TYPE_SINGLE)
            offset += 5 * sizeof(uint32_t);
        else offset += 10 * sizeof(uint32_t);
    }
    
    offsets.eof_offset = offset;
}

// This function tries to imitate WapWorld's checksum function. It's result is different on all compressed and some
// non compressed files. That's ok, because as far as I know it's only used to check if two multiplayer clients have the same
// version of map installed. Partial compatiblity with WapWorld's checksum is just to allow comparing our wwd buffers with
// WapWorld generated ones during testing.

uint32_t wwd_checksum(const char *buffer, size_t buffer_size)
{
    uint32_t checksum = UINT32_MAX - buffer_size - 159;
    for(size_t i = 0; i < buffer_size; ++i) {
        uint32_t delta = (i - (unsigned char)buffer[i]);
        checksum -= delta;
    }
    return checksum;
}

static void write_rect(wap::OutputStream &stream, const wap_rect &rect)
{
    stream.write(rect.left, rect.top, rect.right, rect.bottom);
}

static void write_string(wap::OutputStream &stream, const std::string &str)
{
    stream.write_buffer(str.data(), str.size());
}

static void write_nullterminated_string(wap::OutputStream &stream, const std::string &str)
{
    stream.write_buffer(str.c_str(), str.size()+1);
}

static void write_objects(wap::OutputStream &stream, const std::vector<wap_object> &objects)
{
    for(const wap_object &obj : objects) {
        auto &objp = obj.properties;
        
        stream.write(objp.id, (uint32_t)obj.name.size(), (uint32_t)obj.logic.size(), (uint32_t)obj.image_set.size(),
                     (uint32_t)obj.animation.size(), objp.x, objp.y, objp.z, objp.i, objp.add_flags, objp.dynamic_flags,
                     objp.draw_flags, objp.user_flags, objp.score, objp.points, objp.powerup, objp.damage, objp.smarts,
                     objp.health);
        
        write_rect(stream, objp.move_rect);
        write_rect(stream, objp.hit_rect);
        write_rect(stream, objp.attack_rect);
        write_rect(stream, objp.clip_rect);
        write_rect(stream, objp.user_rects[0]);
        write_rect(stream, objp.user_rects[1]);
        
        stream.write(objp.user_values, objp.x_min, objp.y_min, objp.x_max, objp.y_max, objp.speed_x, objp.speed_y,
                     objp.x_tweak, objp.y_tweak, objp.counter, objp.speed, objp.width, objp.height, objp.direction,
                     objp.face_dir, objp.time_delay, objp.frame_delay, objp.object_type, objp.hit_type_flags, objp.x_move_res,
                     objp.y_move_res);
        
        write_string(stream, obj.name);
        write_string(stream, obj.logic);
        write_string(stream, obj.image_set);
        write_string(stream, obj.animation);
    }
}

static void write_planes(wap::OutputStream &stream, const std::vector<wap_plane> &planes,
                         const std::vector<wwd_plane_offsets> & planes_offsets)
{
    auto plane_offsets = planes_offsets.cbegin();
    for(const wap_plane &plane : planes) {
        auto &planep = plane.properties;
        uint32_t height_px = plane.tiles_high * planep.tile_height;
        uint32_t width_px = plane.tiles_wide * planep.tile_width;
        uint32_t objects_offset = plane.objects.empty() ? 0 : plane_offsets->objects_offset;
        
        stream.write(160, 0, planep.flags, 0, planep.name, width_px, height_px, planep.tile_width, planep.tile_height,
                     plane.tiles_wide, plane.tiles_high, 0, 0, planep.movement_x_percent, planep.movement_y_percent,
                     planep.fill_color, (uint32_t)plane.image_sets.size(), (uint32_t)plane.objects.size(),
                     plane_offsets->tiles_offset, plane_offsets->image_sets_offset, objects_offset, planep.z_coord, 0, 0, 0);
        
        ++plane_offsets;
    }
    
    for(const wap_plane &plane : planes) {
        for(uint32_t tile : plane.tiles)
            stream.write(tile);
    }
    
    for(const wap_plane &plane : planes) {
        for(const std::string &image_set : plane.image_sets)
            write_nullterminated_string(stream, image_set);
    }
    
    for(const wap_plane &plane : planes) {
        write_objects(stream, plane.objects);
    }
}

static void write_tile_descriptions(wap::OutputStream &stream, const std::vector<wap_tile_description> &tile_descriptions)
{
    stream.write(32, 0, (uint32_t)tile_descriptions.size(), 0, 0, 0, 0, 0);
    
    for(const wap_tile_description &desc : tile_descriptions) {
        stream.write(desc.type, 0, desc.width, desc.height);
        if(desc.type == WAP_TILE_TYPE_SINGLE) {
            stream.write(desc.inside_attrib);
        } else {
            stream.write(desc.outside_attrib, desc.inside_attrib);
            write_rect(stream, desc.rect);
        }
    }
}

static void write_main_block(wap::OutputStream &stream, const std::vector<wap_plane> &planes,
                             const std::vector<wwd_plane_offsets> &planes_offsets,
                             const std::vector<wap_tile_description> &tile_descriptions)
{
    write_planes(stream, planes, planes_offsets);
    write_tile_descriptions(stream, tile_descriptions);
}

static void write_header(wap::OutputStream &stream, const wap_wwd &wwd, const wwd_offsets &offsets,
                         uint32_t decompressed_main_block_size, uint32_t checksum)
{
    const wap_wwd_properties &wwdp = wwd.properties;
    uint32_t num_planes = wwd.planes.size();
    stream.write(WAP_WWD_HEADER_SIZE);
    stream.write(0, wwdp.flags, 0, wwdp.level_name, wwdp.author, wwdp.birth, wwdp.rez_file, wwdp.image_dir, wwdp.pal_rez,
                 wwdp.start_x, wwdp.start_y, 0, num_planes, offsets.main_block_offset, offsets.tile_descriptions_offset,
                 decompressed_main_block_size, checksum, 0, wwdp.launch_app, wwdp.image_sets[0], wwdp.image_sets[1],
                 wwdp.image_sets[2], wwdp.image_sets[3], wwdp.prefixes[0], wwdp.prefixes[1], wwdp.prefixes[2],
                 wwdp.prefixes[3]);
}

void wwd_write(const wap_wwd *wwd, std::vector<char> &out_wwd_buffer)
{
    wap_error_context errctx("writing wwd buffer");
    
    wwd_offsets offsets;
    std::vector<wwd_plane_offsets> planes_offsets;
    calculate_offsets(offsets, planes_offsets, *wwd);
    
    std::vector<char> wwd_buffer;
    const wap_wwd_properties &wwdp = wwd->properties;
    
    if(wwdp.flags & WAP_WWD_FLAG_COMPRESS) {
        std::vector<char> main_block(offsets.eof_offset - offsets.main_block_offset);
        wap::OutputStream main_block_stream(main_block.data(), main_block.size());
        write_main_block(main_block_stream, wwd->planes, planes_offsets, wwd->tile_descriptions);
        std::vector<char> compressed_main_block = compress_buffer(main_block.data(), main_block.size());
        wwd_buffer.resize(offsets.main_block_offset + compressed_main_block.size());
        wap::OutputStream stream(wwd_buffer.data(), wwd_buffer.size());
        uint32_t checksum = wwd_checksum(main_block.data(), main_block.size());
        write_header(stream, *wwd, offsets, main_block.size(), checksum);
        stream.write_buffer(compressed_main_block.data(), compressed_main_block.size());
    } else {
        wwd_buffer.resize(offsets.eof_offset);
        wap::OutputStream stream(wwd_buffer.data(), wwd_buffer.size());
        stream.seek(offsets.main_block_offset);
        write_main_block(stream, wwd->planes, planes_offsets, wwd->tile_descriptions);
        char *main_block = wwd_buffer.data() + offsets.main_block_offset;
        size_t main_block_size = wwd_buffer.size() - offsets.main_block_offset;
        uint32_t checksum = wwd_checksum(main_block, main_block_size);
        stream.seek(0);
        write_header(stream, *wwd, offsets, 0, checksum);
    }
    
    std::swap(wwd_buffer, out_wwd_buffer);
}

void wwd_save(const wap_wwd *wwd, const char *file_path)
{
    wap_error_context errctx("saving file '%s'", file_path);
    std::ofstream file(file_path, std::ios::binary);
    if(!file.good())
        throw wap::Error(WAP_EFILE);
    std::vector<char> wwd_buffer;
    wwd_write(wwd, wwd_buffer);
    file.write(wwd_buffer.data(), wwd_buffer.size());
    if(!file.good())
        throw wap::Error(WAP_EFILE);
}

int wap_wwd_write(const wap_wwd *wwd, wap_buffer *out_wwd_buffer)
{
    return wap::handle_exceptions(wwd_write, wwd, *wap::cast_wap_buffer_to_vector(out_wwd_buffer));
}

int wap_wwd_save(const wap_wwd *wwd, const char *file_path)
{
    return wap::handle_exceptions(wwd_save, wwd, file_path);
}
