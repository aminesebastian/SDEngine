#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
using namespace glm;

class Vertex {

public:
	Vertex(const vec3& Position) {
		this->Position = Position;
	}
private:
	vec3 Position;
};

class StaticMesh {

public:
	StaticMesh(Vertex* Verticies, unsigned int NumVertecies);
	virtual ~StaticMesh();

	void Draw();

private:
	StaticMesh(const StaticMesh& other) {}
	void operator=(const StaticMesh& other) {}

	enum {
		POSITION_VB,

		NUM_BUFFERS
	};

	GLuint S_VertexArrayObject;
	GLuint S_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int S_DrawCount;

};

