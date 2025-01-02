#include "render.h"
#include "linalg.h"
#include <stdio.h>
#include <math.h>

#define X_STEP 2.0 / WIDTH
#define Y_STEP 2.0 / HEIGHT

#define clamp_rgb(x) (x < 0 ? 0 : (x > 255 ? 255 : x))
#define scale_x(x) (int)(x * WIDTH / 2 + WIDTH / 2)
#define scale_y(y) (int)(-y * HEIGHT / 2 + HEIGHT / 2)

int screen[HEIGHT][WIDTH][3];
int single_buffer[HEIGHT][WIDTH][3];
float z_buffer[HEIGHT][WIDTH];

void initialize_screen() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j][0] = 0;
            screen[i][j][1] = 0;
            screen[i][j][2] = 0;
        }
    }
}

void clear_screen() {
    // reset color
    printf("\033[0m");
    printf("\033[2J\033[H"); // ANSI escape code to clear the screen
    fflush(stdout);
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j][0] = 0;
            screen[i][j][1] = 0;
            screen[i][j][2] = 0;
            z_buffer[i][j] = INFINITY;
        }
    }
}

void fill(int color[4]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[i][j][0] = color[0];
            screen[i][j][1] = color[1];
            screen[i][j][2] = color[2];
        }
    }
}

void draw_pixel(vec3 pos, int color[4]) {
    int row = (int)round(scale_y(pos.y));
    int col = (int)round(scale_x(pos.x));

    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) {
        return;
    }

    if (z_buffer[row][col] < pos.z) {
        return;
    }
    
    int blended_color[3];
    for (int i = 0; i < 3; i++) {
        float alpha = color[3] / 255.0;
        blended_color[i] = (int)(color[i] * alpha + screen[row][col][i] * (1 - alpha));
        z_buffer[row][col] = pos.z;
        screen[row][col][i] = clamp_rgb(blended_color[i]);
    }
}

void draw_triangle_wireframe(triangle t) {
    for (int i = 0; i < 3; i++) {
        draw_line(t.verts[i], t.verts[(i + 1) % 3], t.color);
    }
}

void draw_triangle_filled(triangle t) {
    vec3 v1 = t.verts[0];
    vec3 v2 = t.verts[1];
    vec3 v3 = t.verts[2];

    int color[4] = {t.color[0], t.color[1], t.color[2], t.color[3]};

    float min_x, max_x, min_y, max_y;
    min_x = min(v1.x, min(v2.x, v3.x));
    max_x = max(v1.x, max(v2.x, v3.x));
    min_y = min(v1.y, min(v2.y, v3.y));
    max_y = max(v1.y, max(v2.y, v3.y));

    for (float y = min_y; y <= max_y; y+=Y_STEP) {
        for (float x = min_x; x <= max_x; x+=X_STEP) {
            vec3 v1_to_v2 = {v2.x - v1.x, v2.y - v1.y, 0};
            vec3 v2_to_v3 = {v3.x - v2.x, v3.y - v2.y, 0};
            vec3 v3_to_v1 = {v1.x - v3.x, v1.y - v3.y, 0};

            vec3 v1_to_p = {x - v1.x, y - v1.y, 0};
            vec3 v2_to_p = {x - v2.x, y - v2.y, 0};
            vec3 v3_to_p = {x - v3.x, y - v3.y, 0};

            float dir1 = cross_product(v1_to_v2, v1_to_p).z;
            float dir2 = cross_product(v2_to_v3, v2_to_p).z;
            float dir3 = cross_product(v3_to_v1, v3_to_p).z;


            // calculate z using barycentric coordinates
            float area = cross_product(v1_to_v2, v3_to_v1).z;
            float w1 = cross_product(v2_to_v3, v2_to_p).z / area;
            float w2 = cross_product(v3_to_v1, v3_to_p).z / area;
            float w3 = cross_product(v1_to_v2, v1_to_p).z / area;
            float z = w1 * v1.z + w2 * v2.z + w3 * v3.z;

            if ((sgn(dir1) == sgn(dir2)) && (sgn(dir2) == sgn(dir3))) {
                vec3 pos = {x, y, z};
                draw_pixel(pos, color);
            }
        }
    }
}

void draw_triangle(triangle t, draw_mode mode) {
    switch (mode) {
        case FILL:
            draw_triangle_filled(t);
            break;
        case WIRE:
            draw_triangle_wireframe(t);
            break;
        default:
            fprintf(stderr, "Invalid draw mode\n");
            break;
    }
}

void draw_line(vec3 v1, vec3 v2, int color[4]) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    
    float x, y, z;
    if (abs(dx) > abs(dy)) {
        float m = dy / dx;
        float b = v1.y - m * v1.x;
        if (dx < 0) {
            vec3 temp = v1;
            v1 = v2;
            v2 = temp;
        }
        for (x = v1.x; x <= v2.x; x+=X_STEP) {
            y = m * x + b;
            z = v1.z + dz * (x - v1.x) / dx;
            vec3 pos = {x, y, z};
            draw_pixel(pos, color);
        }
    } else {
        float m = dx / dy;
        float b = v1.x - m * v1.y;
        if (dy < 0) {
            vec3 temp = v1;
            v1 = v2;
            v2 = temp;
        }
        for (y = v1.y; y <= v2.y; y+=Y_STEP) {
            x = m * y + b;
            z = v1.z + dz * (y - v1.y) / dy;
            vec3 pos = {x, y, z};
            draw_pixel(pos, color);
        }
    }
}

void draw_mesh(mesh m) {
    for (int i = 0; i < m.num_tris; i++) {
        draw_triangle(m.tris[i], m.mode);
    }
}

void cycle_buffer() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            single_buffer[i][j][0] = screen[i][j][0];
            single_buffer[i][j][1] = screen[i][j][1];
            single_buffer[i][j][2] = screen[i][j][2];
        }
    }
}

void push_to_screen() {
    printf("\033[?25l"); // Hide cursor
    printf("\033[H"); // Move cursor to home position

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            #ifdef DOUBLE_BUFFER
                printf("\033[48;2;%d;%d;%dm ", single_buffer[i][j][0], single_buffer[i][j][1], single_buffer[i][j][2]);
            #else
                printf("\033[48;2;%d;%d;%dm ", screen[i][j][0], screen[i][j][1], screen[i][j][2]);
            #endif
        }
        printf("\033[0m\n"); // Reset color and move to the next line
    }
    fflush(stdout);

    #ifdef DOUBLE_BUFFER
        cycle_buffer();
    #endif

    printf("\033[?25h"); // Show cursor
}
