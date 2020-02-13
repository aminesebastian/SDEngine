#include "GPUVertexBufferArray.h"

GPUVertexBufferArray::GPUVertexBufferArray() {
	bSentToGPU = false;
	VertexArrayObject = 0;
}
GPUVertexBufferArray::~GPUVertexBufferArray() {

}
const GLuint& GPUVertexBufferArray::Generate() {
	VertexArrayBufferPointers.Clear();

	for (uint8 i = 0; i < VertexArrayBuffers.Count(); i++) {

		GPUVertexBuffer* buffer = VertexArrayBuffers[i];
		uint8 attribIndex       = i + 1;
		GLuint bufferPointer    = buffer->Generate();

		VertexArrayBufferPointers.Add(bufferPointer);
		if(buffer->GetBufferType() == EGPUBufferType::ArrayBuffer) {
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex, buffer->GetComponentCount(), RenderDataTypeUtilities::GetGLBufferDataType(buffer->GetDataType()), buffer->GetNormalized(), buffer->GetStride(), nullptr);
		}
	}

	bSentToGPU = true;
}