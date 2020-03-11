#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Core/Reflection/ReflectionWrapper.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(const ReflectionWrapper& Wrapper, const FProperty* TargetProperty) = 0;

	template<typename T>
	const T* GetInspectedObject() const {
		return Cast<T>(InspectionObject);
	}
	void SetTarget(const ReflectionWrapper& Wrapper, const FProperty* TargetProperty);
	const bool& IsInitialized() const;
protected:
	const FProperty* InspectedProperty;
	ReflectionWrapper InspectionTarget;

private:
	bool bInitialized;
};

