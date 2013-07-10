#version 150

uniform sampler2D tex;
in vec2 fraguv; 
out vec4 finalColor;

void main() { 
	//set every drawn pixel to white
	finalColor = texture(tex, fraguv);
} 