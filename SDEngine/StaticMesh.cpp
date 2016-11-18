#include "StaticMesh.h"


StaticMesh::StaticMesh(Vertex* Verticies, unsigned int NumVertecies) {
	S_DrawCount = NumVertecies;

	glGenVertexArrays(1, &S_VertexArrayObject);
	glBindVertexArray(S_VertexArrayObject);

	glGenBuffers(NUM_BUFFERS, S_VertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, NumVertecies * sizeof(Verticies[0]), Verticies, GL_STATIC_DRAW);

	GLint posAttrib;
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib, 3, GL_FLOAT, false, NumVertecies * sizeof(Verticies[0]), 0);

	GLint texCoordAttrib;
	glEnableVertexAttribArray(texCoordAttrib);
	glVertexAttribPointer(texCoordAttrib, 2, GL_FLOAT, false, NumVertecies * sizeof(Verticies[0]), (void*)(3 * sizeof(float)));

	GLuint colorBuffer;
	glGenBuffers(1, &colorBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, colorBuffer);
	glBufferData(GL_ARRAY_BUFFER, NumVertecies * sizeof(Verticies[0]), (void*)(5 * sizeof(float)), GL_STATIC_DRAW);

	glBindVertexArray(0);
}
StaticMesh::~StaticMesh() {
	glDeleteVertexArrays(1, &S_VertexArrayObject);
}

void StaticMesh::Draw() {
	glBindVertexArray(S_VertexArrayObject);

	glDrawArrays(GL_TRIANGLES, 0, S_DrawCount);

	glBindVertexArray(0);
}