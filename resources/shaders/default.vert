#version 330 core

uniform vec3 camera_position;

in vec4 vertex_position;
in vec4 normal;

out vec4 vs_normal;
out vec4 vs_camera_direction;

void main() {
    gl_Position = vertex_position;
    vs_normal = normal;
    vs_camera_direction = vec4(camera_position - vec3(gl_Position), 0.0);
}
