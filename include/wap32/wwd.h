#ifndef wap32_wwd_h
#define wap32_wwd_h

#include "common.h"

WAP32_BEGIN_DECLS

typedef enum {
    WAP32_WWD_FLAG_USE_Z_COORDS = 0x1,
    WAP32_WWD_FLAG_COMPRESS = 0x2
} Wap32WwdFlags;

typedef enum {
    WAP32_OBJECT_TYPE_GENERIC   = 1 << 0,
    WAP32_OBJECT_TYPE_PLAYER    = 1 << 1,
    WAP32_OBJECT_TYPE_ENEMY     = 1 << 2,
    WAP32_OBJECT_TYPE_POWERUP   = 1 << 3,
    WAP32_OBJECT_TYPE_SHOT      = 1 << 4,
    WAP32_OBJECT_TYPE_PSHOT     = 1 << 5,
    WAP32_OBJECT_TYPE_ESHOT     = 1 << 6,
    WAP32_OBJECT_TYPE_SPECIAL   = 1 << 7,
    WAP32_OBJECT_TYPE_USER1     = 1 << 8,
    WAP32_OBJECT_TYPE_USER2     = 1 << 9,
    WAP32_OBJECT_TYPE_USER3     = 1 << 10,
    WAP32_OBJECT_TYPE_USER4     = 1 << 11,
} Wap32ObjectType;

typedef enum {
    WAP32_OBJECT_USER_FLAG_1    = 1 << 0,
    WAP32_OBJECT_USER_FLAG_2    = 1 << 1,
    WAP32_OBJECT_USER_FLAG_3    = 1 << 2,
    WAP32_OBJECT_USER_FLAG_4    = 1 << 3,
    WAP32_OBJECT_USER_FLAG_5    = 1 << 4,
    WAP32_OBJECT_USER_FLAG_6    = 1 << 5,
    WAP32_OBJECT_USER_FLAG_7    = 1 << 6,
    WAP32_OBJECT_USER_FLAG_8    = 1 << 7,
    WAP32_OBJECT_USER_FLAG_9    = 1 << 8,
    WAP32_OBJECT_USER_FLAG_10   = 1 << 9,
    WAP32_OBJECT_USER_FLAG_11   = 1 << 10,
    WAP32_OBJECT_USER_FLAG_12   = 1 << 11,
} Wap32ObjectUserFlags;

typedef enum {
    WAP32_OBJECT_DRAW_FLAG_NODRAW   = 1 << 0,
    WAP32_OBJECT_DRAW_FLAG_MIRROR   = 1 << 1,
    WAP32_OBJECT_DRAW_FLAG_INVERT   = 1 << 2,
    WAP32_OBJECT_DRAW_FLAG_FLASH    = 1 << 3,
} Wap32ObjectDrawFlags;

typedef enum {
    WAP32_OBJECT_DYNAMIC_NO_HIT                 = 1 << 0,
    WAP32_OBJECT_DYNAMIC_ALWAYS_ACTIVE          = 1 << 1,
    WAP32_OBJECT_DYNAMIC_FLAG_SAFE              = 1 << 2,
    WAP32_OBJECT_DYNAMIC_FLAG_AUTO_HIT_DAMAGE   = 1 << 3,
} Wap32ObjectDynamicFlags;

typedef enum OBJ_ADD_FLAGS {
    WAP32_OBJECT_ADD_FLAG_DIFFICULT     = 1 << 0,
    WAP32_OBJECT_ADD_FLAG_EYE_CANDY     = 1 << 1,
    WAP32_OBJECT_ADD_FLAG_HIGH_DETAIL   = 1 << 2,
    WAP32_OBJECT_ADD_FLAG_MULTIPLAYER   = 1 << 3,
    WAP32_OBJECT_ADD_FLAG_EXTRA_MEMORY  = 1 << 4,
    WAP32_OBJECT_ADD_FLAG_FAST_CPU      = 1 << 5,
} Wap32ObjectAddFlags;

