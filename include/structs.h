#ifndef STRUCTS_H
#define STRUCTS_H

#include <stdlib.h>

// uint is not defined in stdlib.h on Apple
#ifdef __APPLE__
    typedef unsigned int uint;
#endif

typedef enum {
    FILL,
    WIRE
} draw_mode;

typedef struct {
    double x;
    double y;
    double z;
} vec3;

typedef struct {
    int verts[3];
    int norms[3];
    float color[4];
} face;

typedef struct {
    uint num_verts;
    uint num_tris;
    uint num_normals;
    vec3* verts;
    vec3* normals;
    face* tris;
    draw_mode mode;
} mesh;

typedef struct {
    enum { POINT, AMBIENT } type;
    vec3 pos;
    float intensity;
    float color[3];
} light;

static inline void free_mesh(mesh* m) {
    if (m->tris != NULL) {
        free(m->tris);
        m->tris = NULL;
    }
    if (m->verts != NULL) {
        free(m->verts);
        m->verts = NULL;
    }
    m->num_tris = 0;
    m->num_verts = 0;
}


#endif
