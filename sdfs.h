#ifndef SDFS_H_
#define SDFS_H_

#include "vec3.h"
#include "mat3.h"

/* SDFs */
double sdBoxFrame( vec3* p, vec3* b, double e )
{
    // p = abs(p  )-b;
    vec3 p2 = vec3_abs(p);
    p2 = vec3_sub(&p2, b);

    // vec3 q = abs(p+e)-e;
    vec3 q = vec3_add_s(&p2, e);
    q = vec3_abs(&q);
    q = vec3_add_s(&q, -e);

    // return min(min(
    //     length(max(vec3(p.x,q.y,q.z),0.0))+min(max(p.x,max(q.y,q.z)),0.0),
    //     length(max(vec3(q.x,p.y,q.z),0.0))+min(max(q.x,max(p.y,q.z)),0.0)),
    //     length(max(vec3(q.x,q.y,p.z),0.0))+min(max(q.x,max(q.y,p.z)),0.0));
    vec3 v1 = { p2.x,  q.y,  q.z };
    vec3 v2 = {  q.x, p2.y,  q.z };
    vec3 v3 = {  q.x,  q.y, p2.z };
    v1 = vec3_max(&v1, 0);
    v2 = vec3_max(&v2, 0);
    v3 = vec3_max(&v3, 0);

    return fmin(
                fmin(
                     vec3_magnitude(&v1) + fmin(fmax(p2.x, fmax( q.y, q.z)), 0.0),
                     vec3_magnitude(&v2) + fmin(fmax( q.x, fmax(p2.y, q.z)), 0.0)
                ),
                vec3_magnitude(&v3) + fmin(fmax( q.x, fmax( q.y, p2.z)), 0.0)
            );
}

double sdSphere(vec3* p, double r){
  return vec3_magnitude(p) - r;
}

double sdPlane(vec3* p){
  return p->y;
}

// vec3 sphere_offset = {0, 1, 10};
vec3 box_offset = {0, 2, 2};
vec3 box_dimensions = {1, 1, 1};

double world_sdf(vec3* p){
    //vec3 sphere_p = vec3_sub(p, &sphere_offset);
    //double sphere = sdSphere(&sphere_p, 1.0);

    double plane = sdPlane(p);

    vec3 box_p = vec3_sub(p, &box_offset);
    double box = sdBoxFrame(&box_p, &box_dimensions, 0.25);

    return fmin(plane, box);
    //return p->y;
}

#endif // SDFS_H_
