#include "StaticMesh.h"
#include "Engine/Engine.h"
#include "Engine/Engine.h"
#include "Engine/EngineStatics.h"
#include "Utilities/Logger.h"
#include "Utilities/Math/MathLibrary.h"

StaticMesh::StaticMesh(const TString& Name, const TString& FilePath) : EngineObject(Name) {
	LoadModel(FilePath);
}
StaticMesh::~StaticMesh() {
	for (FSubMesh* subMesh : SubMeshes) {
		glDeleteVertexArrays(1, &subMesh->VertexArrayObject);
	}
}

SArray<FSubMesh*> StaticMesh::GetSubMeshes() {
	return SubMeshes;
}

void StaticMesh::LoadModel(const TString FilePath) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(FilePath, aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		SD_ENGINE_ERROR("Error: Failed to import model {0} with error {1}.", FilePath, import.GetErrorString());
		return;
	}
	ProcessNode(scene->mRootNode, scene);
	GenerateGPUBuffers();
}
void StaticMesh::ProcessNode(aiNode* Node, const aiScene* Scene) {
	// Handle Root
	for (GLuint i = 0; i < Node->mNumMeshes; i++) {
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		FSubMesh* subMesh = GenerateSubMesh(mesh);
		SubMeshes.push_back(subMesh);
	}
	// Handle Children
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		this->ProcessNode(Node->mChildren[i], Scene);
	}
}
FSubMesh* StaticMesh::GenerateSubMesh(aiMesh* Mesh) {
	FSubMesh* subMesh = new FSubMesh();
	subMesh->SubMeshMaterial = EngineStatics::GetDefaultMaterial();
	subMesh->Verticies.reserve(Mesh->mNumVertices);
	subMesh->Normals.reserve(Mesh->mNumVertices);
	subMesh->Tangents.reserve(Mesh->mNumVertices);
	subMesh->TexCoords.reserve(Mesh->mNumVertices);
	subMesh->Indices.reserve(Mesh->mNumFaces);

	for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
		vec3 tempPos;
		tempPos.x = Mesh->mVertices[i].x;
		tempPos.y = Mesh->mVertices[i].y;
		tempPos.z = Mesh->mVertices[i].z;

		subMesh->Verticies.push_back(tempPos);
	}
	if (Mesh->HasNormals()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			subMesh->Verticies.push_back(tempNormal);
		}
	}
	if (Mesh->HasNormals()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			subMesh->Normals.push_back(tempNormal);
		}
	}
	if (Mesh->HasTangentsAndBitangents()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempTangent;
			tempTangent.x = Mesh->mTangents[i].x;
			tempTangent.y = Mesh->mTangents[i].y;
			tempTangent.z = Mesh->mTangents[i].z;

			subMesh->Tangents.push_back(tempTangent);
		}
	}
	if (Mesh->HasTextureCoords(0)) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec2 tempUV;
			tempUV.x = Mesh->mTextureCoords[0][i].x;
			tempUV.y = Mesh->mTextureCoords[0][i].y;

			subMesh->TexCoords.push_back(tempUV);
		}
	}
	if (Mesh->HasFaces()) {
		for (unsigned int i = 0; i < Mesh->mNumFaces; i++) {
			aiFace tempface = Mesh->mFaces[i];
			for (unsigned int j = 0; j < tempface.mNumIndices; j++) {
				subMesh->Indices.push_back(tempface.mIndices[j]);
			}
		}
	}
	if (Mesh->HasVertexColors(0)) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempColor;
			tempColor.x = Mesh->mColors[i]->r;
			tempColor.y = Mesh->mColors[i]->g;
			tempColor.z = Mesh->mColors[i]->b;

			subMesh->VertexColors.push_back(tempColor);
		}
	}

	return subMesh;
}
void StaticMesh::GenerateGPUBuffers() {
	for (FSubMesh* subMesh : SubMeshes) {
		// Preallocate the VertexArrayBuffersArray
		for (int i = 0; i < subMesh->GetBufferCount(); i++) {
			subMesh->VertexArrayBuffers.push_back(0);
		}

		glGenVertexArrays(1, &subMesh->VertexArrayObject);
		glBindVertexArray(subMesh->VertexArrayObject);

		glGenBuffers(subMesh->GetBufferCount(), &subMesh->VertexArrayBuffers[0]);

		//Position
		if (!subMesh->Verticies.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Verticies[0]) * subMesh->Verticies.size(), &subMesh->Verticies[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Tex Coord
		if (!subMesh->TexCoords.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->TexCoords[0]) * subMesh->TexCoords.size(), &subMesh->TexCoords[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Normal
		if (!subMesh->Normals.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Normals[0]) * subMesh->Normals.size(), &subMesh->Normals[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}
		//Colors
		if (!subMesh->VertexColors.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[COLORS_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->VertexColors[0]) * subMesh->VertexColors.size(), &subMesh->VertexColors[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Tangents
		if (!subMesh->Tangents.empty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[TANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Tangents[0]) * subMesh->Tangents.size(), &subMesh->Tangents[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Index
		if (!subMesh->Indices.empty()) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh->VertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(subMesh->Indices[0]) * subMesh->Indices.size(), &subMesh->Indices[0], GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
	}
}