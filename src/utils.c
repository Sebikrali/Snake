#include "utils.h"

void print_vec3(const vec3 vec, const char* name) {
    printf("Vec3 '%s' = (%.1f, %.1f, %.1f)\n", name, vec[0], vec[1], vec[2]);
}

void print_mat4(const mat4 mat, const char* name) {
    printf("Mat4 '%s'\n", name);
    printf("|%.1f, %.1f, %.1f, %.1f|\n", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
    printf("|%.1f, %.1f, %.1f, %.1f|\n", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
    printf("|%.1f, %.1f, %.1f, %.1f|\n", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
    printf("|%.1f, %.1f, %.1f, %.1f|\n", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
}
