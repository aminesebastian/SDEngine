#include "InspectorPanelBuilder.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Pictorum/Widgets/VectorInspectorWidget.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/Math/Transform.h"

InspectorPanelBuilder::InspectorPanelBuilder(PictorumVerticalBox* Owner, EngineObject* CustomizationTarget) {
	Parent = Owner;
	Target = CustomizationTarget;
	AddedPropertyCount = 0;
}
template<>
void InspectorPanelBuilder::AddControlForPropertyInternal(const TString& Name, Vector4D* Property) {
	AssignNewToChildLocal(Parent, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	floatEditor->SetControlledValue(&(*Property)[0], 4);
}
template<>
void InspectorPanelBuilder::AddControlForPropertyInternal(const TString& Name, Vector3D* Property) {
	AssignNewToChildLocal(Parent, PictorumHorizontalBox, hBox, "PropertyHBox");
	AssignNewToChildLocal(hBox, TextWidget, label, "PropertyLabel");
	label->SetText(Name);
	label->SetFontSize(10);
	label->SetFontWeight(EFontWeight::Bold);
	label->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f).SetVerticalAlignment(EVerticalAlignment::CENTER);
	AssignNewToChildLocal(hBox, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	floatEditor->SetControlledValue(&(*Property)[0], 3);
	floatEditor->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);
}
template<>
void InspectorPanelBuilder::AddControlForPropertyInternal(const TString& Name, Vector2D* Property) {
	AssignNewToChildLocal(Parent, VectorInspectorWidget, floatEditor, "VectorInspectorWidget");
	floatEditor->SetControlledValue(&(*Property)[0], 2);
}
template<>
void InspectorPanelBuilder::AddControlForPropertyInternal(const TString& Name, Transform* Property) {
	AddControlForPropertyInternal("Location", ReflectionHelpers::GetProperty<Vector3D, Transform>("Location", Property));
	AssignNewToChildLocal(Parent, SeparatorWidget, sep1, "Separator");
	sep1->SetSize(0.0f, 5.0f);
	AddControlForPropertyInternal("Rotation", ReflectionHelpers::GetProperty<Vector3D, Transform>("Rotation", Property));
	AssignNewToChildLocal(Parent, SeparatorWidget, sep2, "Separator");
	sep2->SetSize(0.0f, 5.0f);
	AddControlForPropertyInternal("Scale", ReflectionHelpers::GetProperty<Vector3D, Transform>("Scale", Property));
}
void InspectorPanelBuilder::AddControlForProperty(const FProperty& Property) {
	if (AddedPropertyCount > 0) {
		AssignNewToChildLocal(Parent, SeparatorWidget, sep, "Separator");
		sep->SetSize(0.0f, 5.0f);
	}
	const TString& typeName = Property.Type->Name;
	if (typeName == "Vector4D") {
		AddControlForPropertyInternal(Property.Name, ReflectionHelpers::GetProperty<Vector4D>(Property, Target));
	} else if (typeName == "Vector3D") {
		AddControlForPropertyInternal(Property.Name, ReflectionHelpers::GetProperty<Vector3D>(Property, Target));
	} else if (typeName == "Vector2D") {
		AddControlForPropertyInternal(Property.Name, ReflectionHelpers::GetProperty<Vector2D>(Property, Target));
	} else if (typeName == "Transform") {
		AddControlForPropertyInternal(Property.Name, ReflectionHelpers::GetProperty<Transform>(Property, Target));
	}
	AddedPropertyCount++;
}