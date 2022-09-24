#ifndef CONFIG_H_
#define CONFIG_H_

#include "vec3.h"
#include "mat3.h"

/* CONFIGURATION */

/* SCREEN/CAMERA OPTIONS */
/* No lag until 150x75 */

const int g_screen_width     = 100;
const int g_screen_height    = 50;
vec3 g_camera_origin         = {0, 3, -4};
vec3 g_camera_rotation_euler = {15, 0, 0};
const vec3 g_black            = {0,0,0};


/* RAYMARCHER OPTIONS */
const double RAYMARCH_SURFACE_DISTANCE = 0.1;
const double RAYMARCH_MAX_DISTANCE     = 1000;
const int    RAYMARCH_MAX_STEPS        = 100;

/* RENDERER OPTIONS */
const int  SCREEN_BUFFER_SIZE    = 4096;
const char PIXEL[]               = "▄";
const char ANSI_RESET_COLOR[]    = "\033[0m";
const char ANSI_RESET_COLOR_NL[] = "\033[0m\n";
const char ANSI_CLEAR_SCREEN[]   = "\033[H\033[2J";
const char ANSI_RETURN[]         = "\033[0;0H";
const char ANSI_HIDE_CURSOR[]    = "\e[?25l";
const char ANSI_SHOW_CURSOR[]    = "\e[?25h";

      vec3   g_light_pos = {2, 2, 6};
const double g_light_strength = 2;



/* GLOBAL DATA */
#define PI 3.1415926535
#define deg2rad( deg )  (deg * PI / 180.0)
double iTime = 0;




/* SDF FUNCTION */
#include "sdfs.h"

/* SDF MATERIALS */
material g_horizon_material = { {0,0,0}};// {.9,1.1,1.3} };

material floor_material  = { {0.21, 0.05, 0} }; // { {0.78, 0.78, 1} };
vec3     box_dimensions  = {1, 3, 1};
vec3     box_offset      = {2, 2.5, 2};
material box_material    = { {1,  0,  0} };
vec3     base_dimensions = {1.1, 0.5, 1.1};
material base_material   = { {.75,.45,.45} };
vec3     ball_offset     = { 0, 1, 3 };
vec3     roof_dimensions = {100, .1, 100};
material roof_material   = { {.23,0,0} };


raydata global_sdf_function(vec3* p){
    raydata ret = {99999999, NULL};  /// DONT TOUCH

    g_light_pos.z = sin(iTime * 0.0000005) * 10;

    // floor
    double plane_d = sdPlane(p);
    bubble_objects(&ret, plane_d, &floor_material);


    // pillairs
    vec3   buff_p = *p;
    buff_p.z = fmod(buff_p.z + iTime * 0.0000008, 4) ;
    buff_p.x = fabs(buff_p.x);
    buff_p = vec3_sub(&buff_p, &box_offset);
    double box_d = sdBoxFrame(&buff_p, &box_dimensions, 0.25);
    bubble_objects(&ret, box_d, &box_material);

    // pillair decorations
    buff_p = *p;
    buff_p.z = fmod(buff_p.z + iTime * 0.0000008, 4) ;
    buff_p.x = fabs(buff_p.x);
    buff_p = vec3_sub(&buff_p, &box_offset);
    buff_p.y -= 2;
    double base_d = sdBox(&buff_p, &base_dimensions);
    bubble_objects(&ret, base_d, &base_material);

    // Ball

    buff_p = *p;
    double off_x = ball_offset.x;
    ball_offset.x = sin(iTime * 0.000001);
    buff_p = vec3_sub(&buff_p, &ball_offset);
    double ball_d = sdSphere(&buff_p, .35);
    bubble_objects(&ret, ball_d, &base_material);

    ball_offset.x = off_x;
    off_x = ball_offset.z;

    buff_p = *p;
    ball_offset.z = cos(iTime * 0.000001) + 3;
    buff_p = vec3_sub(&buff_p, &ball_offset);
    ball_d = sdSphere(&buff_p, .35);
    bubble_objects(&ret, ball_d, &base_material);
    ball_offset.z = off_x;
    // roof
    buff_p = *p;
    buff_p.y -= 4.6;
    double roof_d = sdBox(&buff_p, &roof_dimensions);
    bubble_objects(&ret, roof_d, &roof_material);



    return ret;  // DONT TOUCH
}
#endif // CONFIG_H_
