#include "wwd.h"
#include "errors.h"
#include <cassert>
#include <fstream>
#include <memory>

Wap32Wwd *wap32_wwd_create()
{
    return new (std::nothrow) Wap32Wwd;
}

void wap32_wwd_free(Wap32Wwd *wwd)
{
	delete wwd;
}

int wap32_wwd_open(Wap32Wwd **out, const char *file_path)
{
    try {
        Wap32ErrorContext errctx("opening file '%s'", file_path);
        *out = nullptr;
        std::unique_ptr<Wap32Wwd> wwd { new Wap32Wwd };
        std::ifstream file(file_path, std::ios::binary);
        std::vector<char> wwd_buffer((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        if(!file.good())
            return WAP32_EFILE;
        int error = wap32_wwd__read(*wwd.get(), wwd_buffer);
        if(error < 0)
            return error;
        *out = wwd.release();
        return WAP32_OK;
    } catch(std::bad_alloc&) {
        return WAP32_ENOMEMORY;
    }
}

size_t wap32_wwd_get_planes_count(const Wap32Wwd *wwd)
{
	return wwd->planes.size();
}

void wap32_wwd_set_planes_count(Wap32Wwd *wwd, size_t count)
{
	wwd->planes.resize(count);
}

Wap32Plane *wap32_wwd_get_plane(Wap32Wwd *wwd, size_t plane_index)
{
	return &wwd->planes[plane_index];
}

Wap32WwdProperties *wap32_wwd_get_properties(Wap32Wwd *wwd)
{
    return &wwd->properties;
}

size_t wap32_plane_get_image_sets_count(const Wap32Plane *plane)
{
	return plane->image_sets.size();
}

void wap32_plane_set_image_sets_count(Wap32Plane *plane, size_t count)
{
	plane->image_sets.resize(count);
}

const char *wap32_plane_get_image_set(const Wap32Plane *plane, size_t image_set_index)
{
	return plane->image_sets[image_set_index].c_str();
}

void wap32_plane_set_image_set(Wap32Plane *plane, size_t image_set_index, const char *image_set)
{
    plane->image_sets[image_set_index] = image_set;
}

size_t wap32_plane_get_object_count(const Wap32Plane *plane)
{
    return plane->objects.size();
}

void wap32_plane_set_object_count(Wap32Plane *plane, size_t count)
{
    plane->objects.resize(count);
}

Wap32Object *wap32_plane_get_object(Wap32Plane *plane, size_t object_index)
{
    return &plane->objects[object_index];
}

Wap32PlaneProperties *wap32_plane_get_properties(Wap32Plane *plane)
{
    return &plane->properties;
}

const char *wap32_object_get_name(const Wap32Object *object)
{
    return object->name.c_str();
}

void wap32_object_set_name(Wap32Object *object, const char *name)
{
    object->name.assign(name);
}

const char *wap32_object_get_logic(const Wap32Object *object)
{
    return object->logic.c_str();
}

void wap32_object_set_logic(Wap32Object *object, const char *logic)
{
    object->logic.assign(logic);
}

const char *wap32_object_get_image_set(const Wap32Object *object)
{
    return object->image_set.c_str();
}

void wap32_object_set_image_set(Wap32Object *object, const char *image_set)
{
    object->image_set.assign(image_set);
}

const char *wap32_object_get_animation(const Wap32Object *object)
{
    return object->animation.c_str();
}

void wap32_object_set_animation(Wap32Object *object, const char *animation)
{
    object->animation.assign(animation);
}

Wap32ObjectProperties *wap32_object_get_properties(Wap32Object *object)
{
    return &object->properties;
}

