#include "StaticMesh.h"
#include "Engine/Engine.h"
#include "Engine/Engine.h"
#include "Engine/EngineStatics.h"
#include "Utilities/Logger.h"
#include "Core/Math/MathLibrary.h"
#include "Core/Utilities/SerializationStream.h"
#include "Core/Utilities/DeserializationStream.h"

StaticMesh::StaticMesh(const TString& Name) : StaticMesh(Name, "") {

}
StaticMesh::StaticMesh(const TString& Name, const TString& FilePath) : EngineObject(Name), FilePath(FilePath) {
	GPUBufferGenerated = false;
	if (!FilePath.IsEmpty()) {
		LoadModel(FilePath);
	}
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
	const aiScene* scene = import.ReadFile(FilePath.ToCString(), aiProcess_Triangulate |
		aiProcess_FlipUVs |
		aiProcess_CalcTangentSpace |
		//aiProcess_JoinIdenticalVertices |
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
		SubMeshes.Add(subMesh);
	}
	// Handle Children
	for (unsigned int i = 0; i < Node->mNumChildren; i++) {
		this->ProcessNode(Node->mChildren[i], Scene);
	}
}
FSubMesh* StaticMesh::GenerateSubMesh(aiMesh* Mesh) {
	FSubMesh* subMesh = new FSubMesh();
	subMesh->SubMeshMaterial = EngineStatics::GetDefaultMaterial();
	subMesh->Verticies.PreAllocate(Mesh->mNumVertices);
	subMesh->Normals.PreAllocate(Mesh->mNumVertices);
	subMesh->Tangents.PreAllocate(Mesh->mNumVertices);
	subMesh->TexCoords.PreAllocate(Mesh->mNumVertices);
	subMesh->Indices.PreAllocate(Mesh->mNumFaces);

	for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
		vec3 tempPos;
		tempPos.x = Mesh->mVertices[i].x;
		tempPos.y = Mesh->mVertices[i].y;
		tempPos.z = Mesh->mVertices[i].z;

		subMesh->Verticies.Add(tempPos);
	}
	if (Mesh->HasNormals()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			subMesh->Verticies.Add(tempNormal);
		}
	}
	if (Mesh->HasNormals()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			subMesh->Normals.Add(tempNormal);
		}
	}
	if (Mesh->HasTangentsAndBitangents()) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempTangent;
			tempTangent.x = Mesh->mTangents[i].x;
			tempTangent.y = Mesh->mTangents[i].y;
			tempTangent.z = Mesh->mTangents[i].z;

			subMesh->Tangents.Add(tempTangent);
		}
	}
	if (Mesh->HasTextureCoords(0)) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec2 tempUV;
			tempUV.x = Mesh->mTextureCoords[0][i].x;
			tempUV.y = Mesh->mTextureCoords[0][i].y;

			subMesh->TexCoords.Add(tempUV);
		}
	}
	if (Mesh->HasFaces()) {
		for (unsigned int i = 0; i < Mesh->mNumFaces; i++) {
			aiFace tempface = Mesh->mFaces[i];
			for (unsigned int j = 0; j < tempface.mNumIndices; j++) {
				subMesh->Indices.Add(tempface.mIndices[j]);
			}
		}
	}
	if (Mesh->HasVertexColors(0)) {
		for (unsigned int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempColor;
			tempColor.x = Mesh->mColors[i]->r;
			tempColor.y = Mesh->mColors[i]->g;
			tempColor.z = Mesh->mColors[i]->b;

			subMesh->VertexColors.Add(tempColor);
		}
	}

	return subMesh;
}
void StaticMesh::GenerateGPUBuffers() {
	if (GPUBufferGenerated) {
		return;
	}
	for (FSubMesh* subMesh : SubMeshes) {
		// Preallocate the VertexArrayBuffersArray
		for (int i = 0; i < subMesh->GetBufferCount(); i++) {
			subMesh->VertexArrayBuffers.Add(0);
		}

		glGenVertexArrays(1, &subMesh->VertexArrayObject);
		glBindVertexArray(subMesh->VertexArrayObject);

		glGenBuffers(subMesh->GetBufferCount(), &subMesh->VertexArrayBuffers[0]);

		//Position
		if (!subMesh->Verticies.IsEmpty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[POSITION_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Verticies[0]) * subMesh->Verticies.Count(), &subMesh->Verticies[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Tex Coord
		if (!subMesh->TexCoords.IsEmpty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[TEXCOORD_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->TexCoords[0]) * subMesh->TexCoords.Count(), &subMesh->TexCoords[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(1);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Normal
		if (!subMesh->Normals.IsEmpty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[NORMAL_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Normals[0]) * subMesh->Normals.Count(), &subMesh->Normals[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(2);
			glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Tangents
		if (!subMesh->Tangents.IsEmpty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[TANGENT_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->Tangents[0]) * subMesh->Tangents.Count(), &subMesh->Tangents[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Colors
		if (!subMesh->VertexColors.IsEmpty()) {
			glBindBuffer(GL_ARRAY_BUFFER, subMesh->VertexArrayBuffers[COLORS_VB]);
			glBufferData(GL_ARRAY_BUFFER, sizeof(subMesh->VertexColors[0]) * subMesh->VertexColors.Count(), &subMesh->VertexColors[0], GL_STATIC_DRAW);

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		}

		//Index
		if (!subMesh->Indices.IsEmpty()) {
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, subMesh->VertexArrayBuffers[INDEX_VB]);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(subMesh->Indices[0]) * subMesh->Indices.Count(), &subMesh->Indices[0], GL_STATIC_DRAW);
		}

		glBindVertexArray(0);
	}
	GPUBufferGenerated = true;
}
bool StaticMesh::SerializeToBuffer(ByteBuffer& Buffer) const {
	SerializationStream ss(Buffer);
	ss.SerializeInteger32(SubMeshes.Count());
	for (int i = 0; i < SubMeshes.Count(); i++) {
		FSubMesh* subMesh = SubMeshes[i];
		ss.SerializeVec3Array(subMesh->Verticies);
		ss.SerializeVec3Array(subMesh->Normals);
		ss.SerializeVec3Array(subMesh->Tangents);
		ss.SerializeVec3Array(subMesh->VertexColors);
		ss.SerializeVec2Array(subMesh->TexCoords);
		ss.SerializeUnsignedInteger32Array(subMesh->Indices);
	}
	return true;
}
bool StaticMesh::DeserializeFromBuffer(const ByteBuffer& Buffer) {
	DeserializationStream ds(Buffer);
	int32 subMeshCount;
	ds.DeserializeInteger32(subMeshCount);

	for (int i = 0; i < subMeshCount; i++) {
		FSubMesh* subMesh = new FSubMesh();
		ds.DeserializeVec3Array(subMesh->Verticies);
		ds.DeserializeVec3Array(subMesh->Normals);
		ds.DeserializeVec3Array(subMesh->Tangents);
		ds.DeserializeVec3Array(subMesh->VertexColors);
		ds.DeserializeVec2Array(subMesh->TexCoords);
		ds.DeserializeUnsignedInteger32Array(subMesh->Indices);
		SubMeshes.Add(subMesh);
	}
	return true;
}