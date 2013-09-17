#version 150

in vec3 vert;
in vec2 vertuv;
out vec2 fraguv;

uniform mat4 texm;

void main() {
	fraguv = vec2(texm * vec4(vertuv,0,1));
	// fraguv.y = 1.0-fraguv.y;
	gl_Position = vec4(vert, 1);
}