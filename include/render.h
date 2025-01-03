#ifndef RENDER_H
#define RENDER_H

#include "linalg.h"
#include "structs.h"

#define WIDTH 200
#define HEIGHT 100

#define ENABLE_LIGHTING
#define BUFFER_SIZE 5

void initialize_screen();
void clear_screen();
void fill(float color[4]);
void draw_pixel(vec3 pos, float color[4]);
void draw_mesh(mesh m, light* lights, int num_lights);
void push_to_screen();

#endif
