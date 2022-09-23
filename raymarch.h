#ifndef RAYMARCH_H_
#define RAYMARCH_H_

#include "vec3.h"

#define RM_SURFACE_DISTANCE 0.1
#define RM_MAX_DISTANCE 1000
#define RM_MAX_STEPS 100

int raymarch( const vec3* o, const vec3* r, double (*sdf)(vec3*));


vec3 calc_normal( vec3* pos, double (*sdf)(vec3*)) {
    vec3 n = {0, 0, 0};
    for( int i=0; i<4; i++ ) {
        // vec3 e = 0.5773* (2.0* vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1)) -1.0);
        //
        // vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1))
        vec3 e = { (((i+3)>>1)&1),
                   ((i>>1)&1),
                   (i&1) };
        // 2.0 * vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1))
        e = vec3_mult_s(&e, 2.0);
        // 2.0 * vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1)) - 1.0
        e = vec3_add_s(&e, -1.0);
        // vec3 e = 0.5773* (2.0* vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1)) -1.0);
        e = vec3_mult_s(&e, 0.5773);


        // n += e * map(pos + 0.0005 * e).x;
        //
        // 0.0005 * e
        vec3 n_tmp = vec3_mult_s(&e, 0.0005);
        //  pos + (0.0005 * e)
        n_tmp = vec3_add(pos, &n_tmp);
        //  e * map(pos + (0.0005 * e)).x
        n_tmp = vec3_mult_s(&e, sdf(&n_tmp));
        // n += e * map(pos + 0.0005 * e).x;
        n = vec3_add(&n, &n_tmp);

        // if( n.x+n.y+n.z>100.0 ) break;
    }

    return vec3_direction(&n);
}

int raymarch( const vec3* o, const vec3* r, double (*sdf)(vec3*)){
    vec3 p = *o;
    vec3 ray;


    double total_d = 0;
    double d = sdf(&p);
    int i = 0;
    while (total_d < RM_MAX_DISTANCE && i < RM_MAX_STEPS && RM_SURFACE_DISTANCE < d) {
        total_d += d;
        ray = vec3_mult_s(r, total_d);
        p = vec3_add( &ray, o );

        d = sdf(&p);
        i++;
    }

    return total_d;
}

#endif // RAYMARCH_H_
