#pragma once

#include "pch.h"

typedef struct  {
    mat4 viewProj;
    mat4 proj;
} Render;

void render_create(Render* render, int width, int height);

typedef struct  {
    mat4 viewProj;
    mat4 proj;
    vec3 pos, front, up;
    float pitch, yaw;
} DebugRender;

void debug_render_create(DebugRender* render, int width, int height);

void debug_render_move(DebugRender* render, vec3 direction, float dt);
void debug_render_change_view(DebugRender* render, vec2 delta);
