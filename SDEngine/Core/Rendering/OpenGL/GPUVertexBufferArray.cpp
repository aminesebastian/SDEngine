#include "GPUVertexBufferArray.h"
#include "Core/Utilities/Logger.h"

GPUVertexBufferArray::GPUVertexBufferArray() {
	bSentToGPU = false;
	VertexArrayObjectPointer = 0;
}
GPUVertexBufferArray::~GPUVertexBufferArray() {
	Clear();

	// Delete the buffers.
	for (GPUVertexBuffer* buffer : VertexArrayBuffers) {
		delete buffer;
	}

	// Clear the array for good measure.
	VertexArrayBuffers.Clear();
}
void GPUVertexBufferArray::Bind() {
	if (!bSentToGPU) {
		SD_ENGINE_ERROR("Attempting to bind Vertex Array before sending to GPU.");
		return;
	}
	glBindVertexArray(VertexArrayObjectPointer);
}
void GPUVertexBufferArray::Unbind() {
	glBindVertexArray(0);
}
void GPUVertexBufferArray::DrawTriangleElements(const uint8& ElementsBufferIndex, const uint32& ElementCount) {
	Bind();
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GetVertexBufferPointer(ElementsBufferIndex));
	glDrawElements(GL_TRIANGLES, ElementCount, GL_UNSIGNED_INT, nullptr);
	Unbind();
}
void GPUVertexBufferArray::AddBuffer(const TString& Name, const EGPUBufferType& BufferType, const EGPUBufferUsage& BufferUsage, const EGPUBufferDataType& DataType, const uint8& Stride, const bool& Normalized) {
	// Create a new buffer and add it to the buffer array.
	GPUVertexBuffer* buffer = new GPUVertexBuffer(Name, BufferType, BufferUsage, DataType, Stride, Normalized);
	VertexArrayBuffers.Add(buffer);
}
void GPUVertexBufferArray::Update() {
	if (bSentToGPU) {
		// For each buffer, generate it and then register the attribute.
		for (uint8 i = 0; i < VertexArrayBuffers.Count(); i++) {
			GPUVertexBuffer* buffer = VertexArrayBuffers[i];
			if (buffer->GetBufferType() == EGPUBufferType::ArrayBuffer) {
				glEnableVertexAttribArray(i);
				glVertexAttribPointer(i, buffer->GetComponentCount(), RenderDataTypeUtilities::GetGLBufferDataType(buffer->GetDataType()), buffer->GetNormalized(), buffer->GetStride(), nullptr);
			}
		}
	} else {
		Generate();
	}
}
void GPUVertexBufferArray::Clear() {
	// Delete the vertex array.
	if (bSentToGPU) {
		glDeleteVertexArrays(1, &VertexArrayObjectPointer);
	}

	for (GPUVertexBuffer* buffer : VertexArrayBuffers) {
		buffer->Clear();
	}

	bSentToGPU = false;
}
const GLuint& GPUVertexBufferArray::GetVertexArrayPointer() {
	return VertexArrayObjectPointer;
}
const GLuint& GPUVertexBufferArray::GetVertexBufferPointer(const uint8& BufferIndex) {
	return VertexArrayBuffers[BufferIndex]->GetVertexBufferPointer();
}


const GLuint& GPUVertexBufferArray::Generate() {
	// Clear the vertex buffer pointers array.
	VertexArrayBufferPointers.Clear();

	// Generate and bind the vertex array.
	glGenVertexArrays(1, &VertexArrayObjectPointer);
	glBindVertexArray(VertexArrayObjectPointer);

	// For each buffer, generate it and then register the attribute.
	for (uint8 i = 0; i < VertexArrayBuffers.Count(); i++) {

		GPUVertexBuffer* buffer = VertexArrayBuffers[i];
		GLuint bufferPointer = buffer->Generate();

		VertexArrayBufferPointers.Add(bufferPointer);
		if (buffer->GetBufferType() == EGPUBufferType::ArrayBuffer) {
			glEnableVertexAttribArray(i);
			glVertexAttribPointer(i, buffer->GetComponentCount(), RenderDataTypeUtilities::GetGLBufferDataType(buffer->GetDataType()), buffer->GetNormalized(), buffer->GetStride(), nullptr);
		}
	}

	// Unbind the vertex array and mark it as having been sent to the GPU.
	glBindVertexArray(0);
	bSentToGPU = true;

	return VertexArrayObjectPointer;
}