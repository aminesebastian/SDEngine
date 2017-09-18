#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "Texture2D.h"
#include "FrontBufferObject.h"
#include "TypeDefenitions.h"

class Shader;

class FrontBufferObject {

public:
	FrontBufferObject();
	virtual ~FrontBufferObject();

	void BindForWriting();
	void BindForReading();

	void AddTextureIndex(TString TextureName);

	void BindTextures(Shader* Shader, bool bIncludeDepth = false);

	GLuint& GetTexture(int i) { assert(i < S_NumTextures);  return S_Textures[i]; }
	TString GetTextureName(int i) { return S_TextureNames[i]; }

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void SetReadBuffer(int TextureIndex);
	int GetNumTextures() { return S_NumTextures; }
protected:
	GLuint S_FBO;
	GLuint S_DepthBuffer;
	GLuint S_Textures[8];

	SArray<TString> S_TextureNames;
	int S_NumTextures;
};

