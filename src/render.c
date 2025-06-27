#include "render.h"

void setup_render(render_t* render, int width, int height) {
    glm_ortho(0.0f, (float) width, 0.0f, (float) height, -1.0f, 1.0f, render->viewProj);
}

void setup_debug_render(debug_render_t* render, int width, int height) {
    render->yaw = -90.0f;
    render->pitch = 0.0f;
    vec3 pos = { 0.0f, 0.0f, 2.0f };
    vec3 front = { 0.0f, 0.0f, -1.0f };
    vec3 up = { 0.0f, 1.0f, 0.0f };

    glm_vec3_dup(pos, render->pos);
    glm_vec3_dup(front, render->front);
    glm_vec3_dup(up, render->up);
    
    mat4 view;
    glm_lookat(pos, front, up, view);

    mat4 proj;
    glm_perspective(glm_rad(45.0f), (float) width / (float) height, 0.1f, 100.0f, proj);
    glm_mat4_dup(proj, render->proj);
    glm_mat4_mul(proj, view, render->viewProj);
}

void move_debug_render(debug_render_t* render, vec3 direction, float dt) {
    glm_vec3_scale(direction, dt * 10, direction);
    glm_vec3_add(render->pos, direction, render->pos);

    vec3 center;
    glm_vec3_add(render->pos, render->front, center);

    mat4 view;
    glm_lookat(render->pos, center, render->up, view);
    glm_mat4_mul(render->proj, view, render->viewProj);
}

void change_view_debug_render(debug_render_t* render, vec2 delta) {
    float sensitivity = 0.1f;
    render->yaw += delta[0] * sensitivity;
    render->pitch -= delta[1] * sensitivity;

    if (render->pitch > 90.0f) {
        render->pitch = 90.0f;
    } else if (render->pitch < -90.0f) {
        render->pitch = -90.0f;
    }

    float yaw = glm_rad(render->yaw);
    float pitch = glm_rad(render->pitch);
    render->front[0] = cos(pitch) * cos(yaw);
    render->front[1] = sin(pitch);
    render->front[2] = cos(pitch) * sin(yaw);
    glm_normalize(render->front);

    vec3 center;
    glm_vec3_add(render->pos, render->front, center);

    vec3 up = { 0.0f, 1.0f, 0.0f };
    vec3 right;
    glm_vec3_cross(render->front, up, right);
    glm_normalize(right);

    glm_vec3_cross(right, render->front, render->up);
    glm_normalize(render->up);

    mat4 view;
    glm_lookat(render->pos, center, render->up, view);
    glm_mat4_mul(render->proj, view, render->viewProj);
}

