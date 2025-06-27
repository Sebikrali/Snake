#include "model.h"
#include "pch.h"

model_t create_model() {
    model_t model;

    const int num_vertices = 4 * 2;
    float vertices[] = {
        0.0f,   100.0f, // bottom left
        100.0f, 100.0f, // bottom right
        100.0f, 0.0f,   // top right
        0.0f,   0.0f,   // top left
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
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * num_vertices, vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);

    glGenBuffers(1, &model.ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model.ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * model.num_indices, indices, GL_STATIC_DRAW);

    glBindVertexArray(0);

    return model;
}

void draw_model(model_t model) {
    glBindVertexArray(model.vao);
    glDrawElements(GL_TRIANGLES, model.num_indices, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void destroy_model(model_t model) {
    glDeleteBuffers(1, &model.ebo);
    glDeleteBuffers(1, &model.vbo);
    glDeleteVertexArrays(1, &model.vao);
}

