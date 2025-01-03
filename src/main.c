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
        .pos = { 0.0, 15.0, 0.0 },
        .intensity = 5.0,
        .color = { 1.0, 1.0, 1.0 }
    };

    mesh mesh;
    // load_mesh_from_obj("teapot_export.obj", &mesh, FILL, 0.05f, (float[4]){ 0.0f, 0.8f, 0.0f, 1.0f });
    // translate_mesh(&teapot, (vec3){ 0.0, -0.5, 0.0 });

    // load_mesh_from_obj("sheep.obj", &mesh, FILL, 0.5f, (float[4]){ 0.0f, 0.8f, 0.0f, 1.0f });

    load_mesh_from_obj("hand.obj", &mesh, FILL, 2.0f, (float[4]){ 0.0f, 0.8f, 0.0f, 1.0f });

    int running = 1;
    float angle = 0.0;
    while (running) {
        // Clear the screen
        angle += 0.01;
        clear_screen();

        // rotate and draw teapot
        rot_mesh(&mesh, (vec3){ 1.0, 1.0, 1.0 }, angle);
        draw_mesh(mesh, &l, 1);
        rot_mesh(&mesh, (vec3){ 1.0, 1.0, 1.0 }, -angle);

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