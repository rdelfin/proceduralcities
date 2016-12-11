// GLSL GEOMETRY SHADER
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

void main ()
{
    vertex_normal = vs_normal[0];
    gl_Position = projection * view * model * gl_in[0].gl_Position;
    EmitVertex();
    vertex_normal = vs_normal[1];
    gl_Position = projection * view * model * gl_in[1].gl_Position;
    EmitVertex();
    EndPrimitive();
}
