#include "VectorInspectorWidget.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/Containers/PictorumHorizontalBox.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Editor/UserInterface/EngineUIStyle.h"

VectorInspectorWidget::VectorInspectorWidget(const TString& Name) : BaseInspectorWidget(Name) {
	Colors.Add(FColor(0.7f, 0.0f, 0.0f));
	Colors.Add(FColor(0.0f, 0.7f, 0.0f));
	Colors.Add(FColor(0.0f, 0.0f, 0.7f));
	Colors.Add(FColor(0.94f, 0.7f, 1.0f));

	Container = nullptr;

	MouseDownEntry = -1;
	ContentPadding = 3.0f;
}
VectorInspectorWidget::~VectorInspectorWidget() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Remove<VectorInspectorWidget, & VectorInspectorWidget::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Remove<VectorInspectorWidget, & VectorInspectorWidget::MouseMoveAnywhere>(this);
}
void VectorInspectorWidget::OnCreated() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Add<VectorInspectorWidget, & VectorInspectorWidget::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Add<VectorInspectorWidget, & VectorInspectorWidget::MouseMoveAnywhere>(this);

	Container = new PictorumHorizontalBox("MainContainer");
	Container->SetPadding(4.0f);
	AddChild(Container);
}
void VectorInspectorWidget::SetLabels(SArray<TString> LabelsIn) {
	Labels = LabelsIn;
}
void VectorInspectorWidget::OnTargetSet(const FProperty* TargetProperty, void* TargetObject) {
	Values.Clear();
	if (TargetProperty->Type == TypeResolver<Vector2D>::Get()) {
		for (uint8 i = 0; i < 2; i++) {
			AddEntry(Labels[i], &(*ReflectionHelpers::GetProperty<Vector2D>(TargetProperty, TargetObject))[i], i);
		}
	} else if (TargetProperty->Type == TypeResolver<Vector3D>::Get()) {
		for (uint8 i = 0; i < 3; i++) {
			AddEntry(Labels[i], &(*ReflectionHelpers::GetProperty<Vector3D>(TargetProperty, TargetObject))[i], i);
		}
	} else if (TargetProperty->Type == TypeResolver<Vector4D>::Get()) {
		for (uint8 i = 0; i < 4; i++) {
			AddEntry(Labels[i], &(*ReflectionHelpers::GetProperty<Vector4D>(TargetProperty, TargetObject))[i], i);
		}
	}
}
void VectorInspectorWidget::AddEntry(const TString& Label, float* InitialValue, const int32& Index) {
	Values.Add(InitialValue);
	if (Index > 0) {
		SeparatorWidget* separator = new SeparatorWidget("Entry" + to_string(Index) + "Separator");
		separator->SetSize(3.0f, 0.0f);
		Container->AddChild(separator);
	}

	AssignNewToChildLocal(Container, PictorumHorizontalBox, hBox, "ValueContainer"+to_string(Index));
	hBox->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(1.0f);

	TextWidget* label = new TextWidget("Entry" + to_string(Index) + "Label");
	label->SetText(Label);
	label->SetFontSize(10);
	label->SetTextColor(EngineUIStyles::LIGHT_TEXT_COLOR);

	SolidWidget* labelBg = new SolidWidget("Entry" + to_string(Index) + "LabelBG");
	labelBg->SetBackgroundColor(Colors[Index]);
	labelBg->SetPadding(ContentPadding);
	labelBg->AddChild(label);
	labelBg->SetBorderRadius(5.0f, 0.0f, 5.0f, 0.0f);

	EditableTextWidget* value = new EditableTextWidget("Entry" + to_string(Index) + "Value");
	value->SetText(StringUtilities::ToStringWithPrecision(*InitialValue, 2));
	value->SetFontSize(9);
	value->SetTextColor(EngineUIStyles::LIGHT_TEXT_COLOR);
	value->SetFontWeight(EFontWeight::Bold);
	value->OnMouseDownDelegate.Add<VectorInspectorWidget, &VectorInspectorWidget::ValueMouseDown>(this);
	value->OnMouseMoveDelegate.Add<VectorInspectorWidget, &VectorInspectorWidget::ValueMouseMove>(this);

	SolidWidget* valueBg = new SolidWidget("Entry" + to_string(Index) + "ValueBG");
	valueBg->OnHoveredDelegate.Add<VectorInspectorWidget, &VectorInspectorWidget::ValueHovered>(this);
	valueBg->OnUnhoveredDelegate.Add<VectorInspectorWidget, &VectorInspectorWidget::ValueUnhovered>(this);
	valueBg->SetBackgroundColor(EngineUIStyles::BACKGROUND_COLOR);
	valueBg->SetPadding(ContentPadding);
	valueBg->SetBorderRadius(0.0f, 5.0f, 0.0f, 5.0f);
	valueBg->SetVisibility(EPictorumVisibilityState::VISIBLE);
	valueBg->AddChild(value);

	hBox->AddChild(labelBg)->SetVerticalAlignment(EVerticalAlignment::STRETCH);
	hBox->AddChild(valueBg)->SetVerticalAlignment(EVerticalAlignment::STRETCH).SetFillAvilableSpace(1.0f);

	ValueBackgroundWidgets.Add(valueBg);
	ValueWidgets.Add(value);
}
void VectorInspectorWidget::ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	for (int i = 0; i < ValueBackgroundWidgets.Count(); i++) {
		if (ValueWidgets[i] == Widget) {
			MouseDownEntry = i;
			break;
		}
	}
}
void VectorInspectorWidget::ValueMouseMove(PictorumWidget* Widget, const Vector2D& MouseLocation, const Vector2D& Delta, FUserInterfaceEvent& EventIn) {
	if (Widget->WasClickedInside()) {
		EventIn.CaptureMouse();
	}
}
void VectorInspectorWidget::ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeHorizontal);
}
void VectorInspectorWidget::ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
}
void VectorInspectorWidget::MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta) {
	if (MouseDownEntry >= 0) {
		*Values[MouseDownEntry] += Delta.x / 10.0f;
		ValueWidgets[MouseDownEntry]->SetText(StringUtilities::ToStringWithPrecision(*Values[MouseDownEntry], 2));
	}
}
void VectorInspectorWidget::MouseUpAnywhere(const vec2& MouseLocation) {
	MouseDownEntry = -1;
}