#include "render.h"
#include "linalg.h"
#include "setting.h"
#include <stdio.h>
#include <math.h>

#define clamp_rgb(x) (x < 0 ? 0 : (x > 255 ? 255 : x))
#define scale_x(x) (int)(x * WIDTH / 2 + WIDTH / 2)
#define scale_y(y) (int)(-y * HEIGHT / 2 + HEIGHT / 2)
#define scale_rgb(x) (int)(x * 255)

unsigned char screen[BUFFER_SIZE][HEIGHT][WIDTH][3];
int single_buffer[HEIGHT][WIDTH][3];
float z_buffer[HEIGHT][WIDTH];

void initialize_screen() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int k = 0; k < BUFFER_SIZE; k++) {
                screen[k][i][j][0] = 0;
                screen[k][i][j][1] = 0;
                screen[k][i][j][2] = 0;
            }
        }
    }
}

void clear_screen() {
    printf("\033[?25l");     // Hide cursor
    printf("\033[H");        // Move cursor to home position (1,1)
    fflush(stdout);
    
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {

            screen[BUFFER_SIZE - 1][i][j][0] = 0;
            screen[BUFFER_SIZE - 1][i][j][1] = 0;
            screen[BUFFER_SIZE - 1][i][j][2] = 0;
            z_buffer[i][j] = INFINITY;
        }
    }
}

void fill(float color[4]) {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            screen[BUFFER_SIZE - 1][i][j][0] = scale_rgb(color[0]);
            screen[BUFFER_SIZE - 1][i][j][1] = scale_rgb(color[1]);
            screen[BUFFER_SIZE - 1][i][j][2] = scale_rgb(color[2]);
        }
    }
}

// z buffer is not correct.  not sure what is wrong.
void draw_line(vec3 v1, vec3 v2, float color[4]) {
    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;
    float stepsf = max(abs(dx), abs(dy)) * 1000.0f;
    if (stepsf < 1.0f) stepsf = 1.0f;
    int steps = (int)stepsf;
    float sx = dx / steps;
    float sy = dy / steps;
    float sz = dz / steps;
    vec3 pos = v1;
    for (int i = 0; i <= steps; i++) {
        draw_pixel(pos, color);
        pos.x += sx;
        pos.y += sy;
        pos.z += sz;
    }
}

void draw_pixel(vec3 pos, float color[4]) {
    int row = (int)round(scale_y(pos.y));
    int col = (int)round(scale_x(pos.x));

    if (row < 0 || row >= HEIGHT || col < 0 || col >= WIDTH) {
        return;
    }

    if (z_buffer[row][col] < pos.z) {
        return;
    }
    
    float blended_color[3];
    for (int i = 0; i < 3; i++) {
        float alpha = color[3];
        blended_color[i] = (color[i] * alpha + screen[BUFFER_SIZE - 1][row][col][i] * (1 - alpha));
        z_buffer[row][col] = pos.z;
        screen[BUFFER_SIZE - 1][row][col][i] = scale_rgb(blended_color[i]);
    }
}

void draw_face_wireframe(mesh* m, face t) {
    for (int i = 0; i < 3; i++) {
        uint v1 = t.verts[i];
        uint v2 = t.verts[(i + 1) % 3];
        draw_line(m->verts[v1], m->verts[v2], t.color);
    }
}

vec3 get_normal_interp(mesh* m, face t, vec3 p) {
    // return average of normals
    vec3 n1 = m->normals[t.norms[0]];
    vec3 n2 = m->normals[t.norms[1]];
    vec3 n3 = m->normals[t.norms[2]];
    vec3 n = {
        (n1.x + n2.x + n3.x) / 3.0,
        (n1.y + n2.y + n3.y) / 3.0,
        (n1.z + n2.z + n3.z) / 3.0
    };
    normalize(&n);
    return n;
}

void draw_face_filled(mesh* m, face t, light* lights, int num_lights) {
    uint v1_id = t.verts[0];
    uint v2_id = t.verts[1];
    uint v3_id = t.verts[2];

    vec3 v1 = m->verts[v1_id];
    vec3 v2 = m->verts[v2_id];
    vec3 v3 = m->verts[v3_id];

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

            vec3 pos = { x, y, z };

            if ((sgn(dir1) == sgn(dir2)) && (sgn(dir2) == sgn(dir3))) {
                float color[4] = {t.color[0], t.color[1], t.color[2], t.color[3]};
                #ifdef ENABLE_LIGHTING
                    // lighting calculations
                    float total_r = 0, total_g = 0, total_b = 0;

                    for (int i = 0; i < num_lights; i++) {
                        float intensity = lights[i].intensity;

                        if (lights[i].type == POINT) {
                            vec3 norm = get_normal_interp(m, t, pos);
                            vec3 light_dir = {lights[i].pos.x - pos.x, lights[i].pos.y - pos.y, lights[i].pos.z - pos.z};
                            normalize(&light_dir);
                            intensity *= max(0.0f, dot_product(norm, light_dir));
                        }
                        total_r += lights[i].color[0] * intensity;
                        total_g += lights[i].color[1] * intensity;
                        total_b += lights[i].color[2] * intensity;
                    }
                    color[0] *= min(1.0f, total_r); // this is a bit off
                    color[1] *= min(1.0f, total_g);
                    color[2] *= min(1.0f, total_b);
                #endif

                draw_pixel(pos, color);
            }
        }
    }
}

void draw_face(mesh* m, face t, light* lights, int num_lights, draw_mode mode) {
    switch (mode) {
        case FILL:
            draw_face_filled(m, t, lights, num_lights);
            break;
        case WIRE:
            draw_face_wireframe(m, t);
            break;
        default:
            fprintf(stderr, "Invalid draw mode\n");
            break;
    }
}

void draw_mesh(mesh m, light* lights, int num_lights) {
    for (int i = 0; i < m.num_tris; i++) {
        draw_face(&m, m.tris[i], lights, num_lights,  m.mode);
    }
}

void cycle_buffer() {
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            for (int k = 0; k < BUFFER_SIZE - 1; k++) {
                screen[k][i][j][0] = screen[k + 1][i][j][0];
                screen[k][i][j][1] = screen[k + 1][i][j][1];
                screen[k][i][j][2] = screen[k + 1][i][j][2];
            }
        }
    }
}

void push_to_screen() {
    printf("\033[?25l"); // Hide cursor
    printf("\033[H"); // Move cursor to home position

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            int r = 0, g = 0, b = 0;
            for (int k = 0; k < BUFFER_SIZE; k++) {
                r += screen[k][i][j][0];
                g += screen[k][i][j][1];
                b += screen[k][i][j][2];
            }
            printf("\033[48;2;%d;%d;%dm ", 
                clamp_rgb(r / BUFFER_SIZE),
                clamp_rgb(g / BUFFER_SIZE), 
                clamp_rgb(b / BUFFER_SIZE)
            );
        }
        printf("\033[0m\n"); // Reset color and move to the next line
    }
    fflush(stdout);

    cycle_buffer();

    printf("\033[?25h"); // Show cursor
}
