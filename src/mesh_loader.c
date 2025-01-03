#include "mesh_loader.h"
#include <stdio.h>
#include <string.h>

void parse_vert(char* line, vec3* verts, int index, float scale_factor) {
    // Parse the vertex line
    char* token = strtok(line, " ");
    token = strtok(NULL, " ");
    float x = atof(token);
    token = strtok(NULL, " ");
    float y = atof(token);
    token = strtok(NULL, " ");
    float z = atof(token);

    // Create a vec3 and add it to the list of vertices
    vec3 v = {
        .x = x * scale_factor,
        .y = y * scale_factor,
        .z = z * scale_factor
    };

    verts[index] = v;
}

void parse_face(char* line, face* tris, vec3* verts, int index) {
    char* token = strtok(line, " ");
    token = strtok(NULL, " ");
    int v1 = atoi(token) - 1;
    token = strtok(NULL, " ");
    int v2 = atoi(token) - 1;
    token = strtok(NULL, " ");
    int v3 = atoi(token) - 1;

    // Create a face and add it to the list of faces
    tris[index] = (face){
        .verts = { v1, v2, v3 },
        .color = { 1.0f, 1.0f, 1.0f, 1.0f }
    };
}

int load_mesh_from_obj(const char* filename, mesh* mesh, draw_mode mode, float scale_factor) { 
    // Open the file
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        return 1;
    }

    // Count the number of vertices and faces
    int num_verts = 0;
    int num_tris = 0;
    char line[128];
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == 'v') {
            num_verts++;
        } else if (line[0] == 'f') {
            num_tris++;
        }
    }

    // Allocate memory for vertices and faces
    face* tris =  (face*)malloc(num_tris * sizeof(face));
    vec3* verts = (vec3*)malloc(num_verts * sizeof(vec3));

    // Reset the file pointer
    fseek(file, 0, SEEK_SET);

    // Parse the vertices
    int vert_index = 0;
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == 'v' && line[1] == ' ') {
            parse_vert(line, verts, vert_index, scale_factor);
            vert_index++;
        }
    }

    // Reset the file pointer
    fseek(file, 0, SEEK_SET);

    // Parse the faces
    int tri_index = 0;
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == 'f' && line[1] == ' ') {
            parse_face(line, tris, verts, tri_index);
            tri_index++;
        }
    }

    // Set the mesh properties
    mesh->tris = tris;
    mesh->verts = verts;
    mesh->num_tris = num_tris;
    mesh->num_verts = num_verts;
    mesh->mode = mode;
}
