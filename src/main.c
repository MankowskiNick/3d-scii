#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "render.h"
#include "linalg.h"
#include "structs.h"
#include "mesh_loader.h"

// #define FILENAME "models/teapot_export.obj"
// #define SCALING_FACTOR 0.04f

// #define FILENAME "models/cube.obj"
// #define SCALING_FACTOR 0.5f

// #define FILENAME "models/sheep.obj"
// #define SCALING_FACTOR 0.5f

#define FILENAME "models/Mask01.obj"
#define SCALING_FACTOR 0.0004f

// #define FILENAME "models/backpack.obj" // need to translate down 1.5
// #define SCALING_FACTOR 0.01f

int main() {
    initialize_screen();

    light diffuse_light = {
        .pos = { 1.0, -1.0, 0.0 },
        .intensity = 0.5,
        .color = { 1.0, 1.0, 1.0 },
        .type = POINT
    };
    light ambient_light = {
        .intensity = 0.1,
        .color = { 1.0, 1.0, 1.0 },
        .type = AMBIENT
    };

    light l[] = { diffuse_light, ambient_light };
    int num_lights = 2;

    mesh mesh;
    if (!load_mesh_from_obj(FILENAME, &mesh, FILL, SCALING_FACTOR, (float[4]){ 1.0f, 1.0f, 1.0f, 1.0f })) {
        printf("Failed to load mesh\n");
        return 1;
    }
    rot_mesh(&mesh, (vec3){ 0.0, 1.0, 0.0 }, M_PI_2);

    int running = 1;
    int frame_count = 0;
    float framerate = 0.0;
    while (running) {
        clock_t start = clock();

        // Rotate light
        rot_y(&(l[0].pos), 0.01);

        // Draw the mesh
        clear_screen();
        draw_mesh(mesh, &l, 2);
        push_to_screen();

        // Calculate current framerate
        clock_t end = clock();
        float frametime = ((float)(end - start) / CLOCKS_PER_SEC) * 1000.0;
        framerate = 1000.0 / frametime;
        frame_count++;

        // Print debug info
        printf("Frame time: %.2f ms \n", frametime);
        printf("Framerate: %.2f\n", framerate);
        printf("Frame count: %d\n", frame_count);
    }

    // Free the mesh
    free_mesh(&mesh);

    printf("\033[20;0H"); // Move cursor below the output
    printf("\033[?25h"); // Show cursor
    return 0;
}
