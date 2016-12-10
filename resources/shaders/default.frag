#version 330 core
#line 1

in vec4 vertex_normal;
in vec4 camera_direction;


uniform vec3 texcolor = vec3(1.0, 1.0, 1.0);
uniform vec3 diffuse = vec3(0.5, 0.5, 0.5);
uniform vec3 ambient = vec3(0.3, 0.3, 0.3);
uniform vec3 specular = vec3(0.1, 0.1, 0.1);
uniform vec4 light_direction = vec4(-1.0, 1.0, 0.0, 0.0);
uniform float alpha = 1.0;
uniform float shininess = 1.0;

out vec4 vertex_color;

void main() {
    float dot_nl = dot(normalize(light_direction), normalize(vertex_normal));
    dot_nl = clamp(dot_nl, 0.0, 1.0);
    vec3 spec = specular * max(0.0, dot(reflect(-light_direction, vertex_normal), camera_direction));
    vec3 color = clamp(dot_nl * diffuse + ambient + vec3(spec), 0.0, 1.0);
    vertex_color = vec4(color, alpha);
}
