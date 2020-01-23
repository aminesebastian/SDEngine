#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include "Engine/EngineObject.h"
#include "Rendering/Material.h"
#include "Rendering/Texture2D.h"
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

using namespace glm;

enum EVertexBufferIndex {
	POSITION_VB,
	TEXCOORD_VB,
	NORMAL_VB,
	TANGENT_VB,
	INDEX_VB,
	COLORS_VB,
	MAX_BUFFERS
};

struct FSubMesh {
	SArray<vec3> Verticies;
	SArray<vec2> TexCoords;
	SArray<vec3> Normals;
	SArray<vec3> Tangents;
	SArray<vec3> VertexColors;
	SArray<uint32> Indices;
	SArray<GLuint> VertexArrayBuffers;
	Material* SubMeshMaterial;

	GLuint VertexArrayObject;
	int32 GetVertexCount() {
		return (int32)Verticies.size();
	}
	int32 GetBufferCount() {
		int count = 0;
		if (!Verticies.empty()) {
			count++;
		}
		if (!TexCoords.empty()) {
			count++;
		}
		if (!Normals.empty()) {
			count++;
		}
		if (!Tangents.empty()) {
			count++;
		}
		if (!VertexColors.empty()) {
			count++;
		}
		if (!Indices.empty()) {
			count++;
		}
		return count;
	}
};

class StaticMesh : public EngineObject {

public:
	StaticMesh(const TString& Name, const TString& FilePath);
	virtual ~StaticMesh();

	/**
	 * Gets all the submeshes that make up this static mesh.
	 */
	SArray<FSubMesh*> GetSubMeshes();
protected:
	/**
	 * Begins loading model from file.
	 * 
	 * @param ModelName The filepath of the model to load.
	 */
	void LoadModel(const TString FilePath);
	/**
	 * Recursively traverses model tree, calling GenerateSubMesh for each node encountered.
	 * 
	 * @param Node The node of the submesh to parse.
	 * @param Scene The total scene of the model.
	 */
	void ProcessNode(aiNode* Node, const aiScene* Scene);
	/**
	 * Generates a submesh object based on the provided mesh.
	 * 
	 * @param Mesh The assimp mesh object containing all the data about the mesh.
	 * @return A generated submesh object containing all the information about the mesh.
	 */
	FSubMesh* GenerateSubMesh(aiMesh* Mesh);
	/**
	 * Generates the OpenGL buffers for all the submeshes.
	 */
	void GenerateGPUBuffers();

private:
	SArray<FSubMesh*> SubMeshes;
};

