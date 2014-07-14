#ifndef wap32_wwd_h
#define wap32_wwd_h

#include "common.h"

WAP32_BEGIN_DECLS

typedef struct Wap32Wwd Wap32Wwd;
typedef struct Wap32WwdPlane Wap32WwdPlane;
typedef struct Wap32WwdObject Wap32WwdObject;

WAP32_API Wap32Wwd *wap32_wwd_create();

WAP32_API void wap32_wwd_free(Wap32Wwd *wwd);

WAP32_API int wap32_wwd_open(Wap32Wwd **out, const char *file_path);

// Flags

typedef enum {
    WAP32_WWD_USE_Z_COORDS = 0x1,
    WAP32_WWD_COMPRESS = 0x2
} Wap32WwdFlags;

typedef enum {
    WAP32_WWD_OBJECT_TYPE_GENERIC   = 1 << 0,
    WAP32_WWD_OBJECT_TYPE_PLAYER    = 1 << 1,
    WAP32_WWD_OBJECT_TYPE_ENEMY     = 1 << 2,
    WAP32_WWD_OBJECT_TYPE_POWERUP   = 1 << 3,
    WAP32_WWD_OBJECT_TYPE_SHOT      = 1 << 4,
    WAP32_WWD_OBJECT_TYPE_PSHOT     = 1 << 5,
    WAP32_WWD_OBJECT_TYPE_ESHOT     = 1 << 6,
    WAP32_WWD_OBJECT_TYPE_SPECIAL   = 1 << 7,
    WAP32_WWD_OBJECT_TYPE_USER1     = 1 << 8,
    WAP32_WWD_OBJECT_TYPE_USER2     = 1 << 9,
    WAP32_WWD_OBJECT_TYPE_USER3     = 1 << 10,
    WAP32_WWD_OBJECT_TYPE_USER4     = 1 << 11,
} Wap32ObjectType;

typedef enum {
    WAP32_WWD_OBJECT_USER_FLAG_1    = 1 << 0,
    WAP32_WWD_OBJECT_USER_FLAG_2    = 1 << 1,
    WAP32_WWD_OBJECT_USER_FLAG_3    = 1 << 2,
    WAP32_WWD_OBJECT_USER_FLAG_4    = 1 << 3,
    WAP32_WWD_OBJECT_USER_FLAG_5    = 1 << 4,
    WAP32_WWD_OBJECT_USER_FLAG_6    = 1 << 5,
    WAP32_WWD_OBJECT_USER_FLAG_7    = 1 << 6,
    WAP32_WWD_OBJECT_USER_FLAG_8    = 1 << 7,
    WAP32_WWD_OBJECT_USER_FLAG_9    = 1 << 8,
    WAP32_WWD_OBJECT_USER_FLAG_10   = 1 << 9,
    WAP32_WWD_OBJECT_USER_FLAG_11   = 1 << 10,
    WAP32_WWD_OBJECT_USER_FLAG_12   = 1 << 11,
} Wap32ObjectUserFlags;

typedef enum {
    WAP32_WWD_OBJECT_DRAW_FLAG_NODRAW   = 1 << 0,
    WAP32_WWD_OBJECT_DRAW_FLAG_MIRROR   = 1 << 1,
    WAP32_WWD_OBJECT_DRAW_FLAG_INVERT   = 1 << 2,
    WAP32_WWD_OBJECT_DRAW_FLAG_FLASH    = 1 << 3,
} Wap32ObjectDrawFlags;

typedef enum {
    WAP32_WWD_OBJECT_DYNAMIC_NO_HIT                 = 1 << 0,
    WAP32_WWD_OBJECT_DYNAMIC_ALWAYS_ACTIVE          = 1 << 1,
    WAP32_WWD_OBJECT_DYNAMIC_FLAG_SAFE              = 1 << 2,
    WAP32_WWD_OBJECT_DYNAMIC_FLAG_AUTO_HIT_DAMAGE   = 1 << 3,
} Wap32ObjectDynamicFlags;

typedef enum OBJ_ADD_FLAGS {
    WAP32_WWD_OBJECT_ADD_FLAG_DIFFICULT     = 1 << 0,
    WAP32_WWD_OBJECT_ADD_FLAG_EYE_CANDY     = 1 << 1,
    WAP32_WWD_OBJECT_ADD_FLAG_HIGH_DETAIL   = 1 << 2,
    WAP32_WWD_OBJECT_ADD_FLAG_MULTIPLAYER   = 1 << 3,
    WAP32_WWD_OBJECT_ADD_FLAG_EXTRA_MEMORY  = 1 << 4,
    WAP32_WWD_OBJECT_ADD_FLAG_FAST_CPU      = 1 << 5,
} Wap32ObjectAddFlags;

typedef enum {
    WAP32_PLANE_FLAG_MAIN_PLANE     = 1 << 0,
    WAP32_PLANE_FLAG_NO_DRAW        = 1 << 1,
    WAP32_PLANE_FLAG_X_WRAPPING     = 1 << 2,
    WAP32_PLANE_FLAG_Y_WRAPPING     = 1 << 3,
    WAP32_PLANE_FLAG_AUTO_TILE_SIZE = 1 << 4,
} Wap32PlaneFlags;

