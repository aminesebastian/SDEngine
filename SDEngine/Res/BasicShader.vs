#version 120

attribute vec3 position;
in vec2 vertexUV;

out vec2 texcoord;

void main() {
	gl_Position = vec4(position, 1.0);
	texCoord = texcoord;
}