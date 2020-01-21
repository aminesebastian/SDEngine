#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <string>
#include <vector>
#include "Entity.h"
#include "Material.h"
#include "Texture2D.h"
#include <ASSIMP/Importer.hpp>
#include <ASSIMP/scene.h>
#include <ASSIMP/postprocess.h>

using namespace glm;


class Vertex {

public:
	Vertex(const vec3& Position, const vec2& TexCoord = vec2(0,0), const vec4& Color = vec4(1,1,1,1), const vec3& Normal = vec3(0, 0, 0)) {
		this->Position = Position;
		this->TexCoord = TexCoord;
		this->Color = Color;
		this->Normal = Normal;
	}

	inline vec3 GetPosition() { return Position; }
	inline vec2 GetTexCoord() { return TexCoord; }
	inline vec3 GetNormal() { return Normal; }
	inline vec3 GetTangent() { return Tangent; }
	inline vec4 GetColor() { return Color; }

private:
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
	vec3 Tangent;
	vec4 Color;
};

class StaticMesh : public Entity {

public:
	StaticMesh(TString Name, const Transform& SpawnTransform, Vertex* Verticies, unsigned int NumVertecies, unsigned int* Indicies, unsigned int NumIndicides);
	StaticMesh(TString Name, const Transform& SpawnTransform, Material* Material, const std::string& ModelName);
	StaticMesh(TString Name, const Transform& SpawnTransform, const std::string& ModelName);

	virtual ~StaticMesh();

	virtual void Draw(Camera* Camera) override;
	virtual bool TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance);

	void loadModel(const std::string& ModelName);
	void processNode(aiNode* Node, const aiScene* Scene);
	void populateVertecies(aiMesh* Mesh);

	void RegisterTexture(Texture2D* texture) { Textures.push_back(texture); }

	void SetCustomColor(vec3 Color) { S_CustomAlbedo = Color; S_UseCustomColor = true; }
	void SetCustomRoughness(float Roughness) { S_CustomRoughness = Roughness; S_UseCustomRoughness = true; }
	void SetMaterialID(int ID) { S_MaterialID = ID; }


	void SetMaterial(Material* NewMaterial) { S_Material = NewMaterial; }
	Material* GetMaterial() { return S_Material; }

	SArray<vec3> GetVerticies() { return S_Verticies; }
private:
	void operator=(const StaticMesh& other) {}
	void InitMesh();

	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		TANGENT_VB,
		INDEX_VB,
		COLORS_VB,
		NUM_BUFFERS
	};

	GLuint S_VertexArrayObject;
	GLuint S_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int S_DrawCount;
	std::vector<Texture2D*> Textures;

	std::vector<vec3> S_Verticies;
	std::vector<vec2> S_TexCoords;
	std::vector<vec3> S_Normals;
	std::vector<vec3> S_Tangents;
	std::vector<vec3> S_VertexColors;
	std::vector<uint32> S_Indicies;

	Material* S_Material;
	bool S_UseCustomColor = false;
	bool S_UseCustomRoughness = false;
	bool S_UseCustomMetalness = false;

	vec3 S_CustomAlbedo;
	float S_CustomRoughness;
	float S_CustomMetalness;
	int	S_MaterialID;
};

