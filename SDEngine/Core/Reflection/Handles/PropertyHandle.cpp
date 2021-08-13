#include "Core/Reflection/Handles/ReflectionHandle.h"
#include "Core/Reflection/Utilities/ReflectionHelpers.h"
#include "PropertyHandle.h"

PropertyHandle::PropertyHandle() {
	bIsValid     = false;
	Property     = nullptr;
	Target       = nullptr;
}
PropertyHandle::~PropertyHandle() {

}
PropertyHandle::PropertyHandle(const FProperty* PropertyIn, const ReflectionHandle& TargetHandle) : PropertyHandle() {
	if (PropertyIn && TargetHandle.IsValid()) {
		bIsValid = true;
		Property = PropertyIn;
		Target = TargetHandle.Get<void>();
	} 
}
const TString PropertyHandle::GetPropertyName() const {
	return Property->GetName();
}
const TString PropertyHandle::GetInspectorName() const {
	return Property->GetInspectorName();
}
const FPropertyMetadata& PropertyHandle::GetMetadata() const {
	return Property->GetMetadata();
}
const bool& PropertyHandle::IsValid() const {
	return bIsValid;
}
const ITypeDescriptor* PropertyHandle::GetType() const {
	if (Property) {
		return Property->GetType();
	}
	return nullptr;
}
const FProperty* PropertyHandle::GetRawProperty() const {
	return Property;
}