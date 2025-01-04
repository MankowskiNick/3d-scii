#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "render.h"
#include "linalg.h"
#include "structs.h"
#include "mesh_loader.h"

int main() {
    initialize_screen();

    light l = {
        .pos = { 1.0, 0.0, 0.0 },
        .intensity = 10.0,
        .color = { 1.0, 1.0, 1.0 }
    };

    mesh mesh;

    if (!load_mesh_from_obj("Mask01.obj", &mesh, FILL, 0.0004f, (float[4]){ 0.0f, 0.8f, 0.0f, 1.0f })) {
        printf("Failed to load mesh\n");
        return 1;
    }

    int running = 1;
    int frame_count = 0;
    float angle = 0.0;
    while (running) {
        frame_count++;
        // Clear the screen
        angle += 0.01;
        clear_screen();

        // rotate and draw teapot
        vec3 rot_axis = { 0.0, 1.0, 0.0 };
        rot_mesh(&mesh, rot_axis, angle);
        draw_mesh(mesh, &l, 1);
        rot_mesh(&mesh, rot_axis, -angle);

        // Push the screen to the terminal
        push_to_screen();

        // Print the frame count
        printf("Frame: %d", frame_count++);

        // Sleep for animation effect
        usleep(10000); // 10ms sleep = 100fps
    }

    // Free the mesh
    free_mesh(&mesh);

    printf("\033[20;0H"); // Move cursor below the output
    return 0;
}