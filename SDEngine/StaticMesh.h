#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <string>
#include "Shader.h"

using namespace glm;

class Vertex {

public:
	Vertex(const vec3& Position, const vec2& TextureCoord, const vec4& Color) {
		this->Position = Position;
		this->TextureCoord = TextureCoord;
		this->Color = Color;
	}
	Vertex(const vec3& Position, const vec2& TextureCoord) {
		this->Position = Position;
		this->TextureCoord = TextureCoord;
		this->Color = { 1,1,1,1 };
	}
	Vertex(const vec3& Position, const vec4& Color) {
		this->Position = Position;
		this->TextureCoord = { 1,1 };
		this->Color = Color;
	}
	Vertex(const vec3& Position) {
		this->Position = Position;
		this->TextureCoord = { 1,1 };
		this->Color = { 1,1,1,1 };
	}
private:
	vec3 Position;
	vec2 TextureCoord;
	vec4 Color;
};

class StaticMesh {

public:
	StaticMesh(Vertex* Verticies, unsigned int NumVertecies);
	virtual ~StaticMesh();

	void Draw();

private:
	void operator=(const StaticMesh& other) {}

	enum {
		POSITION_VB,

		NUM_BUFFERS
	};

	GLuint S_VertexArrayObject;
	GLuint S_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int S_DrawCount;

};

