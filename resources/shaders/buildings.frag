#version 330 core

in vec4 vertex_normal;
out vec4 vertex_color;

void main() {
    vec4 normal = vertex_normal;

    if (length(normal - vec4(1.0f, 0.0f, 0.0f, 0.0f)) < 0.01 || length(normal - vec4(-1.0f, 0.0f, 0.0f, 0.0f)) < 0.01) {
        vertex_color = vec4(0.65f, 0.65f, 0.65f, 1.0f);
    }
    else if (length(normal - vec4(0.0f, 1.0f, 0.0f, 0.0f)) < 0.01 || length(normal - vec4(0.0f, -1.0f, 0.0f, 0.0f)) < 0.01) {
        vertex_color = vec4(0.75f, 0.75f, 0.75f, 1.0f);
    }
    else if (length(normal - vec4(0.0f, 0.0f, 1.0f, 0.0f)) < 0.01 || length(normal - vec4(0.0f, 0.0f, -1.0f, 0.0f)) < 0.01) {
        vertex_color = vec4(0.85f, 0.85f, 0.85f, 1.0f);
    }
    else {
        if (abs(normal[0]) > 0.001) {
            vertex_color = vec4(0.70f, 0.70f, 0.70f, 1.0f);
        }
        else {
            vertex_color = vec4(0.90f, 0.90f, 0.90f, 1.0f);
        }
    }
}
