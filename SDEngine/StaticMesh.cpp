#include "StaticMesh.h"
#include "Engine.h"
#include <iostream>
#include "Engine.h"
#include "EngineStatics.h"

StaticMesh::StaticMesh(TString Name, const Transform& SpawnTransform, Material* Material, const std::string& ModelName)
	: Entity(Name, SpawnTransform) {
	loadModel(ModelName);
	InitMesh();
	S_MaterialID = 0;
	S_Material = Material;

	if(Material == nullptr) {
		Material = EngineStatics::GetDefaultMaterial();
	}
}
StaticMesh::StaticMesh(TString Name, const Transform& SpawnTransform, const std::string& ModelName)
	: Entity(Name, SpawnTransform) {
	loadModel(ModelName);
	InitMesh();
	S_MaterialID = 0;
	S_Material = EngineStatics::GetDefaultMaterial();
}
StaticMesh::StaticMesh(TString Name, const Transform& SpawnTransform, Vertex* Verticies, unsigned int NumVertecies, unsigned int* Indicies, unsigned int NumIndicides)
	: Entity(Name, SpawnTransform) {
	for (int i = 0; i < NumVertecies; i++) {
		S_Positions.push_back(Verticies[i].GetPosition());
		S_TexCoords.push_back(Verticies[i].GetTexCoord());
		S_Normals.push_back(Verticies[i].GetNormal());
	}

	for (int i = 0; i < NumIndicides; i++) {
		S_Indicies.push_back(Indicies[i]);
	}

	InitMesh();
	S_Material = EngineStatics::GetDefaultMaterial();
}
StaticMesh::~StaticMesh() {
	glDeleteVertexArrays(1, &S_VertexArrayObject);
}
void StaticMesh::loadModel(const std::string& ModelName) {
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(ModelName, aiProcess_Triangulate | 
													  aiProcess_FlipUVs |
													  aiProcess_CalcTangentSpace |
													  aiProcess_GenSmoothNormals);

	if (!scene || scene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
		std::cerr << "Error: Failed to import model. " << import.GetErrorString() << std::endl;
		return;
	}
	processNode(scene->mRootNode, scene);
}
void StaticMesh::processNode(aiNode* Node, const aiScene* Scene) {
	//Handle Parents
	for (GLuint i = 0; i < Node->mNumMeshes; i++) {
		aiMesh* mesh = Scene->mMeshes[Node->mMeshes[i]];
		populateVertecies(mesh);
	}
	//Handle Children
	for (int i = 0; i < Node->mNumChildren; i++) {
		this->processNode(Node->mChildren[i], Scene);
	}
}

/**********************************************************************************************//**
 * @fn	void StaticMesh::populateVertecies(aiMesh* Mesh)
 *
 * @brief	Populate vertecies.
 *
 * @author	Amine
 * @date	2/20/2017
 *
 * @param [in,out]	Mesh	If non-null, the mesh.
 **************************************************************************************************/

