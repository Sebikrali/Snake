#define GLAD_GL_IMPLEMENTATION

#include "pch.h"

#include "game.h"
#include "shader.h"
#include "render.h"
#include "utils.h"

static const char* NAME = "Snake";
static const int WIDTH = 1000;
static const int HEIGHT = 1000;

static bool g_firstMouse = true;
static double xPos = 0.0;
static double yPos = 0.0;

static bool g_debugCamera = false;
static DebugRender debug_render;

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
    debug_render_change_view(&debug_render, delta);
    xPos = xpos;
    yPos = ypos;
}

void handle_movement(GLFWwindow* window, Game* game) {
    ivec2 direction = {0};
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        ivec2 up = { 0, 1 }; 
        glm_ivec2_add(direction, up, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        ivec2 down = { 0, -1 }; 
        glm_ivec2_add(direction, down, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        ivec2 left = { -1, 0 }; 
        glm_ivec2_add(direction, left, direction);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        ivec2 right = { 1, 0 }; 
        glm_ivec2_add(direction, right, direction);
    }

    if (direction[0] != 0 || direction[1] != 0) {
        ivec2 res;
        glm_ivec2_add(direction, game->direction, res);
        if (res[0] != 0 || res[1] != 0) {
            glm_ivec2_copy(direction, game->direction);
        }
    }
}

void handle_movement_debug_render(GLFWwindow* window, DebugRender* render, float dt) {
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
        debug_render_move(render, direction, dt);
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

    Render render;
    render_create(&render, WIDTH, HEIGHT);

    debug_render_create(&debug_render, WIDTH, HEIGHT);

    Game game;
    game_create(&game);

    Shader shader = shader_create("assets/shader/snake.vert", "assets/shader/snake.frag");
    if (shader.program == 0) {
        return -1;
    }

    glClearColor(0.0f, 0.5f, 0.0f, 1.0f);

    float t = (float) glfwGetTime();
    float dt = 0.0f;
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glfwPollEvents();

        dt = t;
        t = (float) glfwGetTime();
        dt = t - dt;

        // handle_movement_debug_render(window, &debug_render, dt);
        handle_movement(window, &game);

        if (!g_debugCamera) {
            shader_use(shader, render.viewProj);
        } else {
            shader_use(shader, debug_render.viewProj);
        }

        game_update(&game, dt);
        game_draw(&game, shader);

        glfwSwapBuffers(window);
    }

    game_destroy(&game);

    glfwDestroyWindow(window);
    glfwTerminate();
}
