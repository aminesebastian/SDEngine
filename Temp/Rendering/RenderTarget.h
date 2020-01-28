#pragma once
#include <GLEW/glew.h>
#include <stdio.h>
#include <cassert>
#include "Rendering/Texture2D.h"
#include "Core/DataStructures/DataStructures.h"
#include "Utilities/TypeDefenitions.h"

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
	RenderTarget(vec2 RenderTargetResolution);
	virtual ~RenderTarget();

	/**
	 * Binds the render target as the active draw target.
	 */
	void BindForWriting();
	/**
	 * Binds the render target for reading.
	 */
	void BindForReading();

	/**
	 * Adds a texture attachment to this render target.
	 * 
	 * @param Texture The texture to add.
	 */
	void AddTextureIndex(FRenderTargetTextureEntry* Texture);
	/**
	 * Adds a depth and stencil attachment to the render target.
	 */
	void AddDepthStencilBuffer();

	/**
	 * Binds all the textures from this buffer to a shader.
	 * 
	 * @param Shader The Shader to bind to.
	 * @param Offset The offset to indicate which slot this texture should be bound to in the shader.
	 * @param bIncludeDepth If true, the depth texture will also be bounds.
	 */
	void BindTextures(Shader* Shader, int32 Offset = 0, bool bIncludeDepth = false);
	/**
	 * Binds a texture from this buffer to a shader.
	 * 
	 * @param Shader The shader to bind to.
	 * @param Index The index of the texture within this buffer.
	 * @param Offset The offset to indicate which slot this texture should be bound to in the shader.
	 * @param Name The name this texture should have in the shader.
	 */
	void BindTexture(Shader* Shader, int32 Index = 0, int32 Offset = 0, TString Name = "");
	/**
	* Binds the depth stencil texture to a shader.
	* 
	* @param Shader The shader to bind to.
	* @param Offset The offset to indicate which slot this texture should be bound to in the shader.
	* @param Name The name this texture should have in the shader.
	*/
	void BindDepthStencilTexture(Shader* Shader, int32 Offset = 0, TString Name = "DepthStencil");

	/**
	 * Finalizes the render target and generates all requested textures.
	 * Any textures added after this call is made will not be bound to the render target.
	 * 
	 * @param Resolution The resolution of this render target.
	 */
	bool FinalizeRenderTarget();

	GLuint& GetTexture(int32 i);
	TString GetTextureName(int32 i);
	int32 GetAttachedTextureCount();
	vec2 GetRenderTargetResolution();
protected:
	GLuint FrameBuffer;
	GLuint DepthStencilBuffer;
	GLuint TextureAttachments[8];

	bool bGenerateDepthBuffer;

	SArray<FRenderTargetTextureEntry*> TextureEntries;
	int32 TextureCount;

	vec2 Resolution;
};

