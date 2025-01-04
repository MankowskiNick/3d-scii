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

float dot_product(vec3 v1, vec3 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

float magnitude(vec3 v) {
    return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

void normalize(vec3* v) {
    float mag = magnitude(*v);
    v->x /= mag;
    v->y /= mag;
    v->z /= mag;
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
    // align v with xz plane
    float ang1 = atan2(axis.y, axis.z);
    rot_x(v, -ang1);
    float ang2 = atan2(axis.x, axis.z);
    rot_y(v, -ang2);
    // rotate v
    rot_z(v, angle);
    // unalign v with xz plane
    rot_y(v, ang2);
    rot_x(v, ang1);

}

void rot_mesh(mesh* m, vec3 axis, double angle) {
    for (uint i = 0; i < m->num_verts; i++) {
        rot(&m->verts[i], axis, angle);
    }
    for (uint i = 0; i < m->num_normals; i++) {
        rot(&m->normals[i], axis, angle);
    }
}

void translate_mesh(mesh* m, vec3 translation) {
    for (uint i = 0; i < m->num_verts; i++) {
        translate(&m->verts[i], translation);
    }
}