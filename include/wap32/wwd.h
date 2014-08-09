#ifndef wap_wwd_h
#define wap_wwd_h

#include "common.h"
#include "buffer.h"

#include <stdint.h>

WAP_BEGIN_DECLS

enum {
    WAP_WWD_FLAG_USE_Z_COORDS = 1 << 0,
    WAP_WWD_FLAG_COMPRESS     = 1 << 1,
};

enum {
    WAP_OBJECT_ADD_FLAG_DIFFICULT       = 1 << 0,
    WAP_OBJECT_ADD_FLAG_EYE_CANDY       = 1 << 1,
    WAP_OBJECT_ADD_FLAG_HIGH_DETAIL     = 1 << 2,
    WAP_OBJECT_ADD_FLAG_MULTIPLAYER     = 1 << 3,
    WAP_OBJECT_ADD_FLAG_EXTRA_MEMORY    = 1 << 4,
    WAP_OBJECT_ADD_FLAG_FAST_CPU        = 1 << 5,
};

enum {
    WAP_PLANE_FLAG_MAIN_PLANE       = 1 << 0, /* Only one plane should hold this flag */
    WAP_PLANE_FLAG_NO_DRAW          = 1 << 1,
    WAP_PLANE_FLAG_X_WRAPPING       = 1 << 2,
    WAP_PLANE_FLAG_Y_WRAPPING       = 1 << 3,
    WAP_PLANE_FLAG_AUTO_TILE_SIZE   = 1 << 4,
};

enum {
    WAP_OBJECT_DRAW_FLAG_NO_DRAW    = 1 << 0,
    WAP_OBJECT_DRAW_FLAG_MIRROR     = 1 << 1,
    WAP_OBJECT_DRAW_FLAG_INVERT     = 1 << 2,
    WAP_OBJECT_DRAW_FLAG_FLASH      = 1 << 3,
};

enum {
    WAP_OBJECT_DYNAMIC_FLAG_NO_HIT          = 1 << 0,
    WAP_OBJECT_DYNAMIC_FLAG_ALWAYS_ACTIVE   = 1 << 1,
    WAP_OBJECT_DYNAMIC_FLAG_SAFE            = 1 << 2,
    WAP_OBJECT_DYNAMIC_FLAG_AUTO_HIT_DAMAGE = 1 << 3,
};

enum {
    WAP_OBJECT_USER_FLAG_1  = 1 << 0,
    WAP_OBJECT_USER_FLAG_2  = 1 << 1,
    WAP_OBJECT_USER_FLAG_3  = 1 << 2,
    WAP_OBJECT_USER_FLAG_4  = 1 << 3,
    WAP_OBJECT_USER_FLAG_5  = 1 << 4,
    WAP_OBJECT_USER_FLAG_6  = 1 << 5,
    WAP_OBJECT_USER_FLAG_7  = 1 << 6,
    WAP_OBJECT_USER_FLAG_8  = 1 << 7,
    WAP_OBJECT_USER_FLAG_9  = 1 << 8,
    WAP_OBJECT_USER_FLAG_10 = 1 << 9,
    WAP_OBJECT_USER_FLAG_11 = 1 << 10,
    WAP_OBJECT_USER_FLAG_12 = 1 << 11,
};

enum {
    WAP_OBJECT_TYPE_GENERIC = 1 << 0,
    WAP_OBJECT_TYPE_PLAYER  = 1 << 1,
    WAP_OBJECT_TYPE_ENEMY   = 1 << 2,
    WAP_OBJECT_TYPE_POWERUP = 1 << 3,
    WAP_OBJECT_TYPE_SHOT    = 1 << 4,
    WAP_OBJECT_TYPE_PSHOT   = 1 << 5,
    WAP_OBJECT_TYPE_ESHOT   = 1 << 6,
    WAP_OBJECT_TYPE_SPECIAL = 1 << 7,
    WAP_OBJECT_TYPE_USER1   = 1 << 8,
    WAP_OBJECT_TYPE_USER2   = 1 << 9,
    WAP_OBJECT_TYPE_USER3   = 1 << 10,
    WAP_OBJECT_TYPE_USER4   = 1 << 11,
};

