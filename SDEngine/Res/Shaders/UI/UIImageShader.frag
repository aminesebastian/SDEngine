#version 400
in vec2 texCoord0;
in vec2 screenPos0;

uniform vec4 TINT;

uniform sampler2D IMAGE;

out vec4 FragColor;

void main() {
	FragColor = texture(IMAGE, texCoord0) * TINT;
}


