#pragma once
#include <GLEW/glew.h>
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Assets/ISerializeableAsset.h"

class Shader;

class Texture2D : public ISerializeableAsset {

public:
	/*Wrap Behavior: GL_REPEAT or GL_CLAMP. Filter Behavior: GL_LINEAR or GL_NEAREST*/
	Texture2D(const TString& FileName,  int32 ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR);
	Texture2D(int Width, int Height);
	virtual ~Texture2D();

	void Bind(TString Name, Shader* Shader, uint32 Unit);

	GLuint& GetTexture() { return Texture; }

	bool SerializeToBuffer(ByteBuffer& Buffer) const override;
	bool DeserializeFromBuffer(const ByteBuffer& Buffer) override;

protected:
	virtual void LoadTexture();
	virtual void SendTextureDataToGPU();

private:
	bool bSentToGPU;
	GLuint Texture;
	TString FileName;

	int32 Width;
	int32 Height;
	int32 NumComponents;
	int32 ExpectedComponents;
	GLint WrapBehaviour;
	GLfloat FilterBehaviour;
	SArray<char> TextureData;
};