enum {
    WAP_TILE_TYPE_SINGLE    = 1,
    WAP_TILE_TYPE_DOUBLE    = 2,
};

enum {
    WAP_TILE_ATTRIBUTE_CLEAR    = 0,
    WAP_TILE_ATTRIBUTE_SOLID    = 1 << 0,
    WAP_TILE_ATTRIBUTE_GROUND   = 1 << 1,
    WAP_TILE_ATTRIBUTE_CLIMB    = 1 << 2,
    WAP_TILE_ATTRIBUTE_DEATH    = 1 << 3,
};

typedef struct wap_wwd wap_wwd;
typedef struct wap_plane wap_plane;
typedef struct wap_object wap_object;

typedef struct {
	uint32_t left;
	uint32_t top;
	uint32_t right;
	uint32_t bottom;
} wap_rect;

typedef struct {
    uint32_t flags; /* WAP_WWD_FLAG_ */
    /* WapWorld expects all these char buffers to be null-terminated */
    char level_name[64];
    char author[64];
    char birth[64];
    char rez_file[256];
    char image_dir[128];
    char pal_rez[128];
    int32_t start_x;
    int32_t start_y;
    char launch_app[128];
    char image_sets[4][128];
    char prefixes[4][32];
} wap_wwd_properties;

typedef struct {
    uint32_t flags; /* WAP_PLANE_FLAG_ flags */
    char name[64]; /* WapWorld expects this char buffer to be null-terminated */
    uint32_t tile_width; /* tile's width in pixels */
    uint32_t tile_height; /* tile's height in pixels */
    int32_t movement_x_percent;
    int32_t movement_y_percent;
    uint32_t fill_color; /* TODO: introduce wap_color struct */
    int32_t z_coord;
} wap_plane_properties;

typedef struct {
    int32_t id; /* Any value is accepted by WapWorld, but a good id should be positive and unique. */
    int32_t x;
    int32_t y;
    int32_t z;
    int32_t i;
    uint32_t add_flags; /* WAP_OBJECT_ADD_FLAG_ flags */
    uint32_t dynamic_flags; /* WAP_OBJECT_DYNAMIC_FLAG_ flags */
    uint32_t draw_flags; /* WAP_OBJECT_DRAW_FLAG_ flags */
    uint32_t user_flags; /* WAP_OBJECT_USER_FLAG_ flags */
    int32_t score;
    int32_t points;
    int32_t powerup;
    int32_t damage;
    int32_t smarts;
    int32_t health;
    wap_rect move_rect;
    wap_rect hit_rect;
    wap_rect attack_rect;
    wap_rect clip_rect;
    wap_rect user_rects[2];
    int32_t user_values[8];
    int32_t x_min;
    int32_t y_min;
    int32_t x_max;
    int32_t y_max;
    int32_t speed_x;
    int32_t speed_y;
    int32_t x_tweak;
    int32_t y_tweak;
    int32_t counter;
    int32_t speed;
    int32_t width;
    int32_t height;
    int32_t direction;
    int32_t face_dir;
    int32_t time_delay;
    int32_t frame_delay;
    uint32_t object_type; /* WAP_OBJECT_TYPE_ single value */
    uint32_t hit_type_flags; /* WAP_OBJECT_TYPE_ flags */
    int32_t x_move_res;
    int32_t y_move_res;
} wap_object_properties;

typedef struct {
    uint32_t type; /* WAP_TILE_TYPE_ single value */
    uint32_t width; /* in pixels */
    uint32_t height; /* in pixels */
    uint32_t inside_attrib; /* WAP_TILE_ATTRIBUTE_ */
    /* outside_attrib and rect only if type == WAP_TILE_TYPE_DOUBLE */
    uint32_t outside_attrib; /* WAP_TILE_ATTRIBUTE_ */
    wap_rect rect;
} wap_tile_description;

/* NULL if out of memory */
WAP_API wap_wwd *wap_wwd_create();

