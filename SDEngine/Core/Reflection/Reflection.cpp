#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/Runtime/TypeResolver.h"

template <>
ITypeDescriptor* GetPrimitiveDescriptor<TString>() {
	static TypeDescriptor_TString typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<const TString>() {
	static TypeDescriptor_TString typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<float>() {
	static TypeDescriptor_Float typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<int32>() {
	static TypeDescriptor_Int32 typeDesc;
	return &typeDesc;
}

template <>
ITypeDescriptor* GetPrimitiveDescriptor<Vector4D>() {
	static TypeDescriptor_Vector4D typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<Vector3D>() {
	static TypeDescriptor_Vector3D typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<Vector2D>() {
	static TypeDescriptor_Vector2D typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<bool>() {
	static TypeDescriptor_Bool typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<void>() {
	static TypeDescriptor_Void typeDesc;
	return &typeDesc;
}

/****************************************************************************/
/* Array Type Descriptors. There should be one of these for every one above */
/****************************************************************************/
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<TString>>() {
	static TypeDescriptor_SArray<TString> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<float>>() {
	static TypeDescriptor_SArray<float> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<int32>>() {
	static TypeDescriptor_SArray<int32> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<Vector4D>>() {
	static TypeDescriptor_SArray<Vector4D> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<Vector3D>>() {
	static TypeDescriptor_SArray<Vector3D> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<Vector2D>>() {
	static TypeDescriptor_SArray<Vector2D> typeDesc;
	return &typeDesc;
}
template <>
ITypeDescriptor* GetPrimitiveDescriptor<SArray<bool>>() {
	static TypeDescriptor_SArray<bool> typeDesc;
	return &typeDesc;
}

