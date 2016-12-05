#version 330 core

layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;
uniform mat4 projection;
uniform mat4 model;
uniform mat4 view;

void main() {
    int n = 0;

    for(n = 0; n < gl_in.length(); n++) {
        gl_Position = projection * view * model * gl_in[n].gl_Position;
        EmitVertex();
    }

    EndPrimitive();
}
