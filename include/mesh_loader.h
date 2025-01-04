#ifndef MESH_LOADER_H
#define MESH_LOADER_H

#include "structs.h"

int load_mesh_from_obj(const char* filename, mesh* out_mesh, draw_mode mode, float scale_factor, float color[4]);

#endif
