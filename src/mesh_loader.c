#include "mesh_loader.h"
#include "setting.h"
#include <stdio.h>
#include <string.h>

void reset_head(FILE* file) {
    fseek(file, 0, SEEK_SET);
}

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

void parse_face(char* line, face* tris, vec3* verts, vec3* norms, int index, float color[4]) {
    uint v[MAX_FACE_VERTICES];
    uint n[MAX_FACE_VERTICES];
    int vertex_count = 0;
    char* token = strtok(line, " ");
    
    token = strtok(NULL, " "); // skip the 'f' token
    while (token != NULL && vertex_count < MAX_FACE_VERTICES) {
        // Split the token by '/' to get vertex/texture/normal indices
        char* slash1 = strchr(token, '/');
        if (slash1) {
            *slash1 = '\0';
            char* slash2 = strchr(slash1 + 1, '/');
            if (slash2) {
                *slash2 = '\0';
                // Get normal index
                n[vertex_count] = atoi(slash2 + 1) - 1;
            }
        }
        else {
            n[vertex_count] = -1;
        }
        
        // Get vertex index
        v[vertex_count] = atoi(token) - 1;
        
        vertex_count++;
        token = strtok(NULL, " ");
    }

    // triangulate the face
    for (int i = 2; i < vertex_count; i++) {
        // Create a face and add it to the list of faces
        tris[index] = (face){
            .verts = { v[0], v[i - 1], v[i] },
            .norms = { n[0], n[i - 1], n[i] },
            #ifdef DEBUG_COLORS
                .color = { 
                    (float)rand() / RAND_MAX,
                    (float)rand() / RAND_MAX,
                    (float)rand() / RAND_MAX,
                    1.0f 
                }
            #else
                .color = { color[0], color[1], color[2], color[3] }
            #endif
        };
        index++;
    }
}

vec3* __load_vec(FILE* file, int* num_vecs, float scale_factor, char* code) {
    char line[128];
    reset_head(file);
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == code[0] && line[1] == code[1]) {
            (*num_vecs)++;
        }
    }

    int vec_index = 0;
    vec3* vecs = (vec3*)malloc(*num_vecs * sizeof(vec3));
    
    reset_head(file);
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == code[0] && line[1] == code[1]) {
            parse_vert(line, vecs, vec_index, scale_factor);
            vec_index++;
        }
    }

    reset_head(file);
    return vecs;
}

vec3* load_verts(FILE* file, int* num_verts, float scale_factor) {
    return __load_vec(file, num_verts, scale_factor, "v ");
}

vec3* load_normals(FILE* file, int* num_norms) {
    return __load_vec(file,  num_norms, 1.0f, "vn");
}

face* load_faces(FILE* file, vec3* verts, vec3* norms, int* num_tris, float color[4]) {
    char line[128];
    reset_head(file);
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == 'f' && line[1] == ' ') {
            (*num_tris)++;
        }
    }

    int tri_index = 0;
    face* tris = (face*)malloc(MAX_FACE_VERTICES * (*num_tris) * sizeof(face));
    
    reset_head(file);
    while (fgets(line, 128, file) != NULL) {
        if (line[0] == 'f' && line[1] == ' ') {
            parse_face(line, tris, verts, norms, tri_index, color);
            tri_index++;
        }
    }

    reset_head(file);

    num_tris = &tri_index;

    return tris;
}

int load_mesh_from_obj(const char* filename, mesh* mesh, draw_mode mode, float scale_factor, float color[4]) { 
    // Open the file
    printf("Loading mesh from file: %s\n", filename);
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        fprintf(stderr, "Failed to open file\n");
        return 0;
    }
    printf("File opened successfully...\n");


    int num_verts = 0;
    int num_tris = 0;
    int num_normals = 0;
    printf("Loading vertices...\n");
    vec3* verts = load_verts(file, &num_verts, scale_factor);
    printf("Loading normals...\n");
    vec3* normals = load_normals(file, &num_normals);

    printf("Loading faces...\n");
    face* tris = load_faces(file, verts, normals, &num_tris, color);

    printf("Exporting mesh...\n");
    // Set the mesh properties
    mesh->tris = tris;
    mesh->verts = verts;
    mesh->num_tris = num_tris;
    mesh->num_verts = num_verts;
    mesh->normals = normals;
    mesh->num_normals = num_normals;
    mesh->mode = mode;

    printf("Mesh loaded successfully\n");
    printf("Number of vertices: %d\n", num_verts);
    printf("Number of normals: %d\n", num_normals);
    printf("Number of faces: %d\n", num_tris);

    printf("Closing file...\n");
    fclose(file);

    printf("Mesh %s loaded successfully\n", filename);

    
    return 1;
}
