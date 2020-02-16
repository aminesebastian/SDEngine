#include "FloatingDetailsPanel.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/OverlayWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"

FloatingDetailsPanel::FloatingDetailsPanel(const TString& Name) : PictorumWidget(Name) {
	EntryCount = 3;
	Colors.Add(FColor(0.7f, 0.0f, 0.0f));
	Colors.Add(FColor(0.0f, 0.7f, 0.0f));
	Colors.Add(FColor(0.0f, 0.0f, 0.7f));
	Colors.Add(FColor(0.94f, 0.7f, 1.0f));

	Values.Add(103.0f);
	Values.Add(34.0f);
	Values.Add(342.43f);
	Values.Add(32.34f);

	MouseDownEntry = -1;
}
FloatingDetailsPanel::~FloatingDetailsPanel() {

}

void FloatingDetailsPanel::OnCreated() {
	GetOwningRenderer()->OnMouseUpAnywhereDelegate.Add<FloatingDetailsPanel, &FloatingDetailsPanel::MouseUpAnywhere>(this);
	GetOwningRenderer()->OnMouseMoveAnywhereDelegate.Add<FloatingDetailsPanel, &FloatingDetailsPanel::MouseMoveAnywhere>(this);

	Background = new SolidWidget("MainContainerBackground");
	Background->SetBackgroundColor(FColor(0.3f, 0.3f, 0.3f));
	Background->SetPadding(4.0f);
	Container = new HorizontalBoxWidget("MainContainer");

	for (uint8 i = 0; i < EntryCount; i++) {
		TextWidget* label = new TextWidget("Entry" + to_string(i) + "Label");
		label->SetText("X");
		label->SetFontSize(12);

		SolidWidget* labelBg = new SolidWidget("Entry" + to_string(i) + "LabelBG");
		labelBg->SetBackgroundColor(Colors[i]);
		labelBg->SetPadding(2.0f);
		labelBg->AddChild(label);

		TextWidget* value = new TextWidget("Entry" + to_string(i) + "Value");
		value->SetText(StringUtilities::ToStringWithPrecision(Values[i], 2));
		value->SetFontSize(12);

		SolidWidget* valueBg = new SolidWidget("Entry" + to_string(i) + "ValueBG");
		valueBg->OnMouseDownDelegate.Add<FloatingDetailsPanel, &FloatingDetailsPanel::ValueMouseDown>(this);
		valueBg->OnHoveredDelegate.Add<FloatingDetailsPanel, &FloatingDetailsPanel::ValueHovered>(this);
		valueBg->OnUnhoveredDelegate.Add<FloatingDetailsPanel, &FloatingDetailsPanel::ValueUnhovered>(this);
		valueBg->SetBackgroundColor(FColor(0.2f, 0.2f, 0.2f));
		valueBg->SetPadding(2.0f);
		valueBg->SetVisibility(EPictorumVisibilityState::VISIBLE);
		valueBg->AddChild(value);

		Container->AddChild(labelBg);
		Container->AddChild(valueBg);

		if (i < EntryCount - 1) {
			SeparatorWidget* separator = new SeparatorWidget("Entry" + to_string(i) + "Separator");
			separator->SetSize(10.0f, 0.0f);
			Container->AddChild(separator);
		}

		ValueBackgroundWidgets.Add(valueBg);
		ValueWidgets.Add(value);
	}
	Background->AddChild(Container);
	AddChild(Background);
}

void FloatingDetailsPanel::ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	for (int i = 0; i < ValueBackgroundWidgets.Count(); i++) {
		if (ValueBackgroundWidgets[i] == Widget) {
			MouseDownEntry = i;
			EventIn.CaptureMouse();
			break;
		}
	}
}
void FloatingDetailsPanel::ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::SizeHorizontal);
}
void FloatingDetailsPanel::ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn) {
	Engine::GetInputSubsystem()->SetMouseCursorStyle(EMouseCursorStyle::Arrow);
}
void FloatingDetailsPanel::MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta) {
	if (MouseDownEntry >= 0) {
		Values[MouseDownEntry] += Delta.x / 10.0f;
		ValueWidgets[MouseDownEntry]->SetText(StringUtilities::ToStringWithPrecision(Values[MouseDownEntry], 2));
	}
}
void FloatingDetailsPanel::MouseUpAnywhere(const vec2& MouseLocation) {
	MouseDownEntry = -1;
}