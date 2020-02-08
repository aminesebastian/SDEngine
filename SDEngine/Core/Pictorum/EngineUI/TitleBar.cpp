#include "TitleBar.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"

TitleBar::TitleBar(const TString& Name) : PictorumWidget(Name) {
	CloseTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CloseIcon.sasset");
	MinimizeTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MinimizeIcon.sasset");
	RestoreTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/RestoreIcon.sasset");
	MaximizeTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MaximizeIcon.sasset");

	SolidWidget* bg = new SolidWidget("TitleBarBackground");
	bg->SetBackgroundColor(FColor(0.2f, 0.2f, 0.3f));
	AddChild(bg);

	HorizontalBoxWidget* buttonContainer = new HorizontalBoxWidget("ButtonContainer");
	AddChild(buttonContainer);

	TextWidget* windowTitle = new TextWidget("WindowTitle");
	windowTitle->SetText("SD Engine");
	windowTitle->SetTextColor(FColor(0.9f, 0.9f, 0.9f));
	windowTitle->SetFontSize(11);
	buttonContainer->AddChild(windowTitle)->SetFillAvilableSpace(1.0f);

	ImageWidget* minimizeButton = new ImageWidget("Minimize");
	minimizeButton->SetImage(MinimizeTexture);
	minimizeButton->SetSize(vec2(25.0f, 25.0f));
	buttonContainer->AddChild(minimizeButton);

	ImageWidget* maximizeButton = new ImageWidget("Maximize");
	maximizeButton->SetImage(MaximizeTexture);
	maximizeButton->SetSize(vec2(25.0f, 25.0f));
	buttonContainer->AddChild(maximizeButton);

	ImageWidget* closeButton = new ImageWidget("Close");
	closeButton->SetImage(CloseTexture);
	closeButton->SetSize(vec2(25.0f, 25.0f));
	buttonContainer->AddChild(closeButton);
}
TitleBar::~TitleBar() {

}
bool TitleBar::CanAddChild() const {
	return true;
}
void TitleBar::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	OutputGeometry.SetLocation(vec2(0.0f, CurrentRenderGeometry.GetRenderResolution().y - 25.0f));
	OutputGeometry.SetAllotedSpace(vec2(CurrentRenderGeometry.GetRenderResolution().x, 25.0f));
}