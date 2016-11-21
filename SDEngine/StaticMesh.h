#pragma once
#include <GLM/glm.hpp>
#include <GLEW/glew.h>
#include <string>
#include "Shader.h"
#include "ObjLoader.h"
#include "Entity.h"
#include "Material.h"
#include "Texture2D.h"

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

private:
	vec3 Position;
	vec2 TexCoord;
	vec3 Normal;
	vec4 Color;
};

class StaticMesh : public Entity {

public:
	StaticMesh(const Transform& SpawnTransform, Vertex* Verticies, unsigned int NumVertecies, unsigned int* Indicies, unsigned int NumIndicides);
	StaticMesh(const Transform& SpawnTransform, const std::string& FileName);

	virtual ~StaticMesh();

	virtual void Draw(Shader shader) override;

	void RegisterTexture(Texture2D texture) { Textures.push_back(texture); }
private:
	void operator=(const StaticMesh& other) {}
	void InitMesh(const IndexedModel& model);

	enum {
		POSITION_VB,
		TEXCOORD_VB,
		NORMAL_VB,
		INDEX_VB,

		NUM_BUFFERS
	};

	GLuint S_VertexArrayObject;
	GLuint S_VertexArrayBuffers[NUM_BUFFERS];
	unsigned int S_DrawCount;
	std::vector<Texture2D> Textures;
};

