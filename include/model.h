#pragma once

#include "pch.h"

typedef struct {
    GLuint vao;
    GLuint vbo;
    GLuint ebo;
    int num_indices;
} model_t;

model_t create_model();

void draw_model(model_t model);

// TODO: Figure out if this needs to be a pointer param (i think not, because i can just stop using the object after destroying)
void destroy_model(model_t model);
