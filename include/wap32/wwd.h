#ifndef wap_wwd_h
#define wap_wwd_h

#include "common.h"

WAP_BEGIN_DECLS

typedef enum {
    WAP_WWD_FLAG_USE_Z_COORDS = 1 << 0,
    WAP_WWD_FLAG_COMPRESS     = 1 << 1,
} wap_wwdFlags;

typedef enum {
    WAP_OBJECT_TYPE_GENERIC   = 1 << 0,
    WAP_OBJECT_TYPE_PLAYER    = 1 << 1,
    WAP_OBJECT_TYPE_ENEMY     = 1 << 2,
    WAP_OBJECT_TYPE_POWERUP   = 1 << 3,
    WAP_OBJECT_TYPE_SHOT      = 1 << 4,
    WAP_OBJECT_TYPE_PSHOT     = 1 << 5,
    WAP_OBJECT_TYPE_ESHOT     = 1 << 6,
    WAP_OBJECT_TYPE_SPECIAL   = 1 << 7,
    WAP_OBJECT_TYPE_USER1     = 1 << 8,
    WAP_OBJECT_TYPE_USER2     = 1 << 9,
    WAP_OBJECT_TYPE_USER3     = 1 << 10,
    WAP_OBJECT_TYPE_USER4     = 1 << 11,
} wap_objectType;

typedef enum {
    WAP_OBJECT_USER_FLAG_1    = 1 << 0,
    WAP_OBJECT_USER_FLAG_2    = 1 << 1,
    WAP_OBJECT_USER_FLAG_3    = 1 << 2,
    WAP_OBJECT_USER_FLAG_4    = 1 << 3,
    WAP_OBJECT_USER_FLAG_5    = 1 << 4,
    WAP_OBJECT_USER_FLAG_6    = 1 << 5,
    WAP_OBJECT_USER_FLAG_7    = 1 << 6,
    WAP_OBJECT_USER_FLAG_8    = 1 << 7,
    WAP_OBJECT_USER_FLAG_9    = 1 << 8,
    WAP_OBJECT_USER_FLAG_10   = 1 << 9,
    WAP_OBJECT_USER_FLAG_11   = 1 << 10,
    WAP_OBJECT_USER_FLAG_12   = 1 << 11,
} wap_objectUserFlags;

typedef enum {
    WAP_OBJECT_DRAW_FLAG_NODRAW   = 1 << 0,
    WAP_OBJECT_DRAW_FLAG_MIRROR   = 1 << 1,
    WAP_OBJECT_DRAW_FLAG_INVERT   = 1 << 2,
    WAP_OBJECT_DRAW_FLAG_FLASH    = 1 << 3,
} wap_objectDrawFlags;

typedef enum {
    WAP_OBJECT_DYNAMIC_FLAG_NO_HIT                 = 1 << 0,
    WAP_OBJECT_DYNAMIC_FLAG_ALWAYS_ACTIVE          = 1 << 1,
    WAP_OBJECT_DYNAMIC_FLAG_SAFE              = 1 << 2,
    WAP_OBJECT_DYNAMIC_FLAG_AUTO_HIT_DAMAGE   = 1 << 3,
} wap_objectDynamicFlags;

typedef enum OBJ_ADD_FLAGS {
    WAP_OBJECT_ADD_FLAG_DIFFICULT     = 1 << 0,
    WAP_OBJECT_ADD_FLAG_EYE_CANDY     = 1 << 1,
    WAP_OBJECT_ADD_FLAG_HIGH_DETAIL   = 1 << 2,
    WAP_OBJECT_ADD_FLAG_MULTIPLAYER   = 1 << 3,
    WAP_OBJECT_ADD_FLAG_EXTRA_MEMORY  = 1 << 4,
    WAP_OBJECT_ADD_FLAG_FAST_CPU      = 1 << 5,
} wap_objectAddFlags;

typedef enum {
    WAP_PLANE_FLAG_MAIN_PLANE     = 1 << 0,
    WAP_PLANE_FLAG_NO_DRAW        = 1 << 1,
    WAP_PLANE_FLAG_X_WRAPPING     = 1 << 2,
    WAP_PLANE_FLAG_Y_WRAPPING     = 1 << 3,
    WAP_PLANE_FLAG_AUTO_TILE_SIZE = 1 << 4,
} wap_planeFlags;

typedef enum {
    WAP_TILE_ATTRIBUTE_CLEAR  = 0,
    WAP_TILE_ATTRIBUTE_SOLID  = 1 << 0,
    WAP_TILE_ATTRIBUTE_GROUND = 1 << 1,
    WAP_TILE_ATTRIBUTE_CLIMB  = 1 << 2,
    WAP_TILE_ATTRIBUTE_DEATH  = 1 << 3,
} Wap32TileAttribute;

