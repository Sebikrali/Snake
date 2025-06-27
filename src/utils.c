#include "utils.h"

void print_vec3(const vec3 vec, const char* name) {
    printf("Vec3 '%s' = (%.1f, %.1f, %.1f)\n", name, vec[0], vec[1], vec[2]);
}

void print_mat4(const mat4 mat, const char* name) {
    printf("Mat4 '%s'\n", name);
    for (int i = 0; i < 4; ++i) {
        printf("| ");
        for (int j = 0; j < 4; ++j) {
            printf("%.5f ", mat[j][i]);
        }
        printf("|\n");
    }
}
