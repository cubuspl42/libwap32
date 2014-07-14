#ifndef wwd_h
#define wwd_h

#include "wap32/wwd.h"
#include "common.h"
#include <array>
#include <string>
#include <vector>

struct Wap32WwdObject {
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
    std::string name;
    std::string logic;
    std::string image_set;
    std::string animation;

};

struct Wap32WwdPlane {
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
    std::vector<unsigned> tiles;
	std::vector<std::string> image_sets;
    std::vector<Wap32WwdObject> objects;
};

struct Wap32WwdTileDescription {
    unsigned type;
    unsigned width;
    unsigned height;
    unsigned inside_attrib;
    unsigned outside_attrib;
    Wap32Rect rect;
};

struct Wap32Wwd {
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
	std::vector<Wap32WwdPlane> planes;
    std::vector<Wap32WwdTileDescription> tile_descriptions;
    Wap32Wwd();
};

int wap32_wwd__read(Wap32Wwd *wwd, const std::vector<char> &input_buffer);

#endif