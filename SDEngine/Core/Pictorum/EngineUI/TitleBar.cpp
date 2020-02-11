#include "TitleBar.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/HorizontalBoxWidget.h"
#include "Core/Pictorum/Widgets/ImageWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Pictorum/Widgets/OverlayWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"

TitleBar::TitleBar(const TString& Name) : PictorumWidget(Name) {
	TitleBarHeight = 30.0f;

	CloseTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CloseIcon.sasset");
	MinimizeTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MinimizeIcon.sasset");
	RestoreTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/RestoreIcon.sasset");
	MaximizeTexture = Engine::GetInstance()->GetAssetManager()->GetAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/MaximizeIcon.sasset");
}
TitleBar::~TitleBar() {

}

void TitleBar::OnCreated() {
	SDL_SetWindowHitTest(GetOwningRenderer()->GetOwningWindow()->GetWindow(), &MouseHitTestCallback, this);

	/*
	* MasterContainer (LayoutWidget)				  -> Limits draw geometry to TitleBarHeight worth of pixels from the top.
	* OverlayContainer (OverlayWidget)				  -> Ensures all following geometry follow that constraint.
	*		TitleBarBackground (SolidWidget)		  -> Provides the background color for the title bar.
	*		ButtonContainer (HorizontalBoxWidget)	  -> Contains the window title text and the buttons.
	*				WindowTitle (TextWidget)		  -> The title of the window.
	*				MimizeButton (ImageWidget)		  -> The minimize button.
	*				MaximizeButton (ImageWidget)	  -> The maximize button.
	*				CloseButton (ImageWidget)		  -> The close button.
	*	 
	*/

	MasterContainer = new LayoutWidget("MasterContainer");
	MasterContainer->SetAnchor(EPictorumSide::BOTTOM, 1.0f);
	AddChild(MasterContainer);

	OverlayWidget* overlay = new OverlayWidget("OverlayContainer");
	MasterContainer->AddChild(overlay)->SetOffset(EPictorumSide::BOTTOM, -TitleBarHeight);

	SolidWidget* bg = new SolidWidget("TitleBarBackground");
	bg->SetBackgroundColor(FColor(0.2f, 0.2f, 0.3f));
	bMouseDownOnTitleBar = false;
	overlay->AddChild(bg);

	HorizontalBoxWidget* buttonContainer = new HorizontalBoxWidget("ButtonContainer");
	overlay->AddChild(buttonContainer);

	WindowTitleWidget = new TextWidget("WindowTitle");
	WindowTitleWidget->SetText("SD Engine");
	WindowTitleWidget->SetTextColor(FColor(0.9f, 0.9f, 0.9f));
	WindowTitleWidget->SetFontSize(11);
	buttonContainer->AddChild(WindowTitleWidget)->SetFillAvilableSpace(1.0f).SetVerticalAlignment(EVerticalAlignment::CENTER);

	WindowTitleWidget->GetParentSlot<HorizontalBoxSlot>()->GetPadding().SetLeft(10.0f).SetTop(1.0f);

	MinimizeButton = new ImageWidget("Minimize");
	MinimizeButton->SetImage(MinimizeTexture);
	MinimizeButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	MinimizeButton->OnMouseUpDelegate.Add<TitleBar, &TitleBar::OnMinimizeButtonClicked>(this);
	buttonContainer->AddChild(MinimizeButton);

	MaximizeButton = new ImageWidget("Maximize");
	MaximizeButton->SetImage(MaximizeTexture);
	MaximizeButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	MaximizeButton->OnMouseUpDelegate.Add<TitleBar, &TitleBar::OnMaximizeClicked>(this);
	buttonContainer->AddChild(MaximizeButton);

	CloseButton = new ImageWidget("Close");
	CloseButton->SetImage(CloseTexture);
	CloseButton->SetSize(vec2(TitleBarHeight, TitleBarHeight));
	CloseButton->OnMouseUpDelegate.Add<TitleBar, &TitleBar::OnCloseButtonClicked>(this);
	buttonContainer->AddChild(CloseButton);
}
void TitleBar::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	PictorumWidget::Tick(DeltaTime, Geometry);
}
const bool TitleBar::CanAddChild() const {
	return true;
}

void TitleBar::OnCloseButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent) {
	SDL_Event sdlevent;
	sdlevent.type = SDL_QUIT;
	SDL_PushEvent(&sdlevent);
}
void TitleBar::OnMinimizeButtonClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent) {
	SDL_MinimizeWindow(GetOwningRenderer()->GetOwningWindow()->GetWindow());
}
void TitleBar::OnMaximizeClicked(const vec2& MousePosition, FUserInterfaceEvent& UIEvent) {
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