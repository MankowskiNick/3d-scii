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

    mesh teapot;
    load_mesh_from_obj("teapot.obj", &teapot, FILL, 0.2f);

    int running = 1;
    float angle = 0.0;
    while (running) {
        // Clear the screen
        angle += 0.01;
        clear_screen();

        // rotate and draw teapot
        rot_mesh(&teapot, (vec3){ 1.0, 1.0, 1.0 }, angle);
        draw_mesh(teapot);
        rot_mesh(&teapot, (vec3){ 1.0, 1.0, 1.0 }, -angle);

        // Push the screen to the terminal
        push_to_screen();

        // Sleep for animation effect
        usleep(10000); // 10ms sleep = 100fps
    }

    // Free the mesh
    free_mesh(&teapot);

    printf("\033[20;0H"); // Move cursor below the output
    return 0;
}