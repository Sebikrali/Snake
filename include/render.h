#pragma once

#include "pch.h"

typedef struct  {
    mat4 viewProj;
    mat4 proj;
    vec3 pos;
} render_t;

void setup_render(render_t* render, int width, int height);
void setup_debug_render(render_t* render, int width, int height);

void move_debug_render(render_t* render, vec3 direction, float dt);

