#include "common.h"
#include "wwd.h"

#include "buffer.h"
#include "errors.h"
#include "io.h"

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <vector>

enum {
	WAP_WWD_HEADER_SIZE = 1524,
    WAP_WWD_PLANE_DESCRIPTION_SIZE = 160,
    WAP_WWD_OBJECT_DESCRIPTION_SIZE = 284,
};

struct plane_offsets {
    unsigned tiles_offset, image_sets_offset, objects_offset;
};

struct wwd_offsets {
    unsigned tile_descriptions_offset;
    unsigned eof_offset;
    std::vector<plane_offsets> planes_offsets;
};

static void calculate_offsets(wwd_offsets &offsets, const wap_wwd &wwd)
{
    unsigned offset = WAP_WWD_HEADER_SIZE;
    size_t num_planes = wwd.planes.size();
    offsets.planes_offsets.resize(num_planes);
    offset += num_planes * WAP_WWD_PLANE_DESCRIPTION_SIZE;
    offsets.tile_descriptions_offset = offset;
    
    for(size_t i = 0; i < num_planes; ++i) {
        offsets.planes_offsets[i].tiles_offset = offset;
        offset += wwd.planes[i].tiles.size() * 4;
    }
    
    for(size_t i = 0; i < num_planes; ++i) {
        offsets.planes_offsets[i].image_sets_offset = offset;
        for(const std::string &image_set : wwd.planes[i].image_sets)
            offset += image_set.size() + sizeof('\0');
    }
    
    for(size_t i = 0; i < num_planes; ++i) {
        offsets.planes_offsets[i].objects_offset = offset;
        for(const wap_object &object : wwd.planes[i].objects) {
            offset += WAP_WWD_OBJECT_DESCRIPTION_SIZE;
            offset += object.name.size() + object.logic.size() + object.image_set.size() + object.animation.size();
        }
    }
    
    offsets.tile_descriptions_offset = offset;
    offset += 8 * sizeof(unsigned);
    
    for(const wap_tile_description &desc : wwd.tile_descriptions) {
        if(desc.type == WAP_WWD_TILE_TYPE_SINGLE)
            offset += 5 * sizeof(unsigned);
        else offset += 10 * sizeof(unsigned);
    }
    
    offsets.eof_offset = offset;
}


static void write_rect(OutputStream &stream, const wap_rect &rect)
{
    stream.write(rect.left, rect.top, rect.right, rect.bottom);
}

static void write_string(OutputStream &stream, const std::string &str)
{
    stream.write_buffer(str.data(), str.size());
}

static void write_nullterminated_string(OutputStream &stream, const std::string &str)
{
    stream.write_buffer(str.c_str(), str.size()+1);
}

