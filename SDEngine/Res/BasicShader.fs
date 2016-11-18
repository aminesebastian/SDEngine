#version 120

in vec4 color;
in vec2 texCoord;

uniform sampled2D albedo;

varying out vec4 outColor;

void main() {
	outColor = color;
}