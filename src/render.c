#include "render.h"
#include "cglm/vec3.h"

void setup_render(render_t* render, int width, int height) {
    glm_ortho(0.0f, (float) width, 0.0f, (float) height, -1.0f, 1.0f, render->viewProj);
}

void setup_debug_render(render_t* render, int width, int height) {
    mat4 view = {
        {1.0f, 0.0f, 0.0f, 0.0f},
        {0.0f, 1.0f, 0.0f, 0.0f},
        {0.0f, 0.0f, 1.0f, -5.0f},
        {0.0f, 0.0f, 0.0f, 1.0f},
    };
    vec3 pos = { 0.0f, 0.0f, 2.0f };
    glm_vec3_dup(pos, render->pos);
    mat4 proj;
    glm_perspective(glm_rad(60.0f), (float) width / height, 0.1f, 100.0f, proj);
    glm_mat4_dup(proj, render->proj);
    glm_mat4_mul(proj, view, render->viewProj);
}

void move_debug_render(render_t* render, vec3 direction, float dt) {
    // FIX: this
    glm_vec3_scale(direction, dt * 0.5, direction);
    glm_vec3_add(render->pos, direction, render->pos);

    vec4 pos;
    glm_vec4(render->pos, 1.0f, pos);
    glm_vec4_inv(pos);

    mat4 view;
    glm_translate(view, pos);
    glm_mat4_mul(render->proj, view, render->viewProj);
}
