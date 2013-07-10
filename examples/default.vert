#version 150

uniform mat4 projection;
uniform mat4 camera;
uniform mat4 model;

in vec3 vert;
in vec2 vertuv;
out vec2 fraguv;

void main() {
	fraguv = vertuv;
	gl_Position = projection * camera * model * vec4(vert, 1);
}