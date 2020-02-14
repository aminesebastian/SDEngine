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
	void Clear();

	template<typename T>
	void SetBufferData(const uint8& BufferIndex, const SArray<T>& Data) {
		VertexArrayBuffers[BufferIndex]->SetData(Data, 0, Data.Count());
	}
	template<typename T>
	void SetBufferData(const uint8& BufferIndex, const SArray<T>& Data, const int32& DataElements) {
		VertexArrayBuffers[BufferIndex]->SetData(Data, 0 , DataElements);
	}
	template<typename T>
	void SetBufferData(const uint8& BufferIndex, const SArray<T>& Data, const uint32& StartIndex, const int32& DataElements) {
		VertexArrayBuffers[BufferIndex]->SetData(Data, StartIndex, DataElements);
	}


	const GLuint& GetVertexArrayPointer();
	const GLuint& GetVertexBufferPointer(const uint8& BufferIndex);
private:
	const GLuint& Generate();

	SArray<GPUVertexBuffer*> VertexArrayBuffers;
	SArray<GLuint> VertexArrayBufferPointers;
	GLuint VertexArrayObjectPointer;
	bool bSentToGPU;
};