void StaticMesh::populateVertecies(aiMesh* Mesh) {
	S_Positions.reserve(Mesh->mNumVertices);
	S_Normals.reserve(Mesh->mNumVertices);
	S_Tangents.reserve(Mesh->mNumVertices);
	S_TexCoords.reserve(Mesh->mNumVertices);
	S_Indicies.reserve(Mesh->mNumFaces);

	for (int i = 0; i < Mesh->mNumVertices; i++) {
		vec3 tempPos;
		tempPos.x = Mesh->mVertices[i].x;
		tempPos.y = Mesh->mVertices[i].y;
		tempPos.z = Mesh->mVertices[i].z;

		S_Positions.push_back(tempPos);
	}
	if (Mesh->HasNormals()) {
		for (int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			S_Positions.push_back(tempNormal);
		}
	}
	if (Mesh->HasNormals()) {
		for (int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempNormal;
			tempNormal.x = Mesh->mNormals[i].x;
			tempNormal.y = Mesh->mNormals[i].y;
			tempNormal.z = Mesh->mNormals[i].z;

			S_Normals.push_back(tempNormal);
		}
	}
	if (Mesh->HasTangentsAndBitangents()) {
		for (int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempTangent;
			tempTangent.x = Mesh->mTangents[i].x;
			tempTangent.y = Mesh->mTangents[i].y;
			tempTangent.z = Mesh->mTangents[i].z;

			S_Tangents.push_back(tempTangent);
		}
	}
	if (Mesh->HasTextureCoords(0)) {
		for (int i = 0; i < Mesh->mNumVertices; i++) {
			vec2 tempUV;
			tempUV.x = Mesh->mTextureCoords[0][i].x;
			tempUV.y = Mesh->mTextureCoords[0][i].y;

			S_TexCoords.push_back(tempUV);
		}
	}
	if (Mesh->HasFaces()) {
		for (int i = 0; i < Mesh->mNumFaces; i++) {
			aiFace tempface = Mesh->mFaces[i];
			for (int j = 0; j < tempface.mNumIndices; j++) {
				S_Indicies.push_back(tempface.mIndices[j]);
			}
		}
		//Prints out each triangle
		//for(int i=0; i<S_Indicies.size(); i+=3) {
		//	cout << "1: = " << S_Positions[i].x << " " << S_Positions[i].y << " " << S_Positions[i].z << " " << " 2: = " << S_Positions[i+1].x << " " << S_Positions[i+1].y << " " << S_Positions[i+1].z << " 3: = " << S_Positions[i+2].x << " " << S_Positions[i+2].y << " " << S_Positions[i+2].z << endl;
		//}
	}
	if (Mesh->HasVertexColors(0)) {
		for (int i = 0; i < Mesh->mNumVertices; i++) {
			vec3 tempColor;
			tempColor.x = Mesh->mColors[i]->r;
			tempColor.y = Mesh->mColors[i]->g;
			tempColor.z = Mesh->mColors[i]->b;

			S_VertexColors.push_back(tempColor);
		}
	}
}
void StaticMesh::InitMesh() {
	S_DrawCount = S_Positions.size();

	glGenVertexArrays(1, &S_VertexArrayObject);
	glBindVertexArray(S_VertexArrayObject);

	glGenBuffers(NUM_BUFFERS, S_VertexArrayBuffers);

	//Position
	if (!S_Positions.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[POSITION_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(S_Positions[0]) * S_Positions.size(), &S_Positions[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

		S_AABB = new FAABB(S_Positions);
	}

	//Texcoord
	if (!S_TexCoords.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[TEXCOORD_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(S_TexCoords[0]) *S_TexCoords.size(), &S_TexCoords[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//Normal
	if (!S_Normals.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[NORMAL_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(S_Normals[0]) * S_Normals.size(), &S_Normals[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}
	//Colors
	if (!S_VertexColors.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[COLORS_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(S_VertexColors[0]) *S_VertexColors.size(), &S_VertexColors[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//Tangents
	if (!S_Tangents.empty()) {
		glBindBuffer(GL_ARRAY_BUFFER, S_VertexArrayBuffers[TANGENT_VB]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(S_Tangents[0]) * S_Tangents.size(), &S_Tangents[0], GL_STATIC_DRAW);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, 0);
	}

	//Index
	if (!S_Indicies.empty()) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, S_VertexArrayBuffers[INDEX_VB]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(S_Indicies[0]) * S_Indicies.size(), &S_Indicies[0], GL_STATIC_DRAW);
	}

	glBindVertexArray(0);
}

/**********************************************************************************************//**
 * @fn	void StaticMesh::Draw(Shader& shader)
 *
 * @brief	Draws the given shader.
 *
 * @author	Amine
 * @date	2/20/2017
 *
 * @param [in,out]	shader	The shader.
 **************************************************************************************************/

void StaticMesh::Draw(Camera* Camera) { //Clean this up
	if (Camera == NULL) {	//Drawing for shadow pass
		glBindVertexArray(S_VertexArrayObject);
		glDrawElements(GL_TRIANGLES, S_DrawCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		return;
	}
	if (S_Material->GetShaderModel() != EShaderModel::TRANSLUCENT && Engine::GetInstance()->GetRenderingEngine()->GetRenderingStage() == ERenderingStage::GEOMETRY) {
		S_Material->BindMaterial(GetTransform(), Camera);
		glBindVertexArray(S_VertexArrayObject);
		glDrawElements(GL_TRIANGLES, S_DrawCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
	if (S_Material->GetShaderModel() == EShaderModel::TRANSLUCENT && Engine::GetInstance()->GetRenderingEngine()->GetRenderingStage() == ERenderingStage::TRANSLUCENCY) {
		S_Material->BindMaterial(GetTransform(), Camera);
		glBindVertexArray(S_VertexArrayObject);
		glDrawElements(GL_TRIANGLES, S_DrawCount, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}
}
