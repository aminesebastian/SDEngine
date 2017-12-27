#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "Texture2D.h"
#include "TypeDefenitions.h"

class Shader;

struct FFBOTextureEntry {
public:
	TString S_TextureName;
	GLint S_InternalFormat;
	GLenum S_DataType;
	GLenum S_Format;
	GLint S_MinMagFilter;
	GLint S_WrapType;

	FFBOTextureEntry(TString TextureName, GLint InternalFormat, GLint MinMagFilter, GLint WrapType, GLenum Format, GLenum DataType) {
		S_TextureName = TextureName;
		S_InternalFormat = InternalFormat;
		S_MinMagFilter = MinMagFilter;
		S_DataType = DataType;
		S_Format = Format;
		S_WrapType = WrapType;
	}
};
class FrameBufferObject {

public:
	FrameBufferObject();
	virtual ~FrameBufferObject();

	void BindForWriting();
	void BindForReading();

	void AddTextureIndex(FFBOTextureEntry* Texture);

	void BindTextures(Shader* Shader, bool bIncludeDepth = false);

	GLuint& GetTexture(int i) { assert(i < S_NumTextures);  return S_Textures[i]; }
	TString GetTextureName(int i) { return S_TextureEntries[i]->S_TextureName; }

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void SetReadBuffer(int TextureIndex);
	int GetNumTextures() { return S_NumTextures; }
protected:
	GLuint S_FBO;
	GLuint S_DepthBuffer;
	GLuint S_Textures[8];

	bool bGenerateDepthTexture;

	SArray<FFBOTextureEntry*> S_TextureEntries;
	int S_NumTextures;
};

