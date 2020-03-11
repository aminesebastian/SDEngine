#include "FloatInspectorWidget.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/Containers/PictorumHorizontalBox.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"
#include "Editor/UserInterface/EngineUIStyle.h"

FloatInspectorWidget::FloatInspectorWidget(const TString& Name) : BaseInspectorWidget(Name) {
	ContentPadding = 3.0f;
	bMouseWasDownInside = false;
}
FloatInspectorWidget::~FloatInspectorWidget() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Remove<FloatInspectorWidget, & FloatInspectorWidget::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Remove<FloatInspectorWidget, & FloatInspectorWidget::MouseMoveAnywhere>(this);
}
void FloatInspectorWidget::OnCreated() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::MouseMoveAnywhere>(this);
}
void FloatInspectorWidget::OnTargetSet(const ReflectionWrapper& Wrapper, const FProperty* TargetProperty) {
	AssignNewToChild(this, SolidWidget, ValueBackgroundWidget, "ValueBg");
	ValueBackgroundWidget->OnHoveredDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::ValueHovered>(this);
	ValueBackgroundWidget->OnUnhoveredDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::ValueUnhovered>(this);
	ValueBackgroundWidget->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	ValueBackgroundWidget->SetPadding(ContentPadding);
	ValueBackgroundWidget->SetBorderRadius(0.0f, 5.0f, 0.0f, 5.0f);
	ValueBackgroundWidget->SetVisibility(EPictorumVisibilityState::VISIBLE);

	AssignNewToChild(ValueBackgroundWidget, EditableTextWidget, ValueWidget, "Value");
	ValueWidget->SetText(StringUtilities::ToStringWithPrecision(*Wrapper.GetPropertyValue<float>(TargetProperty), 2));
	ValueWidget->SetFontSize(9);
	ValueWidget->SetTextColor(EngineUIStyles::LIGHT_TEXT_COLOR);
	ValueWidget->SetFontWeight(EFontWeight::Bold);
	ValueWidget->SetInputFormatter(new DecimalInputTextFormatter(2));
	ValueWidget->OnMouseDownDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::ValueMouseDown>(this);
	ValueWidget->OnMouseMoveDelegate.Add<FloatInspectorWidget, & FloatInspectorWidget::ValueMouseMove>(this);
}
void FloatInspectorWidget::ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	bMouseWasDownInside = true;
}
void FloatInspectorWidget::ValueMouseMove(PictorumWidget* Widget, const Vector2D& MouseLocation, const Vector2D& Delta, FUserInterfaceEvent& EventIn) {
	if (Widget->WasClickedInside()) {
		EventIn.CaptureMouse();
	}
}
void FloatInspectorWidget::ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeHorizontal);
}
void FloatInspectorWidget::ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
}
void FloatInspectorWidget::MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta) {
	if (bMouseWasDownInside) {
		float value = *InspectionTarget.GetPropertyValue<float>(InspectedProperty) + Delta.x / 10.0f;
		InspectionTarget.SetPropertyValue(InspectedProperty, value);
		ValueWidget->SetText(StringUtilities::ToStringWithPrecision(*InspectionTarget.GetPropertyValue<float>(InspectedProperty), 2));
	}
}
void FloatInspectorWidget::MouseUpAnywhere(const vec2& MouseLocation) {
	bMouseWasDownInside = false;
}