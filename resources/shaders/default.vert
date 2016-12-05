#version 330 core

in vec4 vertex_position;
in vec4 normal;

out vec4 vs_normal;

void main() {
    gl_Position = vertex_position;
    vs_normal = normal;
}
