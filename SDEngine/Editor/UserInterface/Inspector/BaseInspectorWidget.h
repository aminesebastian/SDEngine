#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Reflection/Handles/PropertyHandle.h"
#include "Core/Reflection/Handles/ReflectionHandle.h"
#include "Core/Reflection/Utilities/ReflectionHelpers.h"
#include "Core/Engine/Delegates/Event.h"

class BaseInspectorWidget : public PictorumWidget {
public:
	BaseInspectorWidget(const TString& Name);
	virtual ~BaseInspectorWidget();
	virtual void OnTargetSet(const PropertyHandle& Property) = 0;

	template<typename T>
	const T* GetInspectedObject() const {
		return Cast<T>(InspectionObject);
	}
	void SetTarget(const PropertyHandle& Property);
	const bool& IsInitialized() const;

	Event<void(const FProperty*)> OnPropertyUpdatedDelegate;
protected:
	PropertyHandle InspectedProperty;

	void RaiseOnPropertyUpdatedDelegate();
private:
	bool bInitialized;
};

