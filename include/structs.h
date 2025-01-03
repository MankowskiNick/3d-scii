#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>

typedef enum {
    FILL,
    WIRE
} draw_mode;

typedef struct {
    double x;
    double y;
} vec2;

typedef struct {
    double x;
    double y;
    double z;
} vec3;

typedef struct {
    vec3 verts[3];
    float color[4];
} triangle;

typedef struct {
    triangle* tris;
    int num_tris;
    draw_mode mode;
} mesh;

static inline void free_mesh(mesh* m) {
    if (m->tris != NULL) {
        free(m->tris);
        m->tris = NULL;
    }
    m->num_tris = 0;
}


#endif
