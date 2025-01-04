#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "render.h"
#include "linalg.h"
#include "structs.h"
#include "mesh_loader.h"

// #define FILENAME "models/teapot_export.obj"
// #define SCALING_FACTOR 0.04f

// #define FILENAME "models/cube.obj"
// #define SCALING_FACTOR 0.5f

#define FILENAME "models/Mask01.obj"
#define SCALING_FACTOR 0.0004f

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
    float angle = 0.0;
    while (running) {
        frame_count++;
        // Clear the screen
        angle += 0.01;
        clear_screen();

        // rotate light
        rot_y(&(l[0].pos), 0.01);

        // rotate and draw teapot
        vec3 rot_axis = { 1.0, 1.0, 1.0 };
        // rot_mesh(&mesh, rot_axis, angle);
        draw_mesh(mesh, &l, 2);
        // rot_mesh(&mesh, rot_axis, -angle);

        // Push the screen to the terminal
        push_to_screen();

        // Sleep for animation effect
        usleep(10000); // 10ms sleep = 100fps
    }

    // Free the mesh
    free_mesh(&mesh);

    printf("\033[20;0H"); // Move cursor below the output
    return 0;
}
