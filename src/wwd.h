#ifndef wwd_h
#define wwd_h

#include "wap32/wwd.h"
#include "common.h"
#include <string>
#include <vector>

struct Wap32WwdPlane {
    struct {
        unsigned flags;
        char name[64];
        size_t tile_width;
        size_t tile_height;
        size_t tiles_wide;
        size_t tiles_high;
        int movement_x_percent;
        int movement_y_percent;
        unsigned fill_color;
        int z_coord;
    } data;
	std::vector<std::string> image_sets;
};

struct Wap32Wwd {
    struct {
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
    } data;
	std::vector<Wap32WwdPlane> planes;
    Wap32Wwd();
};

struct Wap32WwdObject {
    struct {
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
        wap32_Rect move_rect;
        wap32_Rect hit_rect;
        wap32_Rect attack_rect;
        wap32_Rect clip_rect;
        wap32_Rect user_rects[2];
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
    } data;
	std::string name;
	std::string logic;
	std::string image_set;
};

#endif