typedef enum TILE_ATRIBTYPE {
    WAP_WWD_TILE_TYPE_SINGLE = 1 << 0,
    WAP_WWD_TILE_TYPE_DOUBLE = 1 << 1,
} Wap32TileType;

typedef struct wap_wwd wap_wwd;
typedef struct wap_plane wap_plane;
typedef struct wap_object wap_object;

typedef struct {
	unsigned left;
	unsigned top;
	unsigned right;
	unsigned bottom;
} wap_rect;

typedef struct {
    unsigned flags;
    char level_name[64];
    char author[64];
    char birth[64];
    char rez_file[256];
    char image_dir[128];
    char pal_rez[128];
    int start_x;
    int start_y;
    char launch_app[128];
    char image_sets[4][128];
    char prefixes[4][32];
} wap_wwd_properties;

typedef struct {
    unsigned flags;
    char name[64];
    unsigned tile_width;
    unsigned tile_height;
    unsigned tiles_wide;
    unsigned tiles_high;
    int movement_x_percent;
    int movement_y_percent;
    unsigned fill_color;
    int z_coord;
} wap_plane_properties;

typedef struct {
    int id;
    int x;
    int y;
    int z;
    int i;
    unsigned add_flags;
    unsigned dynamic_flags;
    unsigned draw_flags;
    unsigned user_flags;
    int score;
    int points;
    int powerup;
    int damage;
    int smarts;
    int health;
    wap_rect move_rect;
    wap_rect hit_rect;
    wap_rect attack_rect;
    wap_rect clip_rect;
    wap_rect user_rects[2];
    int user_values[8];
    int x_min;
    int y_min;
    int x_max;
    int y_max;
    int speed_x;
    int speed_y;
    int x_tweak;
    int y_tweak;
    int counter;
    int speed;
    int width;
    int height;
    int direction;
    int face_dir;
    int time_delay;
    int frame_delay;
    unsigned object_type;
    unsigned hit_type_flags;
    int x_move_res;
    int y_move_res;
} wap_object_properties;

typedef struct {
    unsigned type;
    unsigned width;
    unsigned height;
    unsigned inside_attrib;
    unsigned outside_attrib;
    wap_rect rect;
} wap_tile_description;

WAP_API wap_wwd *wap_wwd_create();

WAP_API void wap_wwd_free(wap_wwd *wwd);

WAP_API int wap_wwd_open(wap_wwd **out, const char *file_path);

WAP_API size_t wap_wwd_get_plane_count(const wap_wwd *wwd);

WAP_API void wap_wwd_set_plane_count(wap_wwd *wwd, size_t count);

WAP_API wap_plane *wap_wwd_get_plane(wap_wwd *wwd, size_t plane_index);

WAP_API size_t wap_wwd_get_tile_description_count(wap_wwd *wwd);

WAP_API wap_tile_description *wap_wwd_get_tile_description(wap_wwd *wwd, size_t description_index);

WAP_API wap_wwd_properties *wap_wwd_get_properties(wap_wwd *wwd);

WAP_API size_t wap_plane_get_tile_count(const wap_plane *plane);

WAP_API void wap_plane_set_tile_count(wap_plane *plane, size_t count);

WAP_API unsigned wap_plane_get_tile(const wap_plane *plane, size_t tile_index);

WAP_API size_t wap_plane_get_image_set_count(const wap_plane *plane);

WAP_API void wap_plane_set_image_set_count(wap_plane *plane, size_t count);

WAP_API const char *wap_plane_get_image_set(const wap_plane *plane, size_t image_set_index);

WAP_API void wap_plane_set_image_set(wap_plane *plane, size_t image_set_index, const char *image_set);

WAP_API size_t wap_plane_get_object_count(const wap_plane *plane);

WAP_API void wap_plane_set_object_count(wap_plane *plane, size_t count);

WAP_API wap_object *wap_plane_get_object(wap_plane *plane, size_t object_index);

WAP_API wap_plane_properties *wap_plane_get_properties(wap_plane *plane);

WAP_API const char *wap_object_get_name(const wap_object *object);

WAP_API void wap_object_set_name(wap_object *object, const char *name);

WAP_API const char *wap_object_get_logic(const wap_object *object);

WAP_API void wap_object_set_logic(wap_object *object, const char *logic);

WAP_API const char *wap_object_get_image_set(const wap_object *object);

WAP_API void wap_object_set_image_set(wap_object *object, const char *image_set);

WAP_API const char *wap_object_get_animation(const wap_object *object);

WAP_API void wap_object_set_animation(wap_object *object, const char *animation);

wap_object_properties *wap_object_get_properties(wap_object *object);

WAP_END_DECLS

#endif
