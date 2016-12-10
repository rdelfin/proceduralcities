#version 330 core

layout (lines) in;
layout (line_strip, max_vertices = 2) out;

uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

in vec4 vs_camera_direction[];
in vec4 vs_normal[];

out vec4 camera_direction;
out vec4 vertex_normal;

void main() {
    int n = 0;

    for(n = 0; n < gl_in.length(); n++) {
        //camera_direction = vs_camera_direction[n];
        vertex_normal = vs_normal[n];
        //gl_Position = projection * view * model * gl_in[n].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
