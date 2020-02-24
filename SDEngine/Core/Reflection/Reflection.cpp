#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/TypeResolver.h"

template <>
TypeDescriptor* GetPrimitiveDescriptor<TString>() {
	static TypeDescriptor_TString typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<float>() {
	static TypeDescriptor_Float typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<int32>() {
	static TypeDescriptor_Int32 typeDesc;
	return &typeDesc;
}