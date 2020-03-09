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


InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, const TypeDescriptor* CustomizationType, void* CustomizationTarget, bool IsNested) {
	Parent             = Owner;
	Target             = CustomizationTarget;
	TargetType         = CustomizationType;
	bIsNested          = IsNested;
	DefaultCategory    = nullptr;
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

	// Raise the appropriate method for the property (if it is a simple data type, raise the simple data type method, otherwise raise the property builder.
	const TString& typeName = Property->Type->Name;
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
		IInspectorPanelGenerator* generator = InspectorPanelManager::Get()->GetGenerator(Property->Type);
		InspectorPanelBuilder propertyBuilder(CurrentCategoryContainer, Property->Type, ReflectionHelpers::GetProperty<void>(Property, Target), true);
		generator->GenerateInspector(propertyBuilder);
	}
	AddedProperties.Add(RemoveConst(Property));
}
void InspectorPanelBuilder::AddControlForVector4DProperty(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property->InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");

	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	labels.Add("W");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(Property, Property->Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector3DProperty(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property->InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");

	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(Property, Property->Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector2DProperty(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property->InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");

	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(Property, Property->Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForTransformProperty(const FProperty* Property) {
	FTransform* trans = ReflectionHelpers::GetProperty<FTransform>(Property, Target);
	AddControlForVector3DProperty(ReflectionHelpers::GetPropertyHandleFromStruct("Location", trans));
	AssignNewToChildLocal(CurrentCategoryContainer, SeparatorWidget, sep1, "Separator");
	sep1->SetSize(0.0f, 5.0f);
	AddControlForVector3DProperty(ReflectionHelpers::GetPropertyHandleFromStruct("Rotation", trans));
	AssignNewToChildLocal(CurrentCategoryContainer, SeparatorWidget, sep2, "Separator");
	sep2->SetSize(0.0f, 5.0f);
	AddControlForVector3DProperty(ReflectionHelpers::GetPropertyHandleFromStruct("Scale", trans));
}
void InspectorPanelBuilder::AddControlForBoolProperty(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property->InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	AssignNewToChildLocal(hBox, CheckboxWidget, checkbox, "CheckboxWidget");
	checkbox->Bind(ReflectionHelpers::GetProperty<bool>(Property, Target));
}
void InspectorPanelBuilder::AddControlForFloatProperty(const FProperty* Property) {
	AssignNewToChildLocal(CurrentCategoryContainer, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property->InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	AssignNewToChildLocal(hBox, FloatInspectorWidget, floatInspector, "FloatWidget");
	floatInspector->SetTarget(Property, Property->Type, Target);
	floatInspector->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}

const TypeDescriptor* InspectorPanelBuilder::GetTypeDescriptor() const {
	return TargetType;
}
PictorumVerticalBox* InspectorPanelBuilder::GetCategoryWidgetContainer(const TString& Category) {
	if (Category == "Default") {
		if (!DefaultCategory) {
			// If this is a nested class, simply allocate a vertical box and add default properties to that.
			// If this is a top level class, make a 'Default' collapsible category.
			if (bIsNested) {
				AssignNewToChild(Parent, PictorumVerticalBox, DefaultCategory, "DefaultCategory");
				Categories.emplace(Category, DefaultCategory);
			} else {
				AssignNewToChildLocal(Parent, CollapsingCategoryWidget, defaultCat, "DefaultCategory");
				defaultCat->SetCategoryLabel(Category);
				Categories.emplace(Category, defaultCat->GetContainer());
				DefaultCategory = defaultCat->GetContainer();
			}
		}
		return DefaultCategory;
	}
	if (Categories.count(Category) == 0) {
		AssignNewToChildLocal(Parent, CollapsingCategoryWidget, currentCat, Category);
		currentCat->SetCategoryLabel(Category);
		Categories.emplace(Category, currentCat->GetContainer());
	}
	return Categories.at(Category);
}