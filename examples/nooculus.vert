#version 150

in vec3 vert;
in vec2 vertuv;
out vec2 fraguv;

void main() {
	gl_Position = vec4(vert, 1);
	fraguv = vertuv;
}