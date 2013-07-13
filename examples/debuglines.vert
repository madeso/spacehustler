#version 150

uniform mat4 projection;
uniform mat4 camera;

in vec3 vert;

void main() {
	gl_Position = projection * camera * vec4(vert, 1);
}