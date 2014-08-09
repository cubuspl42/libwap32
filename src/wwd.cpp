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

size_t wap_wwd_get_plane_count(const wap_wwd *wwd)
{
	return wwd->planes.size();
}

void wap_wwd_set_plane_count(wap_wwd *wwd, size_t count)
{
	wwd->planes.resize(count);
}

wap_plane *wap_wwd_get_plane(wap_wwd *wwd, size_t plane_index)
{
	return &wwd->planes[plane_index];
}

size_t wap_wwd_get_tile_description_count(const wap_wwd *wwd)
{
    return wwd->tile_descriptions.size();
}

wap_tile_description *wap_wwd_get_tile_description(wap_wwd *wwd, size_t description_index)
{
    return &wwd->tile_descriptions[description_index];
}

wap_wwd_properties *wap_wwd_get_properties(wap_wwd *wwd)
{
    return &wwd->properties;
}

size_t wap_plane_get_tile_count(const wap_plane *plane)
{
    return plane->tiles.size();
}

void wap_plane_set_tile_count(wap_plane *plane, size_t count)
{
    plane->tiles.resize(count);
}

unsigned wap_plane_get_tile(const wap_plane *plane, size_t tile_index)
{
    return plane->tiles[tile_index];
}

size_t wap_plane_get_image_set_count(const wap_plane *plane)
{
	return plane->image_sets.size();
}

void wap_plane_set_image_set_count(wap_plane *plane, size_t count)
{
	plane->image_sets.resize(count);
}

const char *wap_plane_get_image_set(const wap_plane *plane, size_t image_set_index)
{
	return plane->image_sets[image_set_index].c_str();
}

void wap_plane_set_image_set(wap_plane *plane, size_t image_set_index, const char *image_set)
{
    plane->image_sets[image_set_index].assign(image_set);
}

size_t wap_plane_get_object_count(const wap_plane *plane)
{
    return plane->objects.size();
}

void wap_plane_set_object_count(wap_plane *plane, size_t count)
{
    plane->objects.resize(count);
}

wap_object *wap_plane_get_object(wap_plane *plane, size_t object_index)
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

void wap_object_set_name(wap_object *object, const char *name)
{
    object->name.assign(name);
}

const char *wap_object_get_logic(const wap_object *object)
{
    return object->logic.c_str();
}

void wap_object_set_logic(wap_object *object, const char *logic)
{
    object->logic.assign(logic);
}

const char *wap_object_get_image_set(const wap_object *object)
{
    return object->image_set.c_str();
}

void wap_object_set_image_set(wap_object *object, const char *image_set)
{
    object->image_set.assign(image_set);
}

const char *wap_object_get_animation(const wap_object *object)
{
    return object->animation.c_str();
}

void wap_object_set_animation(wap_object *object, const char *animation)
{
    object->animation.assign(animation);
}

wap_object_properties *wap_object_get_properties(wap_object *object)
{
    return &object->properties;
}

