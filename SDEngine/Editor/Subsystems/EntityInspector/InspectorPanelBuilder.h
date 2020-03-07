#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Editor/UserInterface/CollapsingCategoryWidget.h"
#include "Core/Reflection/Reflection.h"

class PictorumWidget;

class InspectorPanelBuilder {
public:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, const TypeDescriptor* CustomizationType, void* CustomizationTarget);

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
	virtual void AddControlForTransformProperty(const FProperty* Property);
private:
	PictorumVerticalBox* Parent;
	PictorumVerticalBox* DefaultCategory;
	PictorumVerticalBox* CurrentCategoryContainer;
	void* Target;
	const TypeDescriptor* TargetType;
	SHashMap<TString, CollapsingCategoryWidget*> Categories;
	SArray<FProperty*> AddedProperties;

	CollapsingCategoryWidget* GetCategoryWidget(const TString& Category);
};
