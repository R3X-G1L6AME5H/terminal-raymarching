#ifndef VEC3_H_
#define VEC3_H_

#include "stdio.h"
#include "math.h"

typedef struct {
  double x;
  double y;
  double z;
} vec3;


const vec3 X_AXIS = {1,0,0};
const vec3 Y_AXIS = {0,1,0};
const vec3 Z_AXIS = {0,0,1};



static double vec3_magnitude (const vec3* a);
static double vec3_magnitude_squared (const vec3* a);
static vec3   vec3_direction (const vec3* a);

static vec3   vec3_add    (const vec3* a, const vec3* b);
static vec3   vec3_sub    (const vec3* a, const vec3* b);
static vec3   vec3_mult   (const vec3* a, const vec3* b);
static vec3   vec3_mult_s (const vec3* a, double b);
static vec3   vec3_cross  (const vec3* a, const vec3* b);
static double vec3_dot    (const vec3* a, const vec3* b);

static vec3 vec3_print(const vec3* a);




static double vec3_magnitude(const vec3* a){
  	double buff = a->x * a->x;
  	buff += a->y * a->y;
  	buff += a->z * a->z;

	return sqrt(buff);
}

static double vec3_magnitude_squared(const vec3* a){
  	double buff;
    buff  = a->x * a->x;
  	buff += a->y * a->y;
  	buff += a->z * a->z;

	return buff;
}


static vec3 vec3_direction(const vec3* a){
    vec3 unit_vector = {0,0,0};
    double lengthsq = vec3_magnitude_squared(a);
    if (lengthsq != 0) {
        double length = sqrt(lengthsq);
        unit_vector.x = a->x / length;
        unit_vector.y = a->y / length;
        unit_vector.z = a->z / length;
    }
    return unit_vector;
}

static vec3 vec3_abs(const vec3* a){
    vec3 abs_vector = {fabs(a->x), fabs(a->y), fabs(a->z)};
    return abs_vector;
}

static vec3 vec3_max(const vec3* a, const double b){
 	vec3 v = {
      .x = a->x > b ? a->x : b,
      .y = a->y > b ? a->y : b,
      .z = a->z > b ? a->z : b,
      };
    return v;
}
static vec3 vec3_add(const vec3* a, const vec3* b){
 	vec3 v = {
      .x = a->x + b->x,
      .y = a->y + b->y,
      .z = a->z + b->z };
    return v;
}

static vec3 vec3_add_s(const vec3* a, const double b){
 	vec3 v = {
      .x = a->x + b,
      .y = a->y + b,
      .z = a->z + b };
    return v;
}

static vec3 vec3_sub(const vec3* a, const vec3* b){
 	vec3 v = {
      .x = a->x - b->x,
      .y = a->y - b->y,
      .z = a->z - b->z };
    return v;
}

static vec3 vec3_mult(const vec3* a, const vec3* b){
 	vec3 v = {
      .x = a->x * b->x,
      .y = a->y * b->y,
      .z = a->z * b->z };
    return v;
}

static vec3 vec3_mult_s(const vec3* a, double b){
 	vec3 v = {
      .x = a->x * b,
      .y = a->y * b,
      .z = a->z * b };
    return v;
}


static vec3 vec3_cross(const vec3* a, const vec3* b){
	vec3 v = {
			.x = (a->y * b->z) - (a->z * b->y),
			.y = (a->z * b->x) - (a->x * b->z),
			.z = (a->x * b->y) - (a->y * b->x)
        };

	return v;
}

static double vec3_dot(const vec3* a, const vec3* b){
	return a->x * b->x + a->y * b->y + a->z * b->z;
}

static vec3 vec3_print(const vec3* a){
  printf("[%.4f, %.4f, %.4f]", a->x, a->y, a->z);
}

#endif // VEC3_H_
