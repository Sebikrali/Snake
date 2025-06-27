#pragma once

#include "pch.h"

typedef struct  {
    mat4 viewProj;
    mat4 proj;
} render_t;

void setup_render(render_t* render, int width, int height);

typedef struct  {
    mat4 viewProj;
    mat4 proj;
    vec3 pos, front, up;
    float pitch, yaw;
} debug_render_t;

void setup_debug_render(debug_render_t* render, int width, int height);

void move_debug_render(debug_render_t* render, vec3 direction, float dt);
void change_view_debug_render(debug_render_t* render, vec2 delta);

