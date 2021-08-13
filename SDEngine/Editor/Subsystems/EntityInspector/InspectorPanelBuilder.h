#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Core/Reflection/Handles/ReflectionHandle.h"
#include "Core/Reflection/Reflection.h"
#include "Editor/UserInterface/CollapsingCategoryWidget.h"

class PictorumWidget;

class InspectorPanelBuilder {
public:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionHandle& Target);
	virtual ~InspectorPanelBuilder();

	const ReflectionHandle& GetTarget() const;
	const ITypeDescriptor* GetTypeDescriptor() const;
	void AddControlForProperty(const PropertyHandle& Property);

protected:
	InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionHandle& Target, InspectorPanelBuilder* OwningBuilder);

	virtual void AddControlForFloatProperty(const PropertyHandle& Property);
	virtual void AddControlForVector2DProperty(const PropertyHandle& Property);
	virtual void AddControlForVector3DProperty(const PropertyHandle& Property);
	virtual void AddControlForVector4DProperty(const PropertyHandle& Property);
	virtual void AddControlForBoolProperty(const PropertyHandle& Property);
	PictorumHorizontalBox* GetPropertyContainer(const PropertyHandle& Property);

	const bool IsNested() const;
private:
	InspectorPanelBuilder* OwningBuilder;
	PictorumVerticalBox* OwningContainer;
	PictorumVerticalBox* DefaultCategory;
	PictorumVerticalBox* CurrentCategoryContainer;

	SHashMap<TString, PictorumVerticalBox*> Categories;
	SArray<InspectorPanelBuilder*> ChildBuilders;
	const ReflectionHandle TargetWrapper;
	SArray<const FProperty*> AddedProperties;
	SHashMap<const FProperty*, PictorumHorizontalBox*> TotalPropertiesList;

	PictorumVerticalBox* GetCategoryWidgetContainer(const TString& Category);
	void PropertyUpdatedCallback(const FProperty* Property);
};
