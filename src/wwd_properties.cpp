#include "wwd.h"
#include "utils.h"

// Wap32Wwd getters

unsigned wap32_wwd_get_flags(Wap32Wwd *wwd)
{
	return wwd->flags;
}

const char *wap32_wwd_get_level_name(Wap32Wwd *wwd)
{
	return wwd->level_name;
}

const char *wap32_wwd_get_author(Wap32Wwd *wwd)
{
	return wwd->author;
}

const char *wap32_wwd_get_birth(Wap32Wwd *wwd)
{
	return wwd->birth;
}

const char *wap32_wwd_get_rez_file(Wap32Wwd *wwd)
{
	return wwd->rez_file;
}

const char *wap32_wwd_get_image_dir(Wap32Wwd *wwd)
{
	return wwd->image_dir;
}

const char *wap32_wwd_get_pal_rez(Wap32Wwd *wwd)
{
	return wwd->pal_rez;
}

void wap32_wwd_get_start_position(Wap32Wwd *wwd, int *x, int *y)
{
	*x = wwd->start_x;
	*y = wwd->start_y;
}

const char *wap32_wwd_get_launch_app(Wap32Wwd *wwd)
{
	return wwd->launch_app;
}

const char *wap32_wwd_get_image_set(Wap32Wwd *wwd, unsigned index)
{
	return wwd->image_sets[index];
}

const char *wap32_wwd_get_prefix(Wap32Wwd *wwd, unsigned index)
{
	return wwd->prefixes[index];
}

unsigned wap32_wwd_get_planes_count(Wap32Wwd *wwd)
{
	return wwd->planes.size();
}

Wap32WwdPlane *wap32_wwd_get_plane(Wap32Wwd *wwd, unsigned index)
{
	return &wwd->planes[index];
}

// Wap32Wwd setters

void wap32_wwd_set_flags(Wap32Wwd *wwd, unsigned flags)
{
	wwd->flags = flags;
}

void wap32_wwd_set_level_name(Wap32Wwd *wwd, const char *name)
{
	wap32_util_buffer_assign(wwd->level_name, name);
}

void wap32_wwd_set_author(Wap32Wwd *wwd, const char *author)
{
	wap32_util_buffer_assign(wwd->author, author);
}

void wap32_wwd_set_birth(Wap32Wwd *wwd, const char *birth)
{
	wap32_util_buffer_assign(wwd->birth, birth);
}

void wap32_wwd_set_rez_file(Wap32Wwd *wwd, const char *rez_file)
{
	wap32_util_buffer_assign(wwd->rez_file, rez_file);
}

void wap32_wwd_set_image_dir(Wap32Wwd *wwd, const char *image_dir)
{
	wap32_util_buffer_assign(wwd->image_dir, image_dir);
}

void wap32_wwd_set_pal_rez(Wap32Wwd *wwd, const char *pal_rez)
{
	wap32_util_buffer_assign(wwd->pal_rez, pal_rez);
}

void wap32_wwd_set_start_position(Wap32Wwd *wwd, int x, int y)
{
	wwd->start_x = x;
	wwd->start_y = y;
}

void wap32_wwd_set_launch_app(Wap32Wwd *wwd, const char *launch_app)
{
	wap32_util_buffer_assign(wwd->launch_app, launch_app);
}

void wap32_wwd_set_image_set(Wap32Wwd *wwd, unsigned image_wap32_wwd_set_index, const char *image_set)
{
	wap32_util_buffer_assign(wwd->image_sets[image_wap32_wwd_set_index], image_set);
}

void wap32_wwd_set_prefix(Wap32Wwd *wwd, unsigned prefix_index, const char *prefix)
{
	wap32_util_buffer_assign(wwd->prefixes[prefix_index], prefix);
}

void wap32_wwd_set_planes_count(Wap32Wwd *wwd, unsigned count)
{
	wwd->planes.resize(count);
}

// Wap32WwdPlane getters

unsigned wap32_wwd_plane_get_flags(Wap32WwdPlane *plane, unsigned flags)
{
	return plane->flags;
}

const char *wap32_wwd_plane_get_name(Wap32WwdPlane *plane, const char *name)
{
	return plane->name;
}

void wap32_wwd_plane_get_tile_size(Wap32WwdPlane *plane, size_t *tile_width, size_t *tile_height)
{
	*tile_width = plane->tile_width;
	*tile_height = plane->tile_height;
}

void wap32_wwd_plane_get_dimensions(Wap32WwdPlane *plane, size_t *tiles_wide, size_t *tiles_high)
{
	*tiles_wide = plane->tiles_wide;
	*tiles_high = plane->tiles_high;
}

void wap32_wwd_plane_get_movement_percent(Wap32WwdPlane *plane, int *movement_x_percent, int *movement_y_percent)
{
	*movement_x_percent = plane->movement_x_percent;
	*movement_y_percent = plane->movement_y_percent;
}

unsigned wap32_wwd_plane_get_fill_color(Wap32WwdPlane *plane)
{
	return plane->fill_color;
}

unsigned wap32_wwd_plane_get_z_coord(Wap32WwdPlane *plane)
{
	return plane->z_coord;
}

size_t wap32_wwd_plane_get_image_sets_count(Wap32WwdPlane *plane)
{
	return plane->image_sets.size();
}

const char *wap32_wwd_plane_get_image_set(Wap32WwdPlane *plane, unsigned image_set_index)
{
	return plane->image_sets[image_set_index].c_str();
}

// Wap32WwdPlane setters

void wap32_wwd_plane_set_flags(Wap32WwdPlane *plane, unsigned flags)
{
	plane->flags = flags;
}

void wap32_wwd_plane_set_name(Wap32WwdPlane *plane, const char *name)
{
	wap32_util_buffer_assign(plane->name, name);
}

void wap32_wwd_plane_set_tile_size(Wap32WwdPlane *plane, size_t tile_width, size_t tile_height)
{
	plane->tile_width = tile_width;
	plane->tile_height = tile_height;
}

void wap32_wwd_plane_set_dimensions(Wap32WwdPlane *plane, size_t tiles_wide, size_t tiles_high)
{
	plane->tiles_wide = tiles_wide;
	plane->tiles_high = tiles_high;
}

void wap32_wwd_plane_set_movement_percent(Wap32WwdPlane *plane, int movement_x_percent, int movement_y_percent)
{
	plane->movement_x_percent = movement_x_percent;
	plane->movement_y_percent = movement_y_percent;
}

void wap32_wwd_plane_set_fill_color(Wap32WwdPlane *plane, unsigned fill_color)
{
	plane->fill_color = fill_color;
}

void wap32_wwd_plane_set_z_coord(Wap32WwdPlane *plane, unsigned z_coord)
{
	plane->z_coord = z_coord;
}

void wap32_wwd_plane_set_image_sets_count(Wap32WwdPlane *plane, unsigned count)
{
	plane->image_sets.resize(count);
}

void wap32_wwd_plane_set_image_set(Wap32WwdPlane *plane, unsigned image_set_index, const char *image_set)
{
    plane->image_sets[image_set_index] = image_set;
}

unsigned wap32_wwd_plane_get_object_count(Wap32WwdPlane *plane)
{
    return plane->objects.size();
}

void wap32_wwd_plane_set_object_count(Wap32WwdPlane *plane, unsigned count)
{
    plane->objects.resize(count);
}

Wap32WwdObject *wap32_wwd_plane_get_object(Wap32WwdPlane *plane, unsigned object_index)
{
    return &plane->objects[object_index];
}


