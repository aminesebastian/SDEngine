#include "TitleBar.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"

TitleBar::TitleBar(const TString& Name) : PictorumWidget(Name) {
	SolidWidget* bg = new SolidWidget("TitleBarBackground");
	bg->SetBackgroundColor(FColor(0.2f, 0.2f, 0.3f));
	AddChild(bg);

	HorizontalBoxWidget* buttonContainer = new HorizontalBoxWidget("ButtonContainer");
	AddChild(buttonContainer);

	TextWidget* windowTitle = new TextWidget("WindowTitle");
	windowTitle->SetText("SD Engine");
	windowTitle->SetTextColor(FColor(0.9f, 0.9f, 0.9f));
	windowTitle->SetFontSize(11);
	AddChild(windowTitle);

	SolidWidget* minimizeButton = new SolidWidget("Minimize");
	minimizeButton->SetBackgroundColor(FColor(0.0f, 0.9f, 0.0f));
	buttonContainer->AddChild(minimizeButton);

	SolidWidget* maximizeButton = new SolidWidget("Maximize");
	maximizeButton->SetBackgroundColor(FColor(0.0f, 0.0f, 0.9f));
	buttonContainer->AddChild(maximizeButton);

	SolidWidget* closeButton = new SolidWidget("Close");
	closeButton->SetBackgroundColor(FColor(0.9f, 0.0f, 0.0f));
	buttonContainer->AddChild(closeButton);
}
TitleBar::~TitleBar() {

}
bool TitleBar::CanAddChild() const {
	return true;
}
void TitleBar::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	if (ChildIndex == 0) {
		OutputGeometry.SetLocation(vec2(0.0f, CurrentRenderGeometry.GetRenderResolution().y - 25.0f));
		OutputGeometry.SetAllotedSpace(vec2(CurrentRenderGeometry.GetRenderResolution().x, 25.0f));
	} else if (ChildIndex == 1) {
		OutputGeometry.SetLocation(vec2(CurrentRenderGeometry.GetRenderResolution().x - 75.0f, CurrentRenderGeometry.GetRenderResolution().y - 25.0f));
		OutputGeometry.SetAllotedSpace(vec2(75.0f, 25.0f));
	} else if (ChildIndex == 2) {
		vec2 textSize = Cast<TextWidget>(Children[ChildIndex])->GetDesiredDrawSpace(CurrentRenderGeometry);
		OutputGeometry.SetLocation(vec2((CurrentRenderGeometry.GetRenderResolution().x / 2.0f) - textSize.x / 2.0f, CurrentRenderGeometry.GetRenderResolution().y - textSize.y  - 7));
		OutputGeometry.SetAllotedSpace(textSize);
	}
}