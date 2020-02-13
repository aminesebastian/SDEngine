#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "Core/Rendering/OpenGL/GPUVertexBuffer.h"


class GPUVertexBufferArray {
public:
	GPUVertexBufferArray();
	~GPUVertexBufferArray();

	void Bind();
	void Unbind();
	void Update();
	void DrawTriangleElements(const uint8& ElementsBufferIndex, const uint32& ElementCount);

	void AddBuffer(const TString& Name, const EGPUBufferType& BufferType, const EGPUBufferUsage& BufferUsage, const EGPUBufferDataType& DataType, const uint8& Stride = 0, const bool& Normalized = false);
	template<typename T>
	void SetBufferData(const uint8& BufferIndex, const SArray<T>& Data) {
		VertexArrayBuffers[BufferIndex]->SetData(Data);
	}
	void Clear();

	const GLuint& GetVertexArrayPointer();
	const GLuint& GetVertexBufferPointer(const uint8& BufferIndex);
private:
	const GLuint& Generate();

	SArray<GPUVertexBuffer*> VertexArrayBuffers;
	SArray<GLuint> VertexArrayBufferPointers;
	GLuint VertexArrayObjectPointer;
	bool bSentToGPU;
};

