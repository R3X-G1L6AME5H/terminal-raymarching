#ifndef MAT3_H_
#define MAT3_H_

#include "math.h"
#include "vec3.h"

typedef struct {
  vec3 vx;
  vec3 vy;
  vec3 vz;
} mat3;

const mat3 UNIT_MAT = {{1,0,0}, {0,1,0}, {0,0,1}};

static mat3 mat3_transpose(const mat3* a);
static mat3 mat3_rotation(const double a, const double b, const double c );
static vec3 mat3_rotate_vec3(const mat3* m, const vec3* v);





/* MATRICIES */
static mat3 mat3_transpose(const mat3* a){
  mat3 t = {
    {(a->vx).x, (a->vy).x, (a->vz).x},
    {(a->vx).y, (a->vy).y, (a->vz).y},
    {(a->vx).z, (a->vy).z, (a->vz).z}
  };
  return t;
}

static mat3 mat3_rotation(const double a, const double b, const double c ){
  mat3 ret = {
    { ( cos(c) * cos(b) ),    ( -sin(c) * cos(a) + cos(c) * sin(b) * sin(a) ),     (  sin(c) * sin(a) + cos(c) * sin(b) * cos(a)  ) },
    { ( sin(c) * cos(b) ),    (  cos(c) * cos(a) + sin(c) * sin(b) * sin(a) ),     ( -cos(c) * sin(a) + sin(c) * sin(b) * cos(a)  ) },
    { ( -sin(b)         ),    (  cos(b) * sin(a)                            ),     (  cos(b) * cos(a)                             ) }
  };
  return ret;
}

static vec3 mat3_rotate_vec3(const mat3* m, const vec3* v){
  vec3 new_v = {
    ((m->vx).x * v->x) + ((m->vx).y * v->y) + ((m->vx).z * v->z),
    ((m->vy).x * v->x) + ((m->vy).y * v->y) + ((m->vy).z * v->z),
    ((m->vz).x * v->x) + ((m->vz).y * v->y) + ((m->vz).z * v->z)
  };
  return new_v;
}


#endif // MAT3_H_
