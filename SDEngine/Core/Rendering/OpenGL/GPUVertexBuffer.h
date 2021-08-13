#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/Array.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include <GLEW/glew.h>

struct FGPUBufferAttribute {
	FGPUBufferAttribute() {
		DataType = EGPUBufferDataType::Float;
		Size = 3;
		Stride = 0;
		bNormalized = false;
	}

	EGPUBufferDataType DataType;
	uint8 Size;
	uint8 Stride;
	bool bNormalized;
};


class GPUVertexBuffer {
public:
	template<typename T>
	void SetData(const SArray<T>& Data, const uint32& StartIndex, const int32& DataElements) {
		if (!Data.IsEmpty()) {
			if (bSentToGPU) {
				glBindBuffer(RenderDataTypeUtilities::GetGLBufferType(BufferType), VertexBufferPointer);
				glBufferSubData(RenderDataTypeUtilities::GetGLBufferType(BufferType), StartIndex, SizeOfData * DataElements, DataPointer);
			} else {
				DataPointer = (void*)& Data[0];
				SizeOfData = sizeof(Data[0]);
				ComponentCount = SizeOfData / RenderDataTypeUtilities::GetGLDataTypeSize(DataType);
				DataLength = DataElements;
			}
		} else {
			DataPointer = nullptr;
			SizeOfData = 0;
			DataLength = 0;
			ComponentCount = 0;
		}
	}

	const TString& GetName();
	const EGPUBufferType& GetBufferType();
	const EGPUBufferUsage& GetBufferUsage();
	const GLuint& GetVertexBufferPointer();
	const uint32& GetCurrentBufferSize();
	const EGPUBufferDataType& GetDataType();
	const bool& GetNormalized();
	const uint8& GetStride();
	const uint8& GetComponentCount();
private:
	friend class GPUVertexBufferArray;

	GPUVertexBuffer(const TString& Name, const EGPUBufferType& BufferType, const EGPUBufferUsage& BufferUsage, const EGPUBufferDataType& DataType, const uint8& Stride = 0, const bool& Normalized = false);
	virtual ~GPUVertexBuffer();
	const GLuint Generate();
	void Clear();

	const TString Name;
	const EGPUBufferType BufferType;
	const EGPUBufferUsage BufferUsage;
	const EGPUBufferDataType DataType;
	const bool bNormalized;
	const uint8 Stride;

	GLuint VertexBufferPointer;

	bool bSentToGPU;
	uint32 CurrentBufferSize;

	void* DataPointer;
	uint8 SizeOfData;
	uint8 ComponentCount;
	uint32 DataLength;
};