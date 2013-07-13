#version 150

uniform mat4 projection;
uniform mat4 camera;

in vec3 vert;
in vec3 color;

out vec3 fragColor;

void main() {
	fragColor = color;
	gl_Position = projection * camera * vec4(vert, 1);
}