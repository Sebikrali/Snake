#version 450

layout (location = 0) in vec2 pos;

uniform mat4 viewProj;

void main() {
    gl_Position = viewProj * vec4(pos, 0.0, 1.0);
    // gl_Position = vec4(pos, 0.0, 1.0);
}
