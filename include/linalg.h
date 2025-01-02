#ifndef LINALG_H
#define LINALG_H

#include "structs.h"

#define min(a, b) ((a) < (b) ? (a) : (b))
#define max(a, b) ((a) > (b) ? (a) : (b))
#define abs(x) ((x) < 0 ? -(x) : (x))
#define sgn(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

vec3 cross_product(vec3 v1, vec3 v2);
void translate(vec3* v, vec3 translation);
void rot_x(vec3* v, double angle);
void rot_y(vec3* v, double angle);
void rot_z(vec3* v, double angle);
void rot(vec3* v, vec3 axis, double angle);

void rot_triangle(triangle* t, vec3 axis, double angle);
void translate_triangle(triangle* t, vec3 translation);

void rot_mesh(mesh* m, vec3 axis, double angle);
void translate_mesh(mesh* m, vec3 translation);

#endif
