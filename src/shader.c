#include "pch.h"
#include "shader.h"
#include "utils.h"

GLuint read_shader(const char* path, GLenum type) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "[Shader] Error: Couldn't open shader '%s'\n", path);
        return 0;
    }

    fseek(file, 0L, SEEK_END);
    size_t size = ftell(file);
    fseek(file, 0L, SEEK_SET);

    char* code = (char*) malloc(size + 1);
    if (!code) {
        fprintf(stderr, "[Shader] Error: Out of memory for '%s'\n", path);
        fclose(file);
        return 0;
    }

    size_t read = fread(code, sizeof(char), size, file);
    fclose(file);
    if (read < size) {
        fprintf(stderr, "[Shader] Error: Couldn't read shader '%s'\n", path);
        free(code);
        return 0;
    }

    code[read] = '\0';
    const char* src = code;

    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        fprintf(stderr, "[Shader] Error: Couldn't create shader '%s'\n", path);
        free(code);
        return 0;
    }
    glShaderSource(shader, 1, &src, NULL);
    glCompileShader(shader);

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char log[512];
        glGetShaderInfoLog(shader, 512, NULL, log);
        fprintf(stderr, "[Shader] Error: Couldn't compile shader '%s', Log:\n%s\n", path, log);
        free(code);
        glDeleteShader(shader);
        return 0;
    }

    free(code);
    return shader;
}

shader_t create_shader(const char* vs_path, const char* fs_path) {
    shader_t shader;
    shader.program = 0;
    shader.vs = vs_path;
    shader.fs = fs_path;

    // Create shader objects
    GLuint vs = read_shader(vs_path, GL_VERTEX_SHADER);
    if (vs == 0) {
        return shader;
    }
    GLuint fs = read_shader(fs_path, GL_FRAGMENT_SHADER);
    if (fs == 0) {
        return shader;
    }

    shader.program = glCreateProgram();
    glAttachShader(shader.program, vs);
    glAttachShader(shader.program, fs);
    glLinkProgram(shader.program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    int success;
    glGetProgramiv(shader.program, GL_LINK_STATUS, &success);
    if (!success) {
        char log[512];
        glGetProgramInfoLog(shader.program, 512, NULL, log);
        fprintf(stderr, "[Shader] Error: Couldn't link program ('%s', '%s'), Log:\n%s\n", vs_path, fs_path, log);
        shader.program = 0;
    }

    return shader;
}

void use_shader(shader_t shader, mat4 viewProj) {
    glUseProgram(shader.program);

    int loc = glGetUniformLocation(shader.program, "viewProj");
    glUniformMatrix4fv(loc, 1, GL_FALSE, viewProj[0]);
}

