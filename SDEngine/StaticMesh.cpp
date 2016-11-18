#include "StaticMesh.h"


StaticMesh::StaticMesh(Vertex* Verticies, unsigned int NumVertecies) {
	S_DrawCount = NumVertecies;

	glGenVertexArrays(1, &S_VertexArrayObject);
	glBindVertexArray(S_VertexArrayObject);

	glGenBuffers(NUM_BUFFERS, S_VertexArrayBuffers);
	glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, NumVertecies * sizeof(Verticies[0]), Verticies, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, false, 0, 0); //Skip over Vertex Colors

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