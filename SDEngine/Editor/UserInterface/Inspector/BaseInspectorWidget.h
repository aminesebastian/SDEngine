#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Reflection/ReflectionHelpers.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(const FProperty& TargetProperty, EngineObject* TargetObject) = 0;

	template<typename T>
	T* GetInternalTargetProperty() {
		return ReflectionHelpers::GetProperty<T>(InspectedProperty, InspectionObject);
	}

	void SetTarget(const FProperty&  TargetProperty, EngineObject* TargetObject);
	const FProperty& GetInspectedProperty() const;
	const EngineObject* GetInspectedObject() const;
private:
	FProperty InspectedProperty;
	EngineObject* InspectionObject;
	bool bInitialized;
};

