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


InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, const TypeDescriptor* CustomizationType, void* CustomizationTarget) {
	Parent             = Owner;
	Target             = CustomizationTarget;
	TargetType         = CustomizationType;
	CurrentCategory    = nullptr;
}
void InspectorPanelBuilder::AddControlForProperty(const FProperty& Property) {
	// Skip if duplicate.
	if (AddedProperties.Contains(Property)) {
		return;
	}
	CurrentCategory = GetCategoryWidget(Property.Category);
	if (CurrentCategory == nullptr) {
		CurrentCategory = GetCategoryWidget("Default");
	}

	if (AddedProperties.Count() > 0) {
		AssignNewToChildLocal(CurrentCategory->GetContainer(), SeparatorWidget, sep, "Separator");
		sep->SetSize(0.0f, 5.0f);
	}

	const TString& typeName = Property.Type->Name;
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
		IInspectorPanelGenerator* generator = InspectorPanelManager::Get()->GetGenerator(Property.Type);
		InspectorPanelBuilder propertyBuilder(CurrentCategory->GetContainer(), Property.Type, ReflectionHelpers::GetProperty<void>(Property, Target));
		generator->GenerateInspector(propertyBuilder);
	}
	AddedProperties.Add(Property);
}
void InspectorPanelBuilder::AddControlForVector4DProperty(const FProperty& Property) {
	AssignNewToChildLocal(CurrentCategory->GetContainer(), PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property.InspectorName);
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
	floatEditor->SetTarget(Property, Property.Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector3DProperty(const FProperty& Property) {
	AssignNewToChildLocal(CurrentCategory->GetContainer(), PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property.InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");

	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	labels.Add("Z");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(Property, Property.Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForVector2DProperty(const FProperty& Property) {
	AssignNewToChildLocal(CurrentCategory->GetContainer(), PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property.InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");

	SArray<TString> labels;
	labels.Add("X");
	labels.Add("Y");
	floatEditor->SetLabels(labels);
	floatEditor->SetTarget(Property, Property.Type, Target);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
void InspectorPanelBuilder::AddControlForTransformProperty(const FProperty& Property) {
	Transform* trans = ReflectionHelpers::GetProperty<Transform>(Property, Target);
	AddControlForVector3DProperty(*ReflectionHelpers::GetPropertyHandleFromStruct("Location", trans));
	AssignNewToChildLocal(CurrentCategory->GetContainer(), SeparatorWidget, sep1, "Separator");
	sep1->SetSize(0.0f, 5.0f);
	AddControlForVector3DProperty(*ReflectionHelpers::GetPropertyHandleFromStruct("Rotation", trans));
	AssignNewToChildLocal(CurrentCategory->GetContainer(), SeparatorWidget, sep2, "Separator");
	sep2->SetSize(0.0f, 5.0f);
	AddControlForVector3DProperty(*ReflectionHelpers::GetPropertyHandleFromStruct("Scale", trans));
}
void InspectorPanelBuilder::AddControlForBoolProperty(const FProperty& Property) {
	AssignNewToChildLocal(CurrentCategory->GetContainer(), PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property.InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	AssignNewToChildLocal(hBox, CheckboxWidget, checkbox, "CheckboxWidget");
	checkbox->Bind(ReflectionHelpers::GetProperty<bool>(Property, Target));
}
void InspectorPanelBuilder::AddControlForFloatProperty(const FProperty& Property) {
	AssignNewToChildLocal(CurrentCategory->GetContainer(), PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Property.InspectorName);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	AssignNewToChildLocal(hBox, FloatInspectorWidget, floatInspector, "FloatWidget");
	floatInspector->SetTarget(Property, Property.Type, Target);
	floatInspector->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}

const TypeDescriptor* InspectorPanelBuilder::GetTypeDescriptor() const {
	return TargetType;
}
CollapsingCategoryWidget* InspectorPanelBuilder::GetCategoryWidget(const TString& Category) {
	if (Categories.find(Category) == Categories.end()) {
		AssignNewToChildLocal(Parent, CollapsingCategoryWidget, currentCat, Category);
		currentCat->SetCategoryLabel(Category);
		Categories.emplace(Category, currentCat);
	}
	return Categories.at(Category);
}