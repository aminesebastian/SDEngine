#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Reflection/ReflectionHelpers.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(const FProperty* TargetProperty, const void* TargetObject) = 0;

	template<typename T>
	const T* GetInspectedObject() const {
		return Cast<T>(InspectionObject);
	}
	void SetTarget(const FProperty* TargetProperty, const TypeDescriptor* InspectionTargetType, const void* TargetObject);
	const FProperty* GetInspectedProperty() const;

protected:
	const FProperty* InspectedProperty;
	const void* InspectionObject;
	const TypeDescriptor* InspectionType;

private:
	bool bInitialized;
};

