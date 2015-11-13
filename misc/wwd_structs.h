// structs in .wwd file

struct wap32_WWD_Header {
    unsigned wwd_signature;
    unsigned null0;
    unsigned flags;
    unsigned null1;
    char name[64];
    char author[64];
    char birth[64];
    char rez_file[256];
    char image_dir[128];
    char pal_rez[128];
    unsigned start_x;
    unsigned start_y;
    unsigned null2;
    unsigned num_planes;
    unsigned planes_offset;
    unsigned tile_properties_offset;
    unsigned main_block_length;
    unsigned checksum;
    unsigned null3;
    char launch_app[128];
    char image_set1[128];
    char image_set2[128];
    char image_set3[128];
    char image_set4[128];
    char prefix1[32];
    char prefix2[32];
    char prefix3[32];
    char prefix4[32];
};

struct wap32_WWD_Plane {
    unsigned plane_signature;
    unsigned null0;
    unsigned flags;
    unsigned null1;
    char name[64];
    unsigned height_px;
    unsigned width_px;
    unsigned tile_width;
    unsigned tile_height;
    unsigned tiles_wide;
    unsigned tiles_high;
    unsigned null2;
    unsigned null3;
    unsigned movement_x_percent;
    unsigned movement_y_percent;
    unsigned fill_color;
    unsigned num_image_sets;
    unsigned num_objects;
    unsigned tiles_offset;
    unsigned image_sets_offset;
    unsigned objects_offset;
    unsigned z_coord;
    unsigned null4;
    unsigned null5;
    unsigned null6;
};

struct wap32_WWD_Object {
    unsigned id;
    unsigned object_name_len;
    unsigned logic_name_len;
    unsigned image_set_name_len;
    unsigned x;
    unsigned y;
    unsigned z;
    unsigned i;
    unsigned flags;
    unsigned dynamic_flags;
    unsigned draw_flags;
    unsigned user_flags;
    unsigned score;
    unsigned points;
    unsigned powerup;
    unsigned damage;
    unsigned smarts;
    unsigned health;
    wap32_Rect move_rect;
    wap32_Rect hit_rect;
    wap32_Rect attack_rect;
    wap32_Rect clip_rect;
    wap32_Rect user1_rect;
    wap32_Rect user2_rect;
    unsigned user1;
    unsigned user2;
    unsigned user3;
    unsigned user4;
    unsigned user5;
    unsigned user6;
    unsigned user7;
    unsigned user8;
    unsigned x_min;
    unsigned y_min;
    unsigned x_max;
    unsigned y_max;
    unsigned speed_x;
    unsigned speed_y;
    unsigned x_tweak;
    unsigned y_tweak;
    unsigned counter;
    unsigned speed;
    unsigned width;
    unsigned height;
    unsigned direction;
    unsigned face_dir;
    unsigned time_delay;
    unsigned frame_delay;
    unsigned object_type;
    unsigned hit_type_flags;
    unsigned x_move_res;
    unsigned y_move_res;
};