typedef enum {
    WAP32_WWD_TILE_ATTRIBUTE_CLEAR  = 0,
    WAP32_WWD_TILE_ATTRIBUTE_SOLID  = 1 << 0,
    WAP32_WWD_TILE_ATTRIBUTE_GROUND = 1 << 1,
    WAP32_WWD_TILE_ATTRIBUTE_CLIMB  = 1 << 2,
    WAP32_WWD_TILE_ATTRIBUTE_DEATH  = 1 << 3,
} Wap32TileAttribute;

typedef enum TILE_ATRIBTYPE {
    WAP32_WWD_TILE_TYPE_SINGLE = 1 << 0,
    WAP32_WWD_TILE_TYPE_DOUBLE = 1 << 1,
} Wap32TileType;


WAP32_API unsigned wap32_wwd_get_flags(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_flags(Wap32Wwd *wwd, unsigned flags);

WAP32_API const char *wap32_wwd_get_level_name(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_level_name(Wap32Wwd *wwd, const char *name);

WAP32_API const char *wap32_wwd_get_author(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_author(Wap32Wwd *wwd, const char *author);

WAP32_API const char *wap32_wwd_get_birth(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_birth(Wap32Wwd *wwd, const char *birth);

WAP32_API const char *wap32_wwd_get_rez_file(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_rez_file(Wap32Wwd *wwd, const char *rez_file);

WAP32_API const char *wap32_wwd_get_image_dir(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_image_dir(Wap32Wwd *wwd, const char *image_dir);

WAP32_API const char *wap32_wwd_get_pal_rez(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_pal_rez(Wap32Wwd *wwd, const char *pal_rez);

WAP32_API void wap32_wwd_set_start_position(Wap32Wwd *wwd, int x, int y);
WAP32_API void wap32_wwd_get_start_position(Wap32Wwd *wwd, int *x, int *y);

// Relative path to the game executable
WAP32_API const char *wap32_wwd_get_launch_app(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_launch_app(Wap32Wwd *wwd, const char *pal_rez);

// image_set_index and prefix_index are in {0, 1, 2, 3}
WAP32_API const char *wap32_wwd_get_image_set(Wap32Wwd *wwd, unsigned image_set_index);
WAP32_API void wap32_wwd_set_image_set(Wap32Wwd *wwd, unsigned image_set_index, const char *image_set);

WAP32_API const char *wap32_wwd_get_prefix(Wap32Wwd *wwd, unsigned prefix_index);
WAP32_API void wap32_wwd_set_prefix(Wap32Wwd *wwd, unsigned prefix_index, const char *prefix);

WAP32_API unsigned wap32_wwd_get_planes_count(Wap32Wwd *wwd);
WAP32_API void wap32_wwd_set_planes_count(Wap32Wwd *wwd, unsigned count);

WAP32_API Wap32WwdPlane *wap32_wwd_get_plane(Wap32Wwd *wwd, unsigned plane_index);

WAP32_API void wap32_wwd_set_level_name(Wap32Wwd *wwd, const char *name);

WAP32_API void wap32_wwd_set_author(Wap32Wwd *wwd, const char *author);

WAP32_API void wap32_wwd_set_birth(Wap32Wwd *wwd, const char *birth);

WAP32_API void wap32_wwd_set_rez_file(Wap32Wwd *wwd, const char *rez_file);

WAP32_API void wap32_wwd_set_image_dir(Wap32Wwd *wwd, const char *image_dir);

WAP32_API void wap32_wwd_set_pal_rez(Wap32Wwd *wwd, const char *pal_rez);

WAP32_API void wap32_wwd_set_start_position(Wap32Wwd *wwd, int x, int y);

WAP32_API void wap32_wwd_set_launch_app(Wap32Wwd *wwd, const char *pal_rez);

WAP32_API unsigned wap32_wwd_plane_get_object_count(Wap32WwdPlane *plane);
WAP32_API void wap32_wwd_plane_set_object_count(Wap32WwdPlane *plane, unsigned count);

WAP32_API Wap32WwdObject *wap32_wwd_plane_get_object(Wap32WwdPlane *plane, unsigned object_index);

WAP32_API unsigned wap32_wwd_plane_get_flags(Wap32WwdPlane *plane, unsigned flags);

WAP32_API const char *wap32_wwd_plane_get_name(Wap32WwdPlane *plane, const char *name);

WAP32_API void wap32_wwd_plane_get_tile_size(Wap32WwdPlane *plane, size_t *tile_width, size_t *tile_height);

WAP32_API void wap32_wwd_plane_get_dimensions(Wap32WwdPlane *plane, size_t *tiles_wide, size_t *tiles_high);

WAP32_API void wap32_wwd_plane_get_movement_percent(Wap32WwdPlane *plane, int *movement_x_percent, int *movement_y_percent);

WAP32_API unsigned wap32_wwd_plane_get_fill_color(Wap32WwdPlane *plane);

WAP32_API unsigned wap32_wwd_plane_get_z_coord(Wap32WwdPlane *plane);

WAP32_API size_t wap32_wwd_plane_get_image_sets_count(Wap32WwdPlane *plane);

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