static void write_objects(OutputStream &stream, const std::vector<wap_object> &objects)
{
    for(const wap_object &obj : objects) {
        auto &objp = obj.properties;
        
        stream.write(objp.id, (unsigned)obj.name.size(), (unsigned)obj.logic.size(), (unsigned)obj.image_set.size(),
                     (unsigned)obj.animation.size(), objp.x, objp.y, objp.z, objp.i, objp.add_flags, objp.dynamic_flags,
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

static void write_planes(OutputStream &stream, const std::vector<wap_plane> &planes,
                         const std::vector<plane_offsets> & planes_offsets)
{
    auto plane_offsets = planes_offsets.cbegin();
    for(const wap_plane &plane : planes) {
        auto &planep = plane.properties;
        unsigned height_px = planep.tiles_high * planep.tile_height;
        unsigned width_px = planep.tiles_wide * planep.tile_width;
        unsigned objects_offset = plane.objects.empty() ? 0 : plane_offsets->objects_offset;
        
        stream.write(160, 0, planep.flags, 0, planep.name, width_px, height_px, planep.tile_width, planep.tile_height,
                     planep.tiles_wide, planep.tiles_high, 0, 0, planep.movement_x_percent, planep.movement_y_percent,
                     planep.fill_color, (unsigned)plane.image_sets.size(), (unsigned)plane.objects.size(),
                     plane_offsets->tiles_offset, plane_offsets->image_sets_offset, objects_offset, planep.z_coord, 0, 0, 0);
        
        ++plane_offsets;
    }
    
    for(const wap_plane &plane : planes) {
        for(unsigned tile : plane.tiles)
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

static void write_tile_descriptions(OutputStream &stream, const std::vector<wap_tile_description> &tile_descriptions)
{
    stream.write(32, 0, (unsigned)tile_descriptions.size(), 0, 0, 0, 0, 0);
    
    for(const wap_tile_description &desc : tile_descriptions) {
        stream.write(desc.type, 0, desc.width, desc.height);
        if(desc.type == WAP_WWD_TILE_TYPE_SINGLE) {
            stream.write(desc.inside_attrib);
        } else {
            stream.write(desc.outside_attrib, desc.inside_attrib);
            write_rect(stream, desc.rect);
        }
    }
}

static void write_main_block(OutputStream &stream, const std::vector<wap_plane> &planes,
                             const std::vector<plane_offsets> &planes_offsets,
                             const std::vector<wap_tile_description> &tile_descriptions)
{
    write_planes(stream, planes, planes_offsets);
    write_tile_descriptions(stream, tile_descriptions);
}

static void write_header(OutputStream &stream, const wap_wwd &wwd, const wwd_offsets &offsets,
                         unsigned decompressed_main_block_size, unsigned checksum)
{
    const wap_wwd_properties &wwdp = wwd.properties;
    unsigned num_planes = wwd.planes.size();
    stream.write(WAP_WWD_HEADER_SIZE);
    stream.write(0, wwdp.flags, 0, wwdp.level_name, wwdp.author, wwdp.birth, wwdp.rez_file, wwdp.image_dir, wwdp.pal_rez,
                 wwdp.start_x, wwdp.start_y, 0, num_planes, WAP_WWD_HEADER_SIZE, offsets.tile_descriptions_offset,
                 decompressed_main_block_size, checksum, 0, wwdp.launch_app, wwdp.image_sets[0], wwdp.image_sets[1],
                 wwdp.image_sets[2], wwdp.image_sets[3], wwdp.prefixes[0], wwdp.prefixes[1], wwdp.prefixes[2],
                 wwdp.prefixes[3]);
}

int wap_wwd_write(const wap_wwd *wwd, wap_buffer *out_wwd_buffer)
{
    try {
        wap_error_context errctx("writing wwd buffer");
        
        std::vector<char> &wwd_buffer = wap_buffer__vector(out_wwd_buffer);
        const wap_wwd_properties &wwdp = wwd->properties;
        
        wwd_offsets offsets;
        calculate_offsets(offsets, *wwd);
        
        if(wwdp.flags & WAP_WWD_FLAG_COMPRESS) {
            std::vector<char> main_block(offsets.eof_offset - WAP_WWD_HEADER_SIZE);
            OutputStream main_block_stream(main_block.data(), main_block.size());
            write_main_block(main_block_stream, wwd->planes, offsets.planes_offsets, wwd->tile_descriptions);
            unsigned checksum = wap_util_checksum(main_block.data(), main_block.size());
            std::vector<char> compressed_main_block;
            int error = wap_util_deflate(compressed_main_block, main_block.data(), main_block.size());
            if(error < 0)
                return WAP_ERROR;
            wwd_buffer.resize(WAP_WWD_HEADER_SIZE + compressed_main_block.size());
            OutputStream stream(wap_buffer_data(out_wwd_buffer), wap_buffer_size(out_wwd_buffer));
            write_header(stream, *wwd, offsets, main_block.size(), checksum);
            stream.write_buffer(compressed_main_block.data(), compressed_main_block.size());
        } else {
            wwd_buffer.resize(offsets.eof_offset);
            char *buffer = wap_buffer_data(out_wwd_buffer);
            size_t buffer_size = wap_buffer_size(out_wwd_buffer);
            OutputStream stream(buffer, buffer_size);
            stream.seek(WAP_WWD_HEADER_SIZE);
            write_main_block(stream, wwd->planes, offsets.planes_offsets, wwd->tile_descriptions);
            unsigned checksum = wap_util_checksum(buffer + WAP_WWD_HEADER_SIZE, buffer_size - WAP_WWD_HEADER_SIZE);
            stream.seek(0);
            write_header(stream, *wwd, offsets, 0, checksum);
        }
    } catch (OutputStream::EndOfBuffer&) {
        return WAP_EINVALIDDATA;
    }
    return WAP_OK;
}

int wap_wwd_save(const wap_wwd *wwd, const char *file_path)
{
    try {
        wap_error_context errctx("saving file '%s'", file_path);
        std::ofstream file(file_path, std::ios::binary);
        if(!file.good())
            return WAP_EFILE;
        wap_buffer wwd_buffer;
        wap_buffer_init(&wwd_buffer);
        std::unique_ptr<wap_buffer, void (*)(wap_buffer*)> wap_buffer_ptr { &wwd_buffer, wap_buffer_destroy };
        int error = wap_wwd_write(wwd, &wwd_buffer);
        if(error < 0)
        return error;
        file.write(wap_buffer_data(&wwd_buffer), wap_buffer_size(&wwd_buffer));
        if(!file.good())
            return WAP_EFILE;
        return WAP_OK;
    } catch(std::bad_alloc&) {
        return WAP_ENOMEMORY;
    }
}
