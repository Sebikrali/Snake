#version 450

layout(location = 0) in vec2 pos;
layout(location = 1) in vec3 color;

uniform mat4 viewProj;
uniform ivec2 worldPos;

out vec3 fragColor;

void main() {
    gl_Position = viewProj * vec4(pos + worldPos * 100, 0.0, 1.0);
    fragColor = color;
}
