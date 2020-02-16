#include "FloatEditWidget.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"

FloatEditWidget::FloatEditWidget(const TString& Name) : PictorumWidget(Name) {
	Colors.Add(FColor(0.7f, 0.0f, 0.0f));
	Colors.Add(FColor(0.0f, 0.7f, 0.0f));
	Colors.Add(FColor(0.0f, 0.0f, 0.7f));
	Colors.Add(FColor(0.94f, 0.7f, 1.0f));

	Values    = (float*)malloc(4 * sizeof(float));
	Values[0] = 103.0f;
	Values[1] = 34.0f;
	Values[2] = 342.43f;
	Values[3] = 32.34f;

	MouseDownEntry = -1;
}
FloatEditWidget::~FloatEditWidget() {

}

void FloatEditWidget::OnCreated() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Add<FloatEditWidget, & FloatEditWidget::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Add<FloatEditWidget, & FloatEditWidget::MouseMoveAnywhere>(this);

	Container = new HorizontalBoxWidget("MainContainer");
	Container->SetPadding(4.0f);
	AddChild(Container);

	SetControlledValue(Values, 3);
}
void FloatEditWidget::SetControlledValue(float* ValuePointer, uint8 Count) {
	Values     = ValuePointer;
	ValueCount = Count;

	for (uint8 i = 0; i < Count; i++) {
		AddEntry(Values[i], i);
	}
}
void FloatEditWidget::AddEntry(const float& InitialValue, const int32& Index) {
	if (Index > 0) {
		SeparatorWidget* separator = new SeparatorWidget("Entry" + to_string(Index) + "Separator");
		separator->SetSize(10.0f, 0.0f);
		Container->AddChild(separator);
	}

	TextWidget* label = new TextWidget("Entry" + to_string(Index) + "Label");
	label->SetText("X");
	label->SetFontSize(11);

	SolidWidget* labelBg = new SolidWidget("Entry" + to_string(Index) + "LabelBG");
	labelBg->SetBackgroundColor(Colors[Index]);
	labelBg->SetPadding(2.0f);
	labelBg->AddChild(label);

	TextWidget* value = new TextWidget("Entry" + to_string(Index) + "Value");
	value->SetText(StringUtilities::ToStringWithPrecision(InitialValue, 2));
	value->SetFontSize(11);

	SolidWidget* valueBg = new SolidWidget("Entry" + to_string(Index) + "ValueBG");
	valueBg->OnMouseDownDelegate.Add<FloatEditWidget, & FloatEditWidget::ValueMouseDown>(this);
	valueBg->OnHoveredDelegate.Add<FloatEditWidget, & FloatEditWidget::ValueHovered>(this);
	valueBg->OnUnhoveredDelegate.Add<FloatEditWidget, & FloatEditWidget::ValueUnhovered>(this);
	valueBg->SetBackgroundColor(FColor(0.2f, 0.2f, 0.2f));
	valueBg->SetPadding(2.0f);
	valueBg->SetVisibility(EPictorumVisibilityState::VISIBLE);
	valueBg->AddChild(value);

	Container->AddChild(labelBg);
	Container->AddChild(valueBg);

	ValueBackgroundWidgets.Add(valueBg);
	ValueWidgets.Add(value);
}
void FloatEditWidget::ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	for (int i = 0; i < ValueBackgroundWidgets.Count(); i++) {
		if (ValueBackgroundWidgets[i] == Widget) {
			MouseDownEntry = i;
			EventIn.CaptureMouse();
			break;
		}
	}
}
void FloatEditWidget::ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeHorizontal);
}
void FloatEditWidget::ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
}
void FloatEditWidget::MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta) {
	if (MouseDownEntry >= 0) {
		Values[MouseDownEntry] += Delta.x / 10.0f;
		ValueWidgets[MouseDownEntry]->SetText(StringUtilities::ToStringWithPrecision(Values[MouseDownEntry], 2));
	}
}
void FloatEditWidget::MouseUpAnywhere(const vec2& MouseLocation) {
	MouseDownEntry = -1;
}