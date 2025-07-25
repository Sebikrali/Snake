#include "model.h"
#include "pch.h"
#include "shader.h"

Model model_create(const vec3 color) {
    Model model;

    const int num_vertices = 4;
    float vertices[] = {
        0.0f,   0.0f,   // bottom left
        100.0f, 0.0f,   // bottom right
        100.0f, 100.0f, // top right
        0.0f,   100.0f  // top left
    };
    float colors[] = {
        color[0], color[1], color[2], // bottom left
        color[0], color[1], color[2], // bottom right
        color[0], color[1], color[2], // top right
        color[0], color[1], color[2], // top left
    };

    model.num_indices = 6;
    uint32_t indices[] = {
        0, 1, 2,
        2, 3, 0,
    };

    glGenVertexArrays(1, &model.vao);
    glBindVertexArray(model.vao);

    glGenBuffers(1, &model.vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo);
    glBufferData(GL_ARRAY_BUFFER, 2 * sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glGenBuffers(1, &model.vbo_color);
    glBindBuffer(GL_ARRAY_BUFFER, model.vbo_color);
    glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(float) * num_vertices, colors, GL_STATIC_DRAW);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), 0);

    glGenBuffers(1, &model.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * model.num_indices, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return model;
}

void model_draw(Model model, Shader shader, ivec2 pos) {
    glUseProgram(shader.program);
    int loc = glGetUniformLocation(shader.program, "worldPos");
    glUniform2iv(loc, 1, pos);

    glBindVertexArray(model.vao);
    glDrawElements(GL_TRIANGLES, model.num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void model_destroy(Model model) {
    glDeleteBuffers(1, &model.ebo);
    glDeleteBuffers(1, &model.vbo);
    glDeleteVertexArrays(1, &model.vao);
}

