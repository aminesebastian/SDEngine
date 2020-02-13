#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "Core/Rendering/OpenGL/GPUVertexBuffer.h"


class GPUVertexBufferArray {
public:
	GPUVertexBufferArray();
	~GPUVertexBufferArray();

	const GLuint& Generate();
private:
	SArray<SPair<void*, uint32>> Data;
	SArray<GPUVertexBuffer*> VertexArrayBuffers;
	SArray<GLuint> VertexArrayBufferPointers;
	GLuint VertexArrayObject;
	bool bSentToGPU;
};