typedef enum {
    WAP32_PLANE_FLAG_MAIN_PLANE     = 1 << 0,
    WAP32_PLANE_FLAG_NO_DRAW        = 1 << 1,
    WAP32_PLANE_FLAG_X_WRAPPING     = 1 << 2,
    WAP32_PLANE_FLAG_Y_WRAPPING     = 1 << 3,
    WAP32_PLANE_FLAG_AUTO_TILE_SIZE = 1 << 4,
} Wap32PlaneFlags;

typedef enum {
    WAP32_TILE_ATTRIBUTE_CLEAR  = 0,
    WAP32_TILE_ATTRIBUTE_SOLID  = 1 << 0,
    WAP32_TILE_ATTRIBUTE_GROUND = 1 << 1,
    WAP32_TILE_ATTRIBUTE_CLIMB  = 1 << 2,
    WAP32_TILE_ATTRIBUTE_DEATH  = 1 << 3,
} Wap32TileAttribute;

typedef enum TILE_ATRIBTYPE {
    WAP32_WWD_TILE_TYPE_SINGLE = 1 << 0,
    WAP32_WWD_TILE_TYPE_DOUBLE = 1 << 1,
} Wap32TileType;

typedef struct Wap32Wwd Wap32Wwd;
typedef struct Wap32Plane Wap32Plane;
typedef struct Wap32Object Wap32Object;

struct Wap32WwdProperties {
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
};

struct Wap32PlaneProperties {
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
};

struct Wap32ObjectProperties {
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
    Wap32Rect move_rect;
    Wap32Rect hit_rect;
    Wap32Rect attack_rect;
    Wap32Rect clip_rect;
    Wap32Rect user_rects[2];
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
};

struct Wap32TileDescription {
    unsigned type;
    unsigned width;
    unsigned height;
    unsigned inside_attrib;
    unsigned outside_attrib;
    Wap32Rect rect;
};

WAP32_API Wap32Wwd *wap32_wwd_create();

WAP32_API void wap32_wwd_free(Wap32Wwd *wwd);

WAP32_API int wap32_wwd_open(Wap32Wwd **out, const char *file_path);

WAP32_API size_t wap32_wwd_get_planes_count(const Wap32Wwd *wwd);

WAP32_API void wap32_wwd_set_planes_count(Wap32Wwd *wwd, size_t count);

WAP32_API Wap32Plane *wap32_wwd_get_plane(Wap32Wwd *wwd, size_t plane_index);

WAP32_API Wap32WwdProperties *wap32_wwd_get_properties(Wap32Wwd *wwd);

WAP32_API size_t wap32_plane_get_image_sets_count(const Wap32Plane *plane);

WAP32_API void wap32_plane_set_image_sets_count(Wap32Plane *plane, size_t count);

WAP32_API const char *wap32_plane_get_image_set(const Wap32Plane *plane, size_t image_set_index);

WAP32_API void wap32_plane_set_image_set(Wap32Plane *plane, size_t image_set_index, const char *image_set);

WAP32_API size_t wap32_plane_get_object_count(const Wap32Plane *plane);

WAP32_API void wap32_plane_set_object_count(Wap32Plane *plane, size_t count);

WAP32_API Wap32Object *wap32_plane_get_object(Wap32Plane *plane, size_t object_index);

WAP32_API Wap32PlaneProperties *wap32_plane_get_properties(Wap32Plane *plane);

WAP32_API const char *WAP32_OBJECT_get_name(const Wap32Object *object);

WAP32_API void WAP32_OBJECT_set_name(Wap32Object *object, const char *name);

WAP32_API const char *WAP32_OBJECT_get_logic(const Wap32Object *object);

WAP32_API void WAP32_OBJECT_set_logic(Wap32Object *object, const char *logic);

WAP32_API const char *WAP32_OBJECT_get_image_set(const Wap32Object *object);

WAP32_API void WAP32_OBJECT_set_image_set(Wap32Object *object, const char *image_set);

WAP32_API const char *WAP32_OBJECT_get_animation(const Wap32Object *object);

WAP32_API void WAP32_OBJECT_set_animation(Wap32Object *object, const char *animation);

Wap32ObjectProperties *WAP32_OBJECT_get_properties(Wap32Object *object);

WAP32_END_DECLS

#endif