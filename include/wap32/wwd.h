#ifndef wap32_wwd_h
#define wap32_wwd_h

#include "common.h"

WAP32_BEGIN_DECLS

typedef struct Wap32Wwd Wap32Wwd;
typedef struct Wap32WwdPlane Wap32WwdPlane;
typedef struct Wap32WwdObject Wap32WwdObject;

WAP32_API int wap32_wwd_open(Wap32Wwd **out, const char *file_path);
WAP32_API void wap32_wwd_free(Wap32Wwd *wwd);

WAP32_API unsigned wap32_wwd_get_flags(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_level_name(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_author(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_birth(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_rez_file(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_image_dir(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_pal_rez(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_get_start_position(Wap32Wwd *wwd, int *x, int *y);
WAP32_API const char *wap32_wwd_get_launch_app(Wap32Wwd *wwd);
WAP32_API const char *wap32_wwd_get_image_set(Wap32Wwd *wwd, unsigned image_wap32_wwd_set_index);
WAP32_API const char *wap32_wwd_get_prefix(Wap32Wwd *wwd, unsigned prefix_index);
WAP32_API size_t wap32_wwd_get_planes_count(Wap32Wwd *wwd);
WAP32_API Wap32WwdPlane *wap32_wwd_get_plane(Wap32Wwd *wwd, unsigned plane_index);

WAP32_API void wap32_wwd_set_flags(Wap32Wwd *wwd, unsigned flags);
WAP32_API void wap32_wwd_set_level_name(Wap32Wwd *wwd, const char *name);
WAP32_API void wap32_wwd_set_author(Wap32Wwd *wwd, const char *author);
WAP32_API void wap32_wwd_set_birth(Wap32Wwd *wwd, const char *birth);
WAP32_API void wap32_wwd_set_rez_file(Wap32Wwd *wwd, const char *rez_file);
WAP32_API void wap32_wwd_set_image_dir(Wap32Wwd *wwd, const char *image_dir);
WAP32_API void wap32_wwd_set_pal_rez(Wap32Wwd *wwd, const char *pal_rez);
WAP32_API void wap32_wwd_set_start_position(Wap32Wwd *wwd, int x, int y);
WAP32_API void wap32_wwd_set_launch_app(Wap32Wwd *wwd, const char *pal_rez);
WAP32_API void wap32_wwd_set_image_set(Wap32Wwd *wwd, unsigned image_wap32_wwd_set_index, const char *image_set);
WAP32_API void wap32_wwd_set_prefix(Wap32Wwd *wwd, unsigned prefix_index, const char *prefix);
WAP32_API void wap32_wwd_set_planes_count(Wap32Wwd *wwd, unsigned count);

WAP32_API unsigned wap32_wwd_plane_get_flags(Wap32WwdPlane *plane, unsigned flags);
WAP32_API const char *wap32_wwd_plane_get_name(Wap32WwdPlane *plane, const char *name);
WAP32_API void wap32_wwd_plane_get_tile_size(Wap32WwdPlane *plane, size_t *tile_width, size_t *tile_height);
WAP32_API void wap32_wwd_plane_get_dimensions(Wap32WwdPlane *plane, size_t *tiles_wide, size_t *tiles_high);
WAP32_API void wap32_wwd_plane_get_movement_percent(Wap32WwdPlane *plane, int *movement_x_percent, int *movement_y_percent);
WAP32_API unsigned wap32_wwd_plane_get_fill_color(Wap32WwdPlane *plane);
WAP32_API unsigned wap32_wwd_plane_get_z_coord(Wap32WwdPlane *plane);
WAP32_API unsigned wap32_wwd_plane_get_image_sets_count(Wap32WwdPlane *plane);
WAP32_API const char *wap32_wwd_plane_get_image_set(Wap32WwdPlane *plane, unsigned image_set_index);

WAP32_API void wap32_wwd_plane_set_flags(Wap32WwdPlane *plane, unsigned flags);
WAP32_API void wap32_wwd_plane_set_name(Wap32WwdPlane *plane, const char *name);
WAP32_API void wap32_wwd_plane_set_tile_size(Wap32WwdPlane *plane, size_t tile_width, size_t tile_height);
WAP32_API void wap32_wwd_plane_set_dimensions(Wap32WwdPlane *plane, size_t tiles_wide, size_t tiles_high);
WAP32_API void wap32_wwd_plane_set_movement_percent(Wap32WwdPlane *plane, int movement_x_percent, int movement_y_percent);
WAP32_API void wap32_wwd_plane_set_fill_color(Wap32WwdPlane *plane, unsigned fill_color);
WAP32_API void wap32_wwd_plane_set_z_coord(Wap32WwdPlane *plane, unsigned z_coord);
WAP32_API void wap32_wwd_plane_set_image_sets_count(Wap32WwdPlane *plane, unsigned count);
WAP32_API void wap32_wwd_plane_set_image_set(Wap32WwdPlane *plane, unsigned image_set_index, const char *image_set);

WAP32_END_DECLS

#endif