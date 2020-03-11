#include "InspectorPanelBuilder.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Pictorum/Widgets/CheckboxWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/Math/Transform.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelManager.h"
#include "Editor/Subsystems/EntityInspector/InspectorPanelGenerator.h"
#include "Editor/UserInterface/Inspector/TypeInspectors/FloatInspectorWidget.h"
#include "Editor/UserInterface/Inspector/TypeInspectors/VectorInspectorWidget.h"


InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, const ReflectionWrapper& Target, InspectorPanelBuilder* Parent) : TargetWrapper(Target) {
	OwningContainer = Owner;
	ParentBuilder   = Parent;
	DefaultCategory = nullptr;
	CurrentCategoryContainer = nullptr;
}
void InspectorPanelBuilder::AddControlForProperty(const FProperty* Property) {
	// Skip if null.
	if (&Property == nullptr) {
		return;
	}
	// Skip if duplicate.
	if (AddedProperties.Contains(Property)) {
		return;
	}

	// Update the current category.
	CurrentCategoryContainer = GetCategoryWidgetContainer(Property->GetMetadata().GetCategory());

	// Add the separator.
	if (CurrentCategoryContainer->GetChildCount() > 0) {
		AssignNewToChildLocal(CurrentCategoryContainer, SeparatorWidget, sep, "Separator");
		sep->SetSize(0.0f, 5.0f);
	}

	// Raise the appropriate method for the property (if it is a simple data GetType(), raise the simple data GetType() method, otherwise raise the property builder.
	const TString& typeName = Property->GetType()->Name;
	if (typeName == "Vector4D") {
		AddControlForVector4DProperty(Property);
	} else if (typeName == "Vector3D") {
		AddControlForVector3DProperty(Property);
	} else if (typeName == "Vector2D") {
		AddControlForVector2DProperty(Property);
	} else if (typeName == "bool") {
		AddControlForBoolProperty(Property);
	} else if (typeName == "float") {
		AddControlForFloatProperty(Property);
	} else {
		IInspectorPanelGenerator* generator = InspectorPanelManager::Get()->GetGenerator(Property->GetType());
		ReflectionWrapper newWrapper(TargetWrapper.GetPropertyValue<void>(Property), Property->GetType());
		InspectorPanelBuilder propertyBuilder(CurrentCategoryContainer, newWrapper, this);
		generator->GenerateInspector(propertyBuilder);
	}

	// Add property as registered.
	AddedProperties.Add(Property);
}
void InspectorPanelBuilder::AddControlForVector4DProperty(const FProperty* Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	labels.Add("W");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(TargetWrapper, Property);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector3DProperty(const FProperty* Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(TargetWrapper, Property);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector2DProperty(const FProperty* Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(TargetWrapper, Property);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForBoolProperty(const FProperty* Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, CheckboxWidget, checkbox, "CheckboxWidget");
	checkbox->Bind(TargetWrapper.GetPropertyValue<bool>(Property));
}
void InspectorPanelBuilder::AddControlForFloatProperty(const FProperty* Property) {
	PictorumHorizontalBox* hBox = GetPropertyContainer(Property);

	AssignNewToChildLocal(hBox, FloatInspectorWidget, floatInspector, "FloatWidget");
	floatInspector->SetTarget(TargetWrapper, Property);
	floatInspector->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
PictorumVerticalBox* InspectorPanelBuilder::GetCategoryWidgetContainer(const TString& Category) {
	if (Category == "Default") {
		if (!DefaultCategory) {
			// If this is a nested class, simply allocate a vertical box and add default properties to that.
			// If this is a top level class, make a 'Default' collapsible category.
			if (IsNested()) {
				AssignNewToChild(OwningContainer, PictorumVerticalBox, DefaultCategory, "DefaultCategory");
				Categories.emplace(Category, DefaultCategory);
			} else {
				AssignNewToChildLocal(OwningContainer, CollapsingCategoryWidget, defaultCat, "DefaultCategory");
				defaultCat->SetCategoryLabel(Category);
				Categories.emplace(Category, defaultCat->GetContainer());
				DefaultCategory = defaultCat->GetContainer();
			}
		}
		return DefaultCategory;
	}
	if (Categories.count(Category) == 0) {
		AssignNewToChildLocal(OwningContainer, CollapsingCategoryWidget, currentCat, Category);
		currentCat->SetCategoryLabel(Category);
		Categories.emplace(Category, currentCat->GetContainer());
	}
	return Categories.at(Category);
}
PictorumHorizontalBox* InspectorPanelBuilder::GetPropertyContainer(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "HBox" + Property->GetInspectorName());
	AssignNewToChildLocal(hBox, TextWidget, label, "Label" + Property->GetInspectorName());
	label->SetText(Property->GetInspectorName());
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	if (IsNested()) {
		ParentBuilder->TotalPropertiesList.emplace(Property, hBox);
	} else {
		TotalPropertiesList.emplace(Property, hBox);
	}

	return hBox;
}
const TypeDescriptor* InspectorPanelBuilder::GetTypeDescriptor() const {
	return TargetWrapper.GetType();
}
const ReflectionWrapper& InspectorPanelBuilder::GetTarget() const {
	return TargetWrapper;
}
const bool InspectorPanelBuilder::IsNested() const {
	return ParentBuilder != nullptr;
}