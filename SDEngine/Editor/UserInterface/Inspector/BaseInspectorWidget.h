#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(FProperty* TargetProperty, EngineObject* TargetObject) = 0;

	template<typename T>
	T* GetInternalTargetProperty() {
		return ReflectionHelpers::GetProperty<T>(InspectedProperty, InspectionObject)
	}

	void SetTarget(FProperty* TargetProperty, EngineObject* TargetObject);
	const FProperty* GetInspectedProperty() const;
	const EngineObject* GetInspectedObject() const;
private:
	FProperty* InspectedProperty;
	EngineObject* InspectionObject;
};

