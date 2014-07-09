#include "wwd.h"

// Wap32Wwd getters

unsigned wap32_wwd_get_flags(Wap32Wwd *wwd)
{
	return wwd->data.flags;
}

const char *wap32_wwd_get_level_name(Wap32Wwd *wwd)
{
	return wwd->data.level_name;
}

const char *wap32_wwd_get_author(Wap32Wwd *wwd)
{
	return wwd->data.author;
}

const char *wap32_wwd_get_birth(Wap32Wwd *wwd)
{
	return wwd->data.birth;
}

const char *wap32_wwd_get_rez_file(Wap32Wwd *wwd)
{
	return wwd->data.rez_file;
}

const char *wap32_wwd_get_image_dir(Wap32Wwd *wwd)
{
	return wwd->data.image_dir;
}

const char *wap32_wwd_get_pal_rez(Wap32Wwd *wwd)
{
	return wwd->data.pal_rez;
}

void wap32_wwd_get_start_position(Wap32Wwd *wwd, int *x, int *y)
{
	*x = wwd->data.start_x;
	*y = wwd->data.start_y;
}

const char *wap32_wwd_get_launch_app(Wap32Wwd *wwd)
{
	return wwd->data.launch_app;
}

const char *wap32_wwd_get_image_set(Wap32Wwd *wwd, unsigned index)
{
	return wwd->data.image_sets[index];
}

const char *wap32_wwd_get_prefix(Wap32Wwd *wwd, unsigned index)
{
	return wwd->data.prefixes[index];
}

size_t wap32_wwd_get_planes_count(Wap32Wwd *wwd)
{
	return wwd->planes.size();
}

Wap32WwdPlane *wap32_wwd_get_plane(Wap32Wwd *wwd, unsigned index)
{
	return &wwd->planes[index];
}

// Wap32Wwd setters

template<size_t size>
static void set_char_buffer(char(&buffer)[size], const char *source)
{
	strncpy(buffer, source, size - 1);
	buffer[size - 1] = '\0';
}

void wap32_wwd_set_flags(Wap32Wwd *wwd, unsigned flags)
{
	wwd->data.flags = flags;
}

void wap32_wwd_set_level_name(Wap32Wwd *wwd, const char *name)
{
	set_char_buffer(wwd->data.level_name, name);
}

void wap32_wwd_set_author(Wap32Wwd *wwd, const char *author)
{
	set_char_buffer(wwd->data.author, author);
}

void wap32_wwd_set_birth(Wap32Wwd *wwd, const char *birth)
{
	set_char_buffer(wwd->data.birth, birth);
}

void wap32_wwd_set_rez_file(Wap32Wwd *wwd, const char *rez_file)
{
	set_char_buffer(wwd->data.rez_file, rez_file);
}

void wap32_wwd_set_image_dir(Wap32Wwd *wwd, const char *image_dir)
{
	set_char_buffer(wwd->data.image_dir, image_dir);
}

void wap32_wwd_set_pal_rez(Wap32Wwd *wwd, const char *pal_rez)
{
	set_char_buffer(wwd->data.pal_rez, pal_rez);
}

void wap32_wwd_set_start_position(Wap32Wwd *wwd, int x, int y)
{
	wwd->data.start_x = x;
	wwd->data.start_y = y;
}

void wap32_wwd_set_launch_app(Wap32Wwd *wwd, const char *launch_app)
{
	set_char_buffer(wwd->data.launch_app, launch_app);
}

void wap32_wwd_set_image_set(Wap32Wwd *wwd, unsigned image_wap32_wwd_set_index, const char *image_set)
{
	set_char_buffer(wwd->data.image_sets[image_wap32_wwd_set_index], image_set);
}

void wap32_wwd_set_prefix(Wap32Wwd *wwd, unsigned prefix_index, const char *prefix)
{
	set_char_buffer(wwd->data.prefixes[prefix_index], prefix);
}

void wap32_wwd_set_planes_count(Wap32Wwd *wwd, unsigned count)
{
	wwd->planes.resize(count);
}

// Wap32WwdPlane getters

unsigned wap32_wwd_plane_get_flags(Wap32WwdPlane *plane, unsigned flags)
{
	return plane->data.flags;
}

const char *wap32_wwd_plane_get_name(Wap32WwdPlane *plane, const char *name)
{
	return plane->data.name;
}

void wap32_wwd_plane_get_tile_size(Wap32WwdPlane *plane, size_t *tile_width, size_t *tile_height)
{
	*tile_width = plane->data.tile_width;
	*tile_height = plane->data.tile_height;
}

void wap32_wwd_plane_get_dimensions(Wap32WwdPlane *plane, size_t *tiles_wide, size_t *tiles_high)
{
	*tiles_wide = plane->data.tiles_wide;
	*tiles_high = plane->data.tiles_high;
}

void wap32_wwd_plane_get_movement_percent(Wap32WwdPlane *plane, int *movement_x_percent, int *movement_y_percent)
{
	*movement_x_percent = plane->data.movement_x_percent;
	*movement_y_percent = plane->data.movement_y_percent;
}

unsigned wap32_wwd_plane_get_fill_color(Wap32WwdPlane *plane)
{
	return plane->data.fill_color;
}

unsigned wap32_wwd_plane_get_z_coord(Wap32WwdPlane *plane)
{
	return plane->data.z_coord;
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
	plane->data.flags = flags;
}

void wap32_wwd_plane_set_name(Wap32WwdPlane *plane, const char *name)
{
	set_char_buffer(plane->data.name, name);
}

void wap32_wwd_plane_set_tile_size(Wap32WwdPlane *plane, size_t tile_width, size_t tile_height)
{
	plane->data.tile_width = tile_width;
	plane->data.tile_height = tile_height;
}

void wap32_wwd_plane_set_dimensions(Wap32WwdPlane *plane, size_t tiles_wide, size_t tiles_high)
{
	plane->data.tiles_wide = tiles_wide;
	plane->data.tiles_high = tiles_high;
}

void wap32_wwd_plane_set_movement_percent(Wap32WwdPlane *plane, int movement_x_percent, int movement_y_percent)
{
	plane->data.movement_x_percent = movement_x_percent;
	plane->data.movement_y_percent = movement_y_percent;
}

void wap32_wwd_plane_set_fill_color(Wap32WwdPlane *plane, unsigned fill_color)
{
	plane->data.fill_color = fill_color;
}

void wap32_wwd_plane_set_z_coord(Wap32WwdPlane *plane, unsigned z_coord)
{
	plane->data.z_coord = z_coord;
}

void wap32_wwd_plane_set_image_sets_count(Wap32WwdPlane *plane, unsigned count)
{
	plane->image_sets.resize(count);
}

void wap32_wwd_plane_set_image_set(Wap32WwdPlane *plane, unsigned image_set_index, const char *image_set)
{
	plane->image_sets[image_set_index] = image_set;
}

