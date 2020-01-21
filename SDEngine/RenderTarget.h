#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "Texture2D.h"
#include "TypeDefenitions.h"

class Shader;

struct FRenderTargetTextureEntry {
public:
	TString S_TextureName;
	GLint S_InternalFormat;
	GLenum S_DataType;
	GLenum S_Format;
	GLint S_MinMagFilter;
	GLint S_WrapType;
	bool bGenerateMipMaps;

	FRenderTargetTextureEntry(TString TextureName, GLint InternalFormat, GLint MinMagFilter, GLint WrapType, GLenum Format, GLenum DataType, bool GenerateMipMaps = false) {
		S_TextureName	 = TextureName;
		S_InternalFormat = InternalFormat;
		S_MinMagFilter	 = MinMagFilter;
		S_DataType		 = DataType;
		S_Format		 = Format;
		S_WrapType		 = WrapType;
		bGenerateMipMaps = GenerateMipMaps;
	}
};
class RenderTarget {

public:
	RenderTarget();
	virtual ~RenderTarget();

	void BindForWriting();
	void BindForReading();

	void AddTextureIndex(FRenderTargetTextureEntry* Texture);

	/**
	 * Binds all the textures from this buffer to a shader.
	 * 
	 * @param Shader The Shader to bind to.
	 * @param Offset The offset to indicate which slot this texture should be bound to in the shader.
	 * @param bIncludeDepth If true, the depth texture will also be bounds.
	 */
	void BindTextures(Shader* Shader, int Offset = 0, bool bIncludeDepth = false);
	/**
	 * Binds a texture from this buffer to a shader.
	 * 
	 * @param Index The index of the texture within this buffer.
	 * @param Offset The offset to indicate which slot this texture should be bound to in the shader.
	 * @param Name The name this texture should have in the shader.
	 */
	void BindTexture(Shader* Shader, int Index = 0, int Offset = 0, TString Name = "");

	GLuint& GetTexture(int i) { assert(i < S_TextureCount);  return S_Textures[i]; }
	TString GetTextureName(int i) { return S_TextureEntries[i]->S_TextureName; }

	bool Init(unsigned int WindowWidth, unsigned int WindowHeight);
	void SetReadBuffer(int TextureIndex);
	int GetTextureCount() { return S_TextureCount; }
	vec2 GetFBOSize() { return S_FBOSize; }
protected:
	GLuint S_FBO;
	GLuint S_DepthBuffer;
	GLuint S_Textures[8];

	bool bGenerateDepthTexture;

	SArray<FRenderTargetTextureEntry*> S_TextureEntries;
	int32 S_TextureCount;

	vec2 S_FBOSize;
};

