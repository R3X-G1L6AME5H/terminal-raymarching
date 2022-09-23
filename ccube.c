#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "signal.h"
#include "time.h"

#define PI 3.1415926535

static double deg2rad( double deg ){return deg * PI / 180.0;}

double iTime;

#include "vec3.h"
#include "mat3.h"
#include "raymarch.h"
#include "sdfs.h"


/*
 *
"\x1b[38;5;" + 232 -> 255 +"m"
void push2buff(double value, char buff[], int* buff_c){
  double norm = fmax(0, fmin(value / 6.0, 1));

}
*/

static double norm_2_256( double norm_num ){
  return round(255 * fmax(0, fmin(norm_num, 1)));
}


char* uv2color(double r, double g, double b, short fgc){
  char* col256 = (char*)malloc(20 * sizeof(char));
  if (fgc > 0){
      sprintf( col256, "%s", "\x1b[38;2;" );
  } else {
      sprintf( col256, "%s", "\x1b[48;2;" );
  }

  sprintf( col256, "%s%03.0f;%03.0f;%03.0fm", col256, norm_2_256(r), norm_2_256(g), norm_2_256(b) );
  return col256;
}


const int SCREEN_BUFFER_SIZE     = 4096;
const char PIXEL[]               = "▄";
const char ANSI_RESET_COLOR[]    = "\033[0m";
const char ANSI_RESET_COLOR_NL[] = "\033[0m\n";
const char ANSI_CLEAR_SCREEN[]   = "\033[H\033[2J";
const char ANSI_RETURN[]         = "\033[0;0H";
const char ANSI_HIDE_CURSOR[]    = "\e[?25l";
const char ANSI_SHOW_CURSOR[]    = "\e[?25h";


void push2buffer(char* buffer, int* buff_c, char* str, int str_size){
  //printf("%d\n", (*buff_c + str_size));
  if (*buff_c + str_size > SCREEN_BUFFER_SIZE){
        // Flush buffer
        printf("%s", buffer);
        // clear buffer
        free(buffer);
        buffer = (char*)malloc(SCREEN_BUFFER_SIZE * sizeof(char));
        *buff_c = 0;
    }

    strcat(buffer, str);
    *buff_c += str_size;
}


vec3 light = {2, 4, 4};
double dist_damp = 45;

double get_ray_value(double uv_x, double uv_y, vec3* origin, mat3* rotation, double (*sdf)(vec3*)){
    vec3 ray = {uv_x, uv_y, 1.0};
    ray = vec3_direction(&ray);   // normalize
    ray = mat3_rotate_vec3(rotation, &ray);
    double d = raymarch(origin, &ray, sdf);

    vec3 pos = vec3_mult_s(&ray, d);
    pos = vec3_add(origin, &pos);
    vec3 normal = calc_normal(&pos, sdf);

    vec3 light_ray = vec3_sub(&light, &pos);
    light_ray = vec3_direction(&light_ray);

    double light_multiplier = vec3_dot(&light_ray, &normal) + 1;
    return fmax(0, fmin(1, d * light_multiplier / dist_damp));
}



void render( int w, int h, double fov,
             vec3* camera_origin,
             mat3* camera_rotation,
             double (*sdf)(vec3*)
            ){

    char* buffer = (char*)malloc(SCREEN_BUFFER_SIZE * sizeof(char));
    int  buff_c = 0;
    char pixel_buff[41];

    double uv_x, uv_y;
    double rd;

    for (double y = h; y >= 0; y -= 2) {
        for (double x = 0; x <= w; x++) {

            uv_x = (x - w / 2.0) / w;

            if (uv_x == 0){continue;}  // at st_x == 0 a vertical white line appears

            // get color for the background
            //printf("%.4f, %.4f - %.4f ; %.4f\n", uv_x, uv_y, rd, sdf(camera_origin));
            uv_y = (y - h / 2.0) / h;
            rd = get_ray_value(uv_x, uv_y, camera_origin, camera_rotation, sdf);
            char* bg_color = uv2color(rd, rd, rd, 0);


            // get color for the character
            uv_y = ((y - 1) - h / 2.0) / h;
            rd = get_ray_value(uv_x, uv_y, camera_origin, camera_rotation, sdf);
            char* fg_color = uv2color(rd, rd, rd, 1);

            // push bit to buffer
            sprintf(pixel_buff, "%s%s%s", bg_color, fg_color, PIXEL);
            push2buffer(buffer, &buff_c, pixel_buff, sizeof(pixel_buff));

            // free memory
            free(bg_color);
            free(fg_color);
        }
        // new line + reset color - prevents color bleed
        push2buffer(buffer, &buff_c, ANSI_RESET_COLOR_NL, sizeof(ANSI_RESET_COLOR_NL));
    }

    printf("%s", buffer);
    free(buffer);
}


void interrupt_handler(int parm);


int main(int argc, char *argv[]) {
    void (*int_handler)(int);
    int_handler = signal (SIGINT, interrupt_handler);

    printf(ANSI_HIDE_CURSOR);
    printf(ANSI_CLEAR_SCREEN);

    /* No lag until 150x75 */
    const int screen_width = 125;
    const int screen_height = 75;
    double camera_fov = deg2rad(90);
    vec3 camera_origin = {0, 4, -2};
    mat3 camera_rotation = mat3_rotation(deg2rad(25), 0, 0);

    double start_stamp = (double)clock();
    int c = 0;
    while (1){
        iTime = (double)clock() - start_stamp;

        box_offset.x = sin(iTime * 0.0000025);
        box_offset.z = 3 + cos(iTime * 0.0000025);
        box_dimensions.x = cos(iTime * 0.000005) * .5 + 1;

        render(screen_width,
               screen_height,
               camera_fov,
               &camera_origin,
               &camera_rotation,
               &world_sdf);


        printf("%sCounter - %d;\nRunning for - %fsec;", ANSI_RESET_COLOR, c, iTime/CLOCKS_PER_SEC);
        c++;
        printf(ANSI_RETURN);
    }

    return 0;
}

void interrupt_handler(int parm){
    printf(ANSI_CLEAR_SCREEN);
    printf(ANSI_SHOW_CURSOR);
    exit(0);
}
