#pragma once
#include <GLEW/glew.h>
#include "Utilities/TypeDefenitions.h"
#include "Core/Assets/ISerializeableAsset.h"

class Shader;

class Texture2D : public ISerializeableAsset {

public:
	/*Wrap Behavior: GL_REPEAT or GL_CLAMP. Filter Behavior: GL_LINEAR or GL_NEAREST*/
	Texture2D(const TString& FileName,  unsigned int ExpectedComponent = 4, GLint WrapBehaviour = GL_REPEAT, GLfloat FilterBehaviour = GL_LINEAR);
	Texture2D(int Width, int Height);
	virtual ~Texture2D();

	void Bind(TString Name, Shader* Shader, uint32 Unit);

	GLuint& GetTexture() { return S_Texture; }

	bool SerializeToBuffer(ByteBuffer& Buffer) const{
		return true;
	}
	bool DeserializeFromBuffer(const ByteBuffer& Buffer) {
		return true;
	}
private:
	GLuint S_Texture;
	TString S_FileName;
};

