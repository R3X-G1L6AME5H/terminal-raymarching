#ifndef RENDERER_H_
#define RENDERER_H_

#include <stdlib.h>
#include <string.h>
#include <stdio.h>


#include "config.h"
#include "vec3.h"
#include "mat3.h"
#include "raymarch.h"


/* Convert  [0-1] range to [0-255] */
static double norm_2_256( double norm_num ){
    return round(255 * fmax(0, fmin(norm_num, 1)));
}

/* Generate the ANSI color code */
char* uv2color(double r, double g, double b, short fgc){
    char* col256 = (char*)malloc(20 * sizeof(char));

    if (fgc > 0){ // If foreground color
        sprintf( col256, "%s", "\x1b[38;2;" );
    } else {
        sprintf( col256, "%s", "\x1b[48;2;" );
    }

    sprintf( col256, "%s%03.0f;%03.0f;%03.0fm", col256, norm_2_256(r), norm_2_256(g), norm_2_256(b) );
    return col256;
}


/* Write to buffer and printf a massive chunk at the same time (performance boost) */
void push2buffer(char* buffer, int* buff_c, const char* str, int str_size){
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


/* do the raymarch;  */
char* get_ray_value(double uv_x, double uv_y, vec3* origin, mat3* rotation, short fgc, raydata (*sdf)(vec3*)){
    vec3 ray = {uv_x, uv_y, 1.0};
    ray = vec3_direction(&ray);   // normalize
    ray = mat3_rotate_vec3(rotation, &ray);
    raydata initial_ray = raymarch(origin, &ray, sdf);
    // get the collision vector
    vec3 pos = vec3_mult_s(&ray, initial_ray.distance);
    pos = vec3_add(origin, &pos);

    // get collision normal
    vec3 normal = calc_normal(&pos, sdf);

    // calculate the effect of light on resulting value
    vec3 light_ray = vec3_sub(&g_light_pos, &pos);
    double light_multiplier = fmax(0, (g_light_strength / vec3_magnitude(&light_ray)  ));

    light_ray = vec3_direction(&light_ray);
    light_multiplier *= vec3_dot(&light_ray, &normal) * .5 + 1; // the angle of surface to the light


    double shadow_multiplier = calc_shadow(&pos, &normal, sdf);

    double val = fmax(0, fmin(1, (light_multiplier + shadow_multiplier) * 0.5  ));


    vec3 COLOR = vec3_lerp(&(initial_ray.material->color), &g_black, val);

    // COLOR = initial_ray.material->color;
    return uv2color(COLOR.x, COLOR.y, COLOR.z, fgc);
}



void render( raydata (*sdf)(vec3*)){
    /* Manage buffers */
    char* buffer = (char*)malloc(SCREEN_BUFFER_SIZE * sizeof(char));
    int  buff_c = 0;
    char pixel_buff[41];

    // Build the camera rotation matrix
    mat3 camera_rotation = mat3_rotation(
                                            deg2rad(g_camera_rotation_euler.x),
                                            deg2rad(g_camera_rotation_euler.y),
                                            deg2rad(g_camera_rotation_euler.z)
                                        );

    double uv_x, uv_y;
    double rd; // Ray direction
    for (double y = g_screen_height; y >= 0; y -= 2) {
        for (double x = 0; x <= g_screen_width; x++) {

            // convert from [0...screen_width] to [(-screen_width / 2)...(screen_width / 2)]
            //     -> this is to center the camera
            uv_x = (x - g_screen_width / 2.0) / g_screen_width;
            if (uv_x == 0){continue;}  // at st_x == 0 a vertical white line appears

            // get color for the background
            uv_y = (y - g_screen_height / 2.0) / g_screen_height; // convert number range
            char* bg_color = get_ray_value(uv_x, uv_y, &g_camera_origin, &camera_rotation, 0, sdf);


            // get color for the character
            uv_y = ((y - 1) - g_screen_height / 2.0) / g_screen_height; // convert number range
            char* fg_color = get_ray_value(uv_x, uv_y, &g_camera_origin, &camera_rotation, 1, sdf);


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

    // flush whatever remains in the buffer at the end
    printf("%s", buffer);
    free(buffer);
}

#endif // RENDERER_H_
