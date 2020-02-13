#pragma once
#include <GLEW/glew.h>

enum EDrawType {
	SCENE_RENDER,
	EDITOR_OUTLINE_RENDER,
	SHADOW_MAP_RENDER
};
enum class EGPUBufferType : uint8 {
	ArrayBuffer,
	ElementBuffer
};
enum class EGPUBufferDataType : uint8 {
	Float,
	HalfPrecision,
	DoublePrecision
};
enum class EGPUBufferUsage : uint8 {
	StreamDraw,
	StreamRead,
	StreamCopy,
	StaticDraw,
	StaticRead,
	StaticCopy,
	DynamicDraw,
	DynamicRead,
	DynamicCopy
};

class RenderDataTypeUtilities {
public:
	static GLuint GetGLBufferDataType(const EGPUBufferDataType& DataType) {
		switch (DataType) {
			case EGPUBufferDataType::Float: return GL_FLOAT;
			case EGPUBufferDataType::HalfPrecision: return GL_HALF_FLOAT;
			case EGPUBufferDataType::DoublePrecision: return GL_DOUBLE;
		}
		return 0;
	}
	static GLuint GetGLBufferType(const EGPUBufferType& BufferType) {
		switch (BufferType) {
			case EGPUBufferType::ArrayBuffer: return GL_ARRAY_BUFFER;
			case EGPUBufferType::ElementBuffer: return GL_ELEMENT_ARRAY_BUFFER;
		}
		return 0;
	}
	static GLuint GetGLBufferUsage(const EGPUBufferUsage& Usage) {
		switch (Usage) {
			case EGPUBufferUsage::StreamDraw: return GL_STREAM_DRAW;
			case EGPUBufferUsage::StreamRead: return GL_STREAM_READ;
			case EGPUBufferUsage::StreamCopy: return GL_STREAM_COPY;
			case EGPUBufferUsage::StaticDraw: return GL_STATIC_DRAW;
			case EGPUBufferUsage::StaticRead: return GL_STATIC_READ;
			case EGPUBufferUsage::StaticCopy: return GL_STATIC_COPY;
			case EGPUBufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
			case EGPUBufferUsage::DynamicRead: return GL_DYNAMIC_READ;
			case EGPUBufferUsage::DynamicCopy: return GL_DYNAMIC_COPY;
		}
		return 0;
	}
	static uint8 GetGLDataTypeSize(const EGPUBufferDataType& DataType) {
		switch (DataType) {
			case EGPUBufferDataType::Float: return 4;
			case EGPUBufferDataType::HalfPrecision: return 2;
			case EGPUBufferDataType::DoublePrecision: return 8;
		}
		return 1;
	}
};
