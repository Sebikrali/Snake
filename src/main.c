#define GLAD_GL_IMPLEMENTATION

#include "pch.h"

#include "model.h"
#include "shader.h"
#include "render.h"
#include "utils.h"

static const char* NAME = "Snake";
static const int WIDTH = 1280;
static const int HEIGHT = 720;

static bool g_firstMouse = true;
static double xPos = 0.0;
static double yPos = 0.0;

static bool g_debugCamera = false;
static debug_render_t debug_render;

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

    if (action != GLFW_PRESS) return; 
    switch (key) {
        case GLFW_KEY_F1:
            g_debugCamera = !g_debugCamera;
            if (g_debugCamera) {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            } else {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            break;
    }
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!g_debugCamera) return;

    if (g_firstMouse) {
        xPos = xpos;
        yPos = ypos;
        g_firstMouse = false;
        return;
    }
    vec2 delta = { xpos - xPos, ypos - yPos };
    change_view_debug_render(&debug_render, delta);
    xPos = xpos;
    yPos = ypos;
}

void handle_movement(GLFWwindow* window, debug_render_t* render, float dt) {
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
    glfwSetCursorPosCallback(window, cursor_position_callback);
    
    if (!gladLoadGL(glfwGetProcAddress)) {
        return -1;
    }

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(DebugCallbackGL, NULL);
    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE); // Disable notifications
    glViewport(0, 0, WIDTH, HEIGHT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glDisable(GL_CULL_FACE);

    render_t render;
    setup_render(&render, WIDTH, HEIGHT);

    setup_debug_render(&debug_render, WIDTH, HEIGHT);

    model_t model = create_model();

    shader_t shader = create_shader("assets/shader/snake.vert", "assets/shader/snake.frag");
    if (shader.program == 0) {
        return -1;
    }

    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

    float t = (float) glfwGetTime();
    float dt = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        dt = t;
        t = (float) glfwGetTime();
        dt = t - dt;

        handle_movement(window, &debug_render, dt);
        glfwPollEvents();

        if (!g_debugCamera) {
            use_shader(shader, render.viewProj);
        } else {
            use_shader(shader, debug_render.viewProj);
        }
        draw_model(model);

        glfwSwapBuffers(window);
    }

    destroy_model(model);

    glfwDestroyWindow(window);
    glfwTerminate();
}
