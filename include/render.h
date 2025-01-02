#ifndef RENDER_H
#define RENDER_H

#include "linalg.h"
#include "structs.h"

#define WIDTH 200
#define HEIGHT 100

#define DOUBLE_BUFFER

void initialize_screen();
void clear_screen();
void fill(int color[4]);
void draw_pixel(vec3 pos, int color[4]);
void draw_triangle(triangle t, draw_mode mode);
void draw_line(vec3 v1, vec3 v2, int color[4]);
void draw_mesh(mesh m);
void push_to_screen();

#endif
