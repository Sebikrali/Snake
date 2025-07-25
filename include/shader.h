#pragma once

#include "pch.h"

// Shader
typedef struct {
    GLuint program;
    const char* vs;
    const char* fs;
} Shader;

Shader shader_create(const char* vs_path, const char* fs_path);

void shader_use(Shader shader, mat4 viewProj);
