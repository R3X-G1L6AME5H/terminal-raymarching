#ifndef RAYMARCH_H_
#define RAYMARCH_H_

#include "config.h"
#include "vec3.h"


double calc_shadow(vec3* pos, vec3* normal, raydata (*sdf)(vec3*)){
    raydata rd;
    vec3 dir = vec3_sub(&g_light_pos, pos);
    vec3 p;

    double max_dist = vec3_magnitude(&dir);
    double dist_traveled = 0;
    double shadow_value = 1;
    vec3 n = vec3_mult_s(normal, 2 * RAYMARCH_SURFACE_DISTANCE);

    do {
        p = vec3_mult_s(&dir, dist_traveled / max_dist);
        p = vec3_add(pos, &p);
        p = vec3_add(&p, &n);

        rd = sdf(&p);
        if( rd.distance - RAYMARCH_SURFACE_DISTANCE < 0 ){
            shadow_value = 0;
            break;
        }

        dist_traveled += rd.distance;

     } while ( dist_traveled < max_dist);

    return shadow_value;
}


/* Adapted from Inigo Quilez; magic as far as I am concerned */
vec3 calc_normal( vec3* pos, raydata (*sdf)(vec3*)) {
    vec3 n = {0, 0, 0};
    for( int i=0; i<4; i++ ) {
        // vec3 e = 0.5773* (2.0* vec3((((i+3)>>1)&1), ((i>>1)&1), (i&1)) -1.0);
        vec3 e = { (((i+3)>>1)&1),
                   ((i>>1)&1),
                   (i&1) };
        e = vec3_mult_s(&e, 2.0);
        e = vec3_add_s(&e, -1.0);
        e = vec3_mult_s(&e, 0.5773);


        // n += e * map(pos + 0.0005 * e).x;
        vec3 n_tmp = vec3_mult_s(&e, 0.0005);
        n_tmp = vec3_add(pos, &n_tmp);
        raydata tmp = sdf(&n_tmp);
        n_tmp = vec3_mult_s(&e, tmp.distance);
        n = vec3_add(&n, &n_tmp);

        // if( n.x+n.y+n.z>100.0 ) break;
    }

    return vec3_direction(&n);
}

raydata raymarch( const vec3* o, const vec3* r, raydata (*sdf)(vec3*)){
    raydata ret = {0, NULL};
    vec3 p = *o;
    vec3 ray;


    raydata d;
    int i = 0;
    do {
        d = sdf(&p);
        ret.distance += d.distance;
        ret.material = d.material;
        ray = vec3_mult_s(r, ret.distance);
        p = vec3_add( &ray, o );
        i++;
        if (ret.distance > RAYMARCH_MAX_DISTANCE ){
            ret.material = &g_horizon_material;
            break;
        }
    } while (RAYMARCH_SURFACE_DISTANCE < d.distance && i < RAYMARCH_MAX_STEPS );

    return ret;
}

#endif // RAYMARCH_H_
