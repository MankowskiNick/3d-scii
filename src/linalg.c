#include "linalg.h"
#include <stdio.h>
#include <math.h>

vec3 cross_product(vec3 v1, vec3 v2) {
    vec3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}

void translate(vec3* v, vec3 translation) {
    v->x += translation.x;
    v->y += translation.y;
    v->z += translation.z;
}

void rot_x(vec3* v, double angle) {
    double y = v->y;
    double z = v->z;
    v->y = y * cos(angle) - z * sin(angle);
    v->z = y * sin(angle) + z * cos(angle);
}
void rot_y(vec3* v, double angle) {
    double x = v->x;
    double z = v->z;
    v->x = x * cos(angle) + z * sin(angle);
    v->z = -x * sin(angle) + z * cos(angle);
}
void rot_z(vec3* v, double angle) {
    double x = v->x;
    double y = v->y;
    v->x = x * cos(angle) - y * sin(angle);
    v->y = x * sin(angle) + y * cos(angle);
}

void rot(vec3* v, vec3 axis, double angle) {
    // rotate so that the axis is the x-axis
    double angle_x = atan2(axis.y, axis.z);
    rot_z(v, angle_x);
    double angle_y = atan2(axis.x, axis.z);
    rot_y(v, angle_y);

    // rotate around the x-axis
    rot_z(v, angle);

    // rotate back to the original position
    rot_y(v, -angle_y);
    rot_x(v, -angle_x);
}

void rot_triangle(triangle* t, vec3 axis, double angle) {    
    for (int i = 0; i < 3; i++) {
        rot(&t->verts[i], axis, angle);
    }
}

void translate_triangle(triangle* t, vec3 translation) {
    for (int i = 0; i < 3; i++) {
        translate(&t->verts[i], translation);
    }
}

void rot_mesh(mesh* m, vec3 axis, double angle) {
    for (int i = 0; i < m->num_tris; i++) {
        rot_triangle(&m->tris[i], axis, angle);
    }
}

void translate_mesh(mesh* m, vec3 translation) {
    for (int i = 0; i < m->num_tris; i++) {
        translate_triangle(&m->tris[i], translation);
    }
}