#include "TitleBar.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/EngineUI/EngineUIStyle.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Containers/PictorumHorizontalBox.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Core/Pictorum/Widgets/OverlayWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"

TitleBar::TitleBar(const TString& Name) : PictorumWidget(Name) {
	TitleBarHeight = 30.0f;

	CloseTexture = Engine::Get()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CloseIcon.sasset");
	MinimizeTexture = Engine::Get()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MinimizeIcon.sasset");
	RestoreTexture = Engine::Get()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/RestoreIcon.sasset");
	MaximizeTexture = Engine::Get()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MaximizeIcon.sasset");
}
TitleBar::~TitleBar() {

}

void TitleBar::OnCreated() {
	SDL_SetWindowHitTest(GetOwningRenderer()->GetOwningWindow()->GetWindow(), &MouseHitTestCallback, this);

	/*
	* TitleBarBackground (SolidWidget)				  -> Provides the background color for the title bar.
	*		ButtonContainer (HorizontalBoxWidget)	  -> Contains the window title text and the buttons.
	*				WindowTitle (TextWidget)		  -> The title of the window.
	*				MimizeButton (ImageWidget)		  -> The minimize button.
	*				MaximizeButton (ImageWidget)	  -> The maximize button.
	*				CloseButton (ImageWidget)		  -> The close button.
	*	 
	*/
	SolidWidget* bg = new SolidWidget("TitleBarBackground");
	AddChild(bg);
	bg->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	bMouseDownOnTitleBar = false;

	PictorumHorizontalBox* buttonContainer = new PictorumHorizontalBox("ButtonContainer");
	buttonContainer->SetPadding(0.0f, 0.0f, 0.0f, 14.0f);
	bg->AddChild(buttonContainer);

	WindowTitleWidget = new TextWidget("WindowTitle");
	WindowTitleWidget->SetText("SD Engine");
	WindowTitleWidget->SetTextColor(FColor(0.9f, 0.9f, 0.9f));
	WindowTitleWidget->SetFontSize(11);
	buttonContainer->AddChild(WindowTitleWidget)->SetFillAvilableSpace(1.0f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	MinimizeButton = new ImageWidget("Minimize");
	MinimizeButton->SetImage(MinimizeTexture);
	MinimizeButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	MinimizeButton->OnMouseUpDelegate.Add<TitleBar, &TitleBar::OnMinimizeButtonClicked>(this);
	buttonContainer->AddChild(MinimizeButton);

	MaximizeButton = new ImageWidget("Maximize");
	MaximizeButton->SetImage(MaximizeTexture);
	MaximizeButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	MaximizeButton->OnMouseUpDelegate.Add<TitleBar, & TitleBar::OnMaximizeClicked>(this);
	buttonContainer->AddChild(MaximizeButton);

	CloseButton = new ImageWidget("Close");
	CloseButton->SetImage(CloseTexture);
	CloseButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	CloseButton->OnMouseUpDelegate.Add<TitleBar, & TitleBar::OnCloseButtonClicked>(this);
	buttonContainer->AddChild(CloseButton);
}
void TitleBar::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	PictorumWidget::Tick(DeltaTime, Geometry);
}
const bool TitleBar::CanAddChild() const {
	return true;
}

void TitleBar::OnCloseButtonClicked(PictorumWidget* Widget, const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& UIEvent) {
	SDL_Event sdlevent;
	sdlevent.type = SDL_QUIT;
	SDL_PushEvent(&sdlevent);
}
void TitleBar::OnMinimizeButtonClicked(PictorumWidget* Widget, const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& UIEvent) {
	SDL_MinimizeWindow(GetOwningRenderer()->GetOwningWindow()->GetWindow());
}
void TitleBar::OnMaximizeClicked(PictorumWidget* Widget, const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& UIEvent) {
	uint32 flags = SDL_GetWindowFlags(GetOwningRenderer()->GetOwningWindow()->GetWindow());
	if (flags & SDL_WINDOW_MAXIMIZED) {
		SDL_RestoreWindow(GetOwningRenderer()->GetOwningWindow()->GetWindow());
		MaximizeButton->SetImage(MaximizeTexture);
	} else {
		SDL_MaximizeWindow(GetOwningRenderer()->GetOwningWindow()->GetWindow());
		MaximizeButton->SetImage(RestoreTexture);
	}
}

void TitleBar::OnWindowMaximized(const int32& WindowId, const FDisplayState& State) {
	MaximizeButton->SetImage(RestoreTexture);
}
void TitleBar::OnWindowRestored(const int32& WindowId, const FDisplayState& State) {
	MaximizeButton->SetImage(MaximizeTexture);
}

SDL_HitTestResult TitleBar::MouseHitTestCallback(SDL_Window* Window, const SDL_Point* MouseLocation, void* Data) {
	TitleBar* titleBar = (TitleBar*)Data;
	if (MouseLocation->y < titleBar->TitleBarHeight && MouseLocation->x < titleBar->GetOwningRenderer()->GetOwningWindow()->GetDimensions().x - (titleBar->TitleBarHeight * 3)) {
		return SDL_HITTEST_DRAGGABLE;
	}
	return SDL_HITTEST_NORMAL;
}