#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/TypeResolver.h"

template <>
TypeDescriptor* GetPrimitiveDescriptor<TString>() {
	static TypeDescriptor_TString typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<const TString>() {
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

template <>
TypeDescriptor* GetPrimitiveDescriptor<Vector4D>() {
	static TypeDescriptor_Vector4D typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<Vector3D>() {
	static TypeDescriptor_Vector3D typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<Vector2D>() {
	static TypeDescriptor_Vector2D typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<bool>() {
	static TypeDescriptor_Bool typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<void>() {
	static TypeDescriptor_Void typeDesc;
	return &typeDesc;
}

/****************************************************************************/
/* Array Type Descriptors. There should be one of these for every one above */
/****************************************************************************/
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<TString>>() {
	static TypeDescriptor_SArray<TString> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<float>>() {
	static TypeDescriptor_SArray<float> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<int32>>() {
	static TypeDescriptor_SArray<int32> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<Vector4D>>() {
	static TypeDescriptor_SArray<Vector4D> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<Vector3D>>() {
	static TypeDescriptor_SArray<Vector3D> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<Vector2D>>() {
	static TypeDescriptor_SArray<Vector2D> typeDesc;
	return &typeDesc;
}
template <>
TypeDescriptor* GetPrimitiveDescriptor<SArray<bool>>() {
	static TypeDescriptor_SArray<bool> typeDesc;
	return &typeDesc;
}

