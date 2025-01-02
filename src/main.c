#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h>
#include "render.h"
#include "linalg.h"
#include "structs.h"

// Helper function to create a vec3
vec3 create_vec3(double x, double y, double z) {
    vec3 v = {x, y, z};
    return v;
}

// Helper function to create a triangle
triangle create_triangle(vec3 v1, vec3 v2, vec3 v3, int r, int g, int b, int a) {
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
    cube.mode = mode; // Default to fill mode

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
    cube.tris[0] = create_triangle(v0, v1, v2, 255, 0, 0, 255); // Back face
    cube.tris[1] = create_triangle(v0, v2, v3, 255, 0, 0, 255);

    cube.tris[2] = create_triangle(v4, v5, v6, 0, 255, 0, 255); // Front face
    cube.tris[3] = create_triangle(v4, v6, v7, 0, 255, 0, 255);

    cube.tris[4] = create_triangle(v3, v2, v6, 0, 0, 255, 255); // Top face
    cube.tris[5] = create_triangle(v3, v6, v7, 0, 0, 255, 255);

    cube.tris[6] = create_triangle(v0, v1, v5, 255, 255, 0, 255); // Bottom face
    cube.tris[7] = create_triangle(v0, v5, v4, 255, 255, 0, 255);

    cube.tris[8] = create_triangle(v0, v3, v7, 0, 255, 255, 255); // Left face
    cube.tris[9] = create_triangle(v0, v7, v4, 0, 255, 255, 255);

    cube.tris[10] = create_triangle(v1, v2, v6, 255, 0, 255, 255); // Right face
    cube.tris[11] = create_triangle(v1, v6, v5, 255, 0, 255, 255);

    return cube;
}

int main() {
    initialize_screen();

    // Draw rotating triangle
    int running = 1;
    double angle = 0.0;
    while (running) {
        // Clear the screen
        angle += 0.01;
        clear_screen();

        // Draw the rotated mesh
        mesh m_rot = create_cube_mesh(FILL);
        rot_mesh(&m_rot, create_vec3(1.0, 1.0, 1.0), angle);
        draw_mesh(m_rot);
        translate_mesh(&m_rot, create_vec3(0.0, 0.0, 5.0));
        free_mesh(&m_rot);

        // Push the screen to the terminal
        push_to_screen();

        // Sleep for animation effect
        usleep(10000);
    }

    printf("\033[20;0H"); // Move cursor below the output
    return 0;
}