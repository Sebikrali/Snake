#pragma once

#include "pch.h"
#include "shader.h"

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint vbo_color;
    GLuint ebo;
    int num_indices;
} Model;

Model model_create(const vec3 color);

void model_draw(Model model, Shader shader, ivec2 pos);

// TODO: Figure out if this needs to be a pointer param (i think not, because i can just stop using the object after destroying)
void model_destroy(Model model);
