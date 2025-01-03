#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "render.h"
#include "linalg.h"
#include "structs.h"
#include "mesh_loader.h"

// Helper function to create a vec3
vec3 create_vec3(double x, double y, double z) {
    vec3 v = {x, y, z};
    return v;
}

// Helper function to create a triangle
triangle create_triangle(vec3 v1, vec3 v2, vec3 v3, float r, float g, float b, float a) {
    triangle t;
    t.verts[0] = v1;
    t.verts[1] = v2;
    t.verts[2] = v3;
    t.color[0] = r;
    t.color[1] = g;
    t.color[2] = b;
    t.color[3] = a;
    return t;
}

// Function to create a cube mesh
mesh create_cube_mesh(draw_mode mode) {
    mesh cube;

    // Allocate memory for 12 triangles
    cube.num_tris = 12;
    cube.tris = (triangle*)malloc(cube.num_tris * sizeof(triangle));
    cube.mode = mode;

    // Define the vertices of the cube
    vec3 v0 = create_vec3(-0.5, -0.5, -0.5);
    vec3 v1 = create_vec3(0.5, -0.5, -0.5);
    vec3 v2 = create_vec3(0.5, 0.5, -0.5);
    vec3 v3 = create_vec3(-0.5, 0.5, -0.5);
    vec3 v4 = create_vec3(-0.5, -0.5, 0.5);
    vec3 v5 = create_vec3(0.5, -0.5, 0.5);
    vec3 v6 = create_vec3(0.5, 0.5, 0.5);
    vec3 v7 = create_vec3(-0.5, 0.5, 0.5);

    // Define the 12 triangles (2 per face)
    cube.tris[0] = create_triangle(v0, v1, v2, 1.0f, 0.0f, 0.0f, 1.0f); // Back face
    cube.tris[1] = create_triangle(v0, v2, v3, 1.0f, 0.0f, 0.0f, 1.0f);

    cube.tris[2] = create_triangle(v4, v5, v6, 0.0f, 1.0f, 0.0f, 1.0f); // Front face
    cube.tris[3] = create_triangle(v4, v6, v7, 0.0f, 1.0f, 0.0f, 1.0f);

    cube.tris[4] = create_triangle(v3, v2, v6, 0.0f, 0.0f, 1.0f, 1.0f); // Top face
    cube.tris[5] = create_triangle(v3, v6, v7, 0.0f, 0.0f, 1.0f, 1.0f);

    cube.tris[6] = create_triangle(v0, v1, v5, 1.0f, 1.0f, 0.0f, 1.0f); // Bottom face
    cube.tris[7] = create_triangle(v0, v5, v4, 1.0f, 1.0f, 0.0f, 1.0f);

    cube.tris[8] = create_triangle(v0, v3, v7, 0.0f, 1.0f, 1.0f, 1.0f); // Left face
    cube.tris[9] = create_triangle(v0, v7, v4, 0.0f, 1.0f, 1.0f, 1.0f);

    cube.tris[10] = create_triangle(v1, v2, v6, 1.0f, 0.0f, 1.0f, 1.0f); // Right face
    cube.tris[11] = create_triangle(v1, v6, v5, 1.0f, 0.0f, 1.0f, 1.0f);

    return cube;
}

int main() {
    initialize_screen();

    mesh teapot;
    load_mesh_from_obj("teapot.obj", &teapot, FILL);

    // Draw rotating triangle
    int running = 1;
    double angle = 0.0;
    while (running) {
        // Clear the screen
        angle += 0.01;
        clear_screen();

        draw_mesh(teapot);

        // // Draw the rotated mesh
        // mesh m_rot = create_cube_mesh(FILL);
        // rot_mesh(&m_rot, create_vec3(1.0, 1.0, 1.0), angle);
        // draw_mesh(m_rot);
        // translate_mesh(&m_rot, create_vec3(0.0, 0.0, 5.0));
        // free_mesh(&m_rot); 

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