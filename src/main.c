#include <stdlib.h>
#define GLAD_GL_IMPLEMENTATION

#include "pch.h"

#include "model.h"
#include "shader.h"
#include "render.h"
#include "utils.h"

static const char* NAME = "Snake";
static const int WIDTH = 1280;
static const int HEIGHT = 720;

void FormatDebugOutput(char *out, size_t outSize, GLenum source, GLenum type, GLuint id, GLenum severity, const char* msg) {
    const char *sourceString;
    switch (source) {
        case GL_DEBUG_SOURCE_API:             sourceString = "API"; break;
        case GL_DEBUG_SOURCE_APPLICATION:     sourceString = "Application"; break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:   sourceString = "Window System"; break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER: sourceString = "Shader Compiler"; break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:     sourceString = "Third Party"; break;
        case GL_DEBUG_SOURCE_OTHER:           sourceString = "Other"; break;
        default:                              sourceString = "Unknown"; break;
    }

    const char *typeString;
    switch (type) {
        case GL_DEBUG_TYPE_ERROR:               typeString = "Error"; break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR: typeString = "Deprecated Behavior"; break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:  typeString = "Undefined Behavior"; break;
        case GL_DEBUG_TYPE_PORTABILITY:         typeString = "Portability"; break;
        case GL_DEBUG_TYPE_PERFORMANCE:         typeString = "Performance"; break;
        case GL_DEBUG_TYPE_OTHER:               typeString = "Other"; break;
        default:                                typeString = "Unknown"; break;
    }

    const char *severityString;
    switch (severity) {
        case GL_DEBUG_SEVERITY_HIGH:   severityString = "High"; break;
        case GL_DEBUG_SEVERITY_MEDIUM: severityString = "Medium"; break;
        case GL_DEBUG_SEVERITY_LOW:    severityString = "Low"; break;
        default:                       severityString = "Unknown"; break;
    }

    snprintf(out, outSize,
        "OpenGL Error: %s [Source = %s, Type = %s, Severity = %s, ID = %u]",
        msg, sourceString, typeString, severityString, id);
}

void APIENTRY DebugCallbackGL(GLenum source, GLenum type, GLuint id,
                               GLenum severity, GLsizei length,
                               const GLchar *message, const void *userParam) {
    char error[1024];
    FormatDebugOutput(error, sizeof(error), source, type, id, severity, message);
    printf("%s\n", error);
}


void error_callback(int error, const char* description) {
    fprintf(stderr, "[GLFW] Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    } 
}

void handle_movement(GLFWwindow* window, render_t* render, float dt) {
    vec3 direction = {0.0f};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        vec3 forward = { 0.0f, 0.0f, -1.0f }; 
        glm_vec3_add(direction, forward, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        vec3 backwards = { 0.0f, 0.0f, 1.0f }; 
        glm_vec3_add(direction, backwards, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        vec3 left = { -1.0f, 0.0f, 0.0f }; 
        glm_vec3_add(direction, left, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        vec3 right = { 1.0f, 0.0f, 0.0f }; 
        glm_vec3_add(direction, right, direction);
    }

    if (glm_vec3_norm2(direction) != 0) {
        move_debug_render(render, direction, dt);
    }
}


int main() {
    if (!glfwInit()) {
        return -1;
    }
    glfwSetErrorCallback(error_callback);

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, NAME, NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    
    if (!gladLoadGL(glfwGetProcAddress)) {
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbackGL, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); // Disable notifications
    glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    render_t render;
    setup_render(&render, WIDTH, HEIGHT);
    render_t debug_render;
    setup_debug_render(&debug_render, WIDTH, HEIGHT);

    model_t model = create_model();

    shader_t shader = create_shader("assets/shader/snake.vert", "assets/shader/snake.frag");
    if (shader.program == 0) {
        return -1;
    }
    float vertices[] = {
        0.5f,  0.5f, 0.0f,  // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  // bottom left
        -0.5f,  0.5f, 0.0f   // top left 
    };
    unsigned int indices[] = {  // note that we start from 0!
        0, 1, 3,  // first Triangle
        1, 2, 3   // second Triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0); 
    glBindVertexArray(0);


    // glClearColor(0.0f, 0.5f, 0.0f, 1.0f);
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);

    mat4 viewProj;
    glm_ortho(0.0f, (float) WIDTH, (float) HEIGHT, 0.0f, -1.0f, 1.0f, viewProj);

    // mat4 view = {
    //     1.0f, 0.0f, 0.0f, 0.0f,
    //     0.0f, 1.0f, 0.0f, 0.0f,
    //     0.0f, 0.0f, 1.0f, -2.0f,
    //     1.0f, 0.0f, 0.0f, 1.0f,
    // };
    // mat4 proj;
    // glm_perspective(60.0f, (float) WIDTH / HEIGHT, 0.1f, 1.0f, proj);
    // glm_mat4_dup(proj, render.proj);
    // glm_mat4_mul(proj, view, render.viewProj);

    float t = (float) glfwGetTime();
    float dt = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        dt = t;
        t = (float) glfwGetTime();
        dt = t - dt;

        // handle_movement(window, &debug_render, dt);
        glfwPollEvents();

        // use_shader(shader, render.viewProj);
        // use_shader(shader, debug_render.viewProj);
        glUseProgram(shader.program);
        int loc = glGetUniformLocation(shader.program, "viewProj");
        // print_mat4(debug_render.viewProj, "viewProj");
        // glUniformMatrix4fv(loc, 1, GL_FALSE, (float*) debug_render.viewProj);
        glUniformMatrix4fv(loc, 1, GL_FALSE, (float*) viewProj);
        // draw_model(model);

        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glfwSwapBuffers(window);
    }

    destroy_model(model);

    glfwDestroyWindow(window);
    glfwTerminate();
}
