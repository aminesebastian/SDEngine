#include "StaticMesh.h"
#include <vector>

StaticMesh::StaticMesh(const Transform& SpawnTransform, const std::string& FileName) {
	InitMesh(OBJModel(FileName).ToIndexedModel());
	S_Transform = SpawnTransform;
}
StaticMesh::StaticMesh(const Transform& SpawnTransform, Vertex* Verticies, unsigned int NumVertecies, unsigned int* Indicies, unsigned int NumIndicides) {
	IndexedModel model;

	model.positions.reserve(NumVertecies);
	model.texCoords.reserve(NumVertecies);

	for (int i = 0; i < NumVertecies; i++) {
		model.positions.push_back(Verticies[i].GetPosition());
		model.texCoords.push_back(Verticies[i].GetTexCoord());
		model.normals.push_back(Verticies[i].GetNormal());
	}

	for (int i = 0; i < NumIndicides; i++) {
		model.indices.push_back(Indicies[i]);
	}

	InitMesh(model);

}
StaticMesh::~StaticMesh() {
	glDeleteVertexArrays(1, &S_VertexArrayObject);
}
void StaticMesh::InitMesh(const IndexedModel& model) {
	S_DrawCount = model.indices.size();

	glGenVertexArrays(1, &S_VertexArrayObject);
	glBindVertexArray(S_VertexArrayObject);

	glGenBuffers(NUM_BUFFERS, S_VertexArrayBuffers);

	if (model.positions.size() <= 0) {
		return;
	}
	//Position
	glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[POSITION_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.positions[0]) * model.positions.size(), &model.positions[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	//Texcoord
	glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[TEXCOORD_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.texCoords[0]) * model.texCoords.size(), &model.texCoords[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);

	//Normal
	glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[NORMAL_VB]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(model.normals[0]) * model.normals.size(), &model.normals[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, S_VertexArrayBuffers[INDEX_VB]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(model.indices[0]) * model.indices.size(), &model.indices[0], GL_STATIC_DRAW);

	glBindVertexArray(0);
}
void StaticMesh::Draw() {
	glBindVertexArray(S_VertexArrayObject);

	glDrawElements(GL_TRIANGLES, S_DrawCount, GL_UNSIGNED_INT, 0);

	glBindVertexArray(0);
}