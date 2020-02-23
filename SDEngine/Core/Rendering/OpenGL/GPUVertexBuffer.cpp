#include "GPUVertexBuffer.h"
#include "Core/Utilities/Logger.h"

GPUVertexBuffer::GPUVertexBuffer(const TString& Name, const EGPUBufferType& BufferType, const EGPUBufferUsage& BufferUsage, const EGPUBufferDataType& DataType, const uint8& Stride, const bool& Normalized)
	: Name(Name), BufferType(EGPUBufferType::ArrayBuffer), BufferUsage(EGPUBufferUsage::StaticDraw), DataType(DataType), Stride(Stride), bNormalized(Normalized) {
	Clear();
}

GPUVertexBuffer::~GPUVertexBuffer() {
	if (bSentToGPU) {
		glDeleteBuffers(1, &VertexBufferPointer);
	}

	Clear();
}
void GPUVertexBuffer::Clear() {
	if (bSentToGPU) {
		glDeleteBuffers(1, &VertexBufferPointer);
	}

	VertexBufferPointer = 0;
	bSentToGPU = false;
	CurrentBufferSize = 0;
	DataPointer = nullptr;
	SizeOfData = 0;
	DataLength = 0;
	ComponentCount = 0;
}
const GLuint GPUVertexBuffer::Generate() {
	if (DataLength == 0) {
		SD_ENGINE_ERROR("Attempted to generate GPU buffer: {0} with empty data.", Name);
		return 0;
	}

	glGenBuffers(1, &VertexBufferPointer);
	glBindBuffer(RenderDataTypeUtilities::GetGLBufferType(BufferType), VertexBufferPointer);
	glBufferData(RenderDataTypeUtilities::GetGLBufferType(BufferType), SizeOfData * DataLength * 10, nullptr, RenderDataTypeUtilities::GetGLBufferUsage(BufferUsage));
	glBufferSubData(RenderDataTypeUtilities::GetGLBufferType(BufferType), 0, SizeOfData * DataLength, DataPointer);
	CurrentBufferSize = DataLength * 10;
	bSentToGPU = true;

	return VertexBufferPointer;
}


const TString& GPUVertexBuffer::GetName() {
	return Name;
}
const EGPUBufferType& GPUVertexBuffer::GetBufferType() {
	return BufferType;
}
const EGPUBufferUsage& GPUVertexBuffer::GetBufferUsage() {
	return BufferUsage;
}
const GLuint& GPUVertexBuffer::GetVertexBufferPointer() {
	return VertexBufferPointer;
}
const uint32& GPUVertexBuffer::GetCurrentBufferSize() {
	return CurrentBufferSize;
}
const EGPUBufferDataType& GPUVertexBuffer::GetDataType() {
	return DataType;
}
const bool& GPUVertexBuffer::GetNormalized() {
	return bNormalized;
}
const uint8& GPUVertexBuffer::GetStride() {
	return Stride;
}
const uint8& GPUVertexBuffer::GetComponentCount() {
	return ComponentCount;
}