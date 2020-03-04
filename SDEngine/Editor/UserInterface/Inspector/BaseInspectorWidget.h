#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Reflection/ReflectionHelpers.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(const FProperty& TargetProperty, void* TargetObject) = 0;

	template<typename T>
	T* GetInternalTargetProperty() {
		return ReflectionHelpers::GetProperty<T>(InspectedProperty, InspectionObject);
	}
	template<typename T>
	const T* GetInspectedObject() const {
		return Cast<T>(InspectionObject);
	}
	void SetTarget(const FProperty& TargetProperty, TypeDescriptor* InspectionTargetType, void* TargetObject);
	const FProperty& GetInspectedProperty() const;

private:
	FProperty InspectedProperty;
	void* InspectionObject;
	TypeDescriptor* InspectionType;
	bool bInitialized;
};

