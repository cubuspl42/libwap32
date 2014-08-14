#include "wwd.h"
#include "errors.h"
#include <cassert>
#include <fstream>
#include <memory>

wap_wwd *wap_wwd_create()
{
    return new (std::nothrow) wap_wwd;
}

void wap_wwd_free(wap_wwd *wwd)
{
	delete wwd;
}

unsigned wap_wwd_get_checksum(const wap_wwd *wwd)
{
    return wwd->checksum;
}

unsigned wap_wwd_get_plane_count(const wap_wwd *wwd)
{
	return wwd->planes.size();
}

int wap_wwd_set_plane_count(wap_wwd *wwd, unsigned count)
{
    return wap::handle_exceptions([=](){
        wwd->planes.resize(count);
    });
}

wap_plane *wap_wwd_get_plane(wap_wwd *wwd, unsigned plane_index)
{
	return &wwd->planes[plane_index];
}

unsigned wap_wwd_get_tile_description_count(const wap_wwd *wwd)
{
    return wwd->tile_descriptions.size();
}

int wap_wwd_set_tile_description_count(wap_wwd *wwd, unsigned count)
{
    return wap::handle_exceptions([=](){
        wwd->tile_descriptions.resize(count);
    });
}

wap_tile_description *wap_wwd_get_tile_description(wap_wwd *wwd, unsigned description_index)
{
    return &wwd->tile_descriptions[description_index];
}

wap_wwd_properties *wap_wwd_get_properties(wap_wwd *wwd)
{
    return &wwd->properties;
}

void wap_plane_get_map_dimensions(wap_plane *plane, unsigned *w, unsigned *h)
{
    *w = plane->tiles_wide;
    *h = plane->tiles_high;
}

int wap_plane_set_map_dimensions(wap_plane *plane, unsigned w, unsigned h)
{
    return wap::handle_exceptions([=](){
        plane->tiles.resize(w * h);
        plane->tiles_wide = w;
        plane->tiles_high = h;
    });
}

unsigned wap_plane_get_tile(const wap_plane *plane, unsigned x, unsigned y)
{
    return plane->tiles[y * plane->tiles_wide + x];
}

void wap_plane_set_tile(wap_plane *plane, unsigned x, unsigned y, unsigned tile)
{
    plane->tiles[y * plane->tiles_wide + x] = tile;
}

unsigned wap_plane_get_image_set_count(const wap_plane *plane)
{
	return plane->image_sets.size();
}

int wap_plane_set_image_set_count(wap_plane *plane, unsigned count)
{
    return wap::handle_exceptions([=](){
        plane->image_sets.resize(count);
    });
}

const char *wap_plane_get_image_set(const wap_plane *plane, unsigned image_set_index)
{
	return plane->image_sets[image_set_index].c_str();
}

int wap_plane_set_image_set(wap_plane *plane, unsigned image_set_index, const char *image_set)
{
    return wap::handle_exceptions([=](){
        plane->image_sets[image_set_index].assign(image_set);
    });
}

unsigned wap_plane_get_object_count(const wap_plane *plane)
{
    return plane->objects.size();
}

int wap_plane_set_object_count(wap_plane *plane, unsigned count)
{
    return wap::handle_exceptions([=](){
        plane->objects.resize(count);
    });
}

wap_object *wap_plane_get_object(wap_plane *plane, unsigned object_index)
{
    return &plane->objects[object_index];
}

wap_plane_properties *wap_plane_get_properties(wap_plane *plane)
{
    return &plane->properties;
}

const char *wap_object_get_name(const wap_object *object)
{
    return object->name.c_str();
}

int wap_object_set_name(wap_object *object, const char *name)
{
    return wap::handle_exceptions([=](){
        object->name.assign(name);
    });
}

const char *wap_object_get_logic(const wap_object *object)
{
    return object->logic.c_str();
}

int wap_object_set_logic(wap_object *object, const char *logic)
{
    return wap::handle_exceptions([=](){
        object->logic.assign(logic);
    });
}

const char *wap_object_get_image_set(const wap_object *object)
{
    return object->image_set.c_str();
}

int wap_object_set_image_set(wap_object *object, const char *image_set)
{
    return wap::handle_exceptions([=](){
        object->image_set.assign(image_set);
    });
}

const char *wap_object_get_animation(const wap_object *object)
{
    return object->animation.c_str();
}

int wap_object_set_animation(wap_object *object, const char *animation)
{
    return wap::handle_exceptions([=](){
        object->animation.assign(animation);
    });
}

wap_object_properties *wap_object_get_properties(wap_object *object)
{
    return &object->properties;
}