WAP_API void wap_wwd_free(wap_wwd *wwd);

/* Possible errors: WAP_ERROR, WAP_ENOMEMORY, WAP_EINVALIDDATA */
WAP_API int wap_wwd_read(wap_wwd *wwd, const char *wwd_buffer, size_t wwd_buffer_size);

/* Possible errors: WAP_ERROR, WAP_ENOMEMORY, WAP_EFILE, WAP_EINVALIDDATA */
WAP_API int wap_wwd_open(wap_wwd *wwd, const char *file_path);

/* Possible errors: WAP_ERROR, WAP_ENOMEMORY, */
WAP_API int wap_wwd_write(const wap_wwd *wwd, wap_buffer *out_wwd_buffer);

/* Possible errors: WAP_ERROR, WAP_ENOMEMORY, WAP_EFILE */
WAP_API int wap_wwd_save(const wap_wwd *wwd, const char *file_path);

/* Get the checksum calculated by a program that created the WWD buffer that was read using wap_wwd_read or wap_wwd_open.
 * Returns zero if no such buffer was read.
 */
WAP_API uint32_t wap_wwd_get_checksum(const wap_wwd *wwd);

WAP_API uint32_t wap_wwd_get_plane_count(const wap_wwd *wwd);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_wwd_set_plane_count(wap_wwd *wwd, uint32_t count);

WAP_API wap_plane *wap_wwd_get_plane(wap_wwd *wwd, uint32_t plane_index);

WAP_API uint32_t wap_wwd_get_tile_description_count(const wap_wwd *wwd);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_wwd_set_tile_description_count(const wap_wwd *wwd, uint32_t count);

WAP_API wap_tile_description *wap_wwd_get_tile_description(wap_wwd *wwd, uint32_t description_index);

WAP_API wap_wwd_properties *wap_wwd_get_properties(wap_wwd *wwd);

WAP_API void wap_plane_get_map_dimensions(wap_plane *plane, uint32_t *w, uint32_t *h);

/* Note: after changing map's dimensions, value returned by wap_plane_get_tile(plane, x, y) is undefined for any x, y until you
 * call wap_plane_set_tile(plane, x, y, tile) or read a new buffer with wap_wwd_read or wap_wwd_open.
 * Possible error: WAP_ENOMEMORY
 */
WAP_API int wap_plane_set_map_dimensions(wap_plane *plane, uint32_t w, uint32_t h);

WAP_API uint32_t wap_plane_get_tile(const wap_plane *plane, uint32_t x, uint32_t y);

WAP_API void wap_plane_set_tile(wap_plane *plane, uint32_t x, uint32_t y, uint32_t tile);

WAP_API uint32_t wap_plane_get_image_set_count(const wap_plane *plane);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_plane_set_image_set_count(wap_plane *plane, uint32_t count);

WAP_API const char *wap_plane_get_image_set(const wap_plane *plane, uint32_t image_set_index);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_plane_set_image_set(wap_plane *plane, uint32_t image_set_index, const char *image_set);

WAP_API uint32_t wap_plane_get_object_count(const wap_plane *plane);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_plane_set_object_count(wap_plane *plane, uint32_t count);

WAP_API wap_object *wap_plane_get_object(wap_plane *plane, uint32_t object_index);

WAP_API wap_plane_properties *wap_plane_get_properties(wap_plane *plane);

WAP_API const char *wap_object_get_name(const wap_object *object);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_object_set_name(wap_object *object, const char *name);

WAP_API const char *wap_object_get_logic(const wap_object *object);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_object_set_logic(wap_object *object, const char *logic);

WAP_API const char *wap_object_get_image_set(const wap_object *object);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_object_set_image_set(wap_object *object, const char *image_set);

WAP_API const char *wap_object_get_animation(const wap_object *object);

/* Possible error: WAP_ENOMEMORY */
WAP_API int wap_object_set_animation(wap_object *object, const char *animation);

WAP_API wap_object_properties *wap_object_get_properties(wap_object *object);

WAP_END_DECLS

#endif
