#pragma once
#include "Core/Pictorum/Widgets/TextWidget.h"

class EditableTextWidget : public TextWidget {
public:
	EditableTextWidget(const TString& Name);
	virtual ~EditableTextWidget();
	void SetInputFormatter(ITextInputFormatter* TextFormatter);
	void SetPlaceholderText(const TString& PlaceholderText);

	Event<void(PictorumWidget*, const TString&)> OnTextSubmittedDelegate;
	Event<void(PictorumWidget*, const TString&)> OnTextChangedDelegate;
protected:
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	void DrawCursor(const FRenderGeometry& Geometry);
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnTextInput(const TString& Text) override;
	virtual void OnRecievedFocus() override;
	virtual void OnFocusLost() override;
	virtual void OnTextSet(const TString& Text) override;

	virtual void OnTextSubmitted(const TString& SubmittedText);
	virtual void SubmitInput();
private:
	TString PlaceholderText;
	FBoxDrawInstruction* CursorDrawInstruction;
	ITextInputFormatter* InputFormatter;
	int32 CursorStartIndex;
	bool CursorStartLeftSide;
	int32 CursorEndIndex;
	float BlinkTime;
	float CurrentBlinkTime;
	bool bCursorFlashOn;
	TString PreviousValidText;

	void SwapToPlaceholderText();
};

