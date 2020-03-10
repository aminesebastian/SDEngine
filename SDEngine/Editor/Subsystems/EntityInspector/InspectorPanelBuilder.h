#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Reflection/ReflectionWrapper.h"
#include "Editor/UserInterface/CollapsingCategoryWidget.h"

class PictorumWidget;

class InspectorPanelBuilder {
public:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionWrapper* Target, bool IsNested = false);

	template<typename T>
	const T* GetTarget() const {
		return (T*)Target;
	}
	const TypeDescriptor* GetTypeDescriptor() const;
	void AddControlForProperty(const FProperty* Property);

protected:
	virtual void AddControlForFloatProperty(const FProperty* Property);
	virtual void AddControlForVector2DProperty(const FProperty* Property);
	virtual void AddControlForVector3DProperty(const FProperty* Property);
	virtual void AddControlForVector4DProperty(const FProperty* Property);
	virtual void AddControlForBoolProperty(const FProperty* Property);

private:
	PictorumVerticalBox* Parent;
	PictorumVerticalBox* DefaultCategory;
	PictorumVerticalBox* CurrentCategoryContainer;
	SHashMap<TString, PictorumVerticalBox*> Categories;

	const ReflectionWrapper* TargetWrapper;

	SArray<FProperty*> AddedProperties;

	bool bIsNested;

	PictorumVerticalBox* GetCategoryWidgetContainer(const TString& Category);
};
