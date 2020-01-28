#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include "Engine/EngineObject.h"
#include "Rendering/Material.h"
#include "Rendering/Texture2D.h"
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>
#include "Core/DataStructures/DataStructures.h"
#include "Core/Assets/ISerializeableAsset.h"

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

	FSubMesh() {
		SubMeshMaterial = nullptr;
		VertexArrayObject = 0;
	}

	int32 GetVertexCount() {
		return Verticies.Count();
	}
	int32 GetBufferCount() {
		int count = 0;
		if (!Verticies.IsEmpty()) {
			count++;
		}
		if (!TexCoords.IsEmpty()) {
			count++;
		}
		if (!Normals.IsEmpty()) {
			count++;
		}
		if (!Tangents.IsEmpty()) {
			count++;
		}
		if (!VertexColors.IsEmpty()) {
			count++;
		}
		if (!Indices.IsEmpty()) {
			count++;
		}
		return count;
	}
};

class StaticMesh : public EngineObject, public ISerializeableAsset {

public:
	StaticMesh(const TString& Name);
	StaticMesh(const TString& Name, const TString& FilePath);
	virtual ~StaticMesh();

	/**
	 * Gets all the submeshes that make up this static mesh.
	 */
	SArray<FSubMesh*> GetSubMeshes();
	/**
	* Checks to see if the data for this static mesh has already been sent to the GPU.
	*/
	bool SentToGPU();
	/**
	* Generates the OpenGL buffers for all the submeshes.
	*/
	void GenerateGPUBuffers();

	bool SerializeToBuffer(ByteBuffer& Buffer) const override;
	bool DeserializeFromBuffer(const ByteBuffer& Buffer) override;
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
private:
	SArray<FSubMesh*> SubMeshes;
	bool bSentToGPU;
};

