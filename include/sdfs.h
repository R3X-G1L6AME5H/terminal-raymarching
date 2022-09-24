#ifndef SDFS_H_
#define SDFS_H_

#include "vec3.h"
#include "mat3.h"

/* RAY DATA MANAGEMENT */

typedef struct{
    vec3 color;
} material;

typedef struct{
    double distance;
    material* material;
} raydata;


void bubble_objects(raydata* a, double b_distance, material* b_id){
    if (a->distance > b_distance){
        a->distance = b_distance;
        a->material = b_id;
    }
}



/* SIGNED DISTANCE FUNCTIONS */
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

double sdBox( vec3* p, vec3* b )
{
  vec3 q = vec3_abs(p);
  q = vec3_sub(&q, b);
  q.x = q.x >= 0 ? q.x : 0.0;
  q.y = q.y >= 0 ? q.y : 0.0;
  q.z = q.z >= 0 ? q.z : 0.0;

  return vec3_magnitude(&q) + fmin(fmax(q.x, fmax(q.y,q.z)), 0.0);
}

double sdSphere(vec3* p, double r){
  return vec3_magnitude(p) - r;
}

double sdPlane(vec3* p){
  return p->y;
}

#endif // SDFS_H_
