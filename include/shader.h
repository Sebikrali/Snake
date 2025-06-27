#pragma once

#include "pch.h"

typedef struct {
    GLuint program;
    const char* vs;
    const char* fs;
} shader_t;

shader_t create_shader(const char* vs_path, const char* fs_path);

void use_shader(shader_t shader, mat4 viewProj);

