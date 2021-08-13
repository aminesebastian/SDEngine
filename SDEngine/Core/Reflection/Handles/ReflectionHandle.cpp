#include "ReflectionHandle.h"
#include "Core/Reflection/Utilities/ReflectionHelpers.h"

ReferenceHandleIterator::ReferenceHandleIterator(const ReflectionHandle* Handle, int32 StartIndex) : Handle(Handle), Index(StartIndex) {
	Handle->GetProperties(Properties);
}
bool ReferenceHandleIterator::operator!= (const ReferenceHandleIterator& Other) const {
	return Index <= Properties.LastIndex() && Index != -1;
}
const PropertyHandle ReferenceHandleIterator::operator* () const {
	if (Index == -1 || Index > Properties.LastIndex()) {
		return PropertyHandle();
	}
	return Handle->GetPropertyHandle(Properties[Index]->GetName());
}
const ReferenceHandleIterator& ReferenceHandleIterator::operator++ () {
	Index++;
	return *this;
}

ReflectionHandle::ReflectionHandle() {
	Target = nullptr;
	TargetType = nullptr;
	bIsValid = false;
	bIsObject = false;
}
ReflectionHandle::ReflectionHandle(const EngineObject* Object) : ReflectionHandle() {
	if (Object) {
		bIsValid = true;
		bIsObject = true;
		Target = (const void*)Object;
		TargetType = Object->StaticDescriptor();
	}
}
ReflectionHandle::ReflectionHandle(const void* Struct, const ITypeDescriptor* StructType) : ReflectionHandle() {
	if (Struct) {
		bIsValid = true;
		bIsObject = false;
		Target = Struct;
		TargetType = StructType;
	}
}
const PropertyHandle ReflectionHandle::GetPropertyHandle(const TString& PropertyName) const {
	if (bIsObject) {
		return PropertyHandle(ReflectionHelpers::GetProperty(PropertyName, (EngineObject*)Target), *this);
	} else {
		return PropertyHandle(ReflectionHelpers::GetProperty(PropertyName, Target, Cast<TypeDescriptor_Struct>(TargetType)), *this);
	}
}
const bool& ReflectionHandle::IsObject() const {
	return bIsObject;
}
const ITypeDescriptor* ReflectionHandle::GetType() const {
	return TargetType;
}
const bool& ReflectionHandle::IsValid() const {
	return bIsValid;
}
void ReflectionHandle::GetProperties(SArray<const FProperty*>& Properties) const {
	if (bIsObject) {
		ReflectionHelpers::GetPropertiesOfClass(Properties, (EngineObject*)Target);
	} else {
		ITypeDescriptor* typeDesc = RemoveConst(TargetType);
		TypeDescriptor_Struct* classTypeDesc = Cast<TypeDescriptor_Struct>(typeDesc);
		for (const FProperty* prop : classTypeDesc->GetProperties()) {
			Properties.Add(prop);
		}
	}
}
