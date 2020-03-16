#pragma once
#include "Core/Pictorum/Widgets/TextWidget.h"

struct FTextCursor {
	int32 CharacterIndex;
	bool bRightSide;

	FTextCursor() {
		CharacterIndex = 0;
		bRightSide = false;
	}
	bool operator==(const FTextCursor& Other) const {
		if (CharacterIndex == Other.CharacterIndex) {
			if (bRightSide == Other.bRightSide) {
				return true;
			}
		}
		return false;
	}
};

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
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnTextInput(const TString& Text) override;
	virtual void OnRecievedFocus() override;
	virtual void OnFocusLost() override;
	virtual void OnTextSet(const TString& Text) override;

	virtual void OnTextSubmitted(const TString& SubmittedText);
	virtual void SubmitInput();

	void DrawCursor(const FRenderGeometry& Geometry);
	const int32 GetCharacterIndexOfCursor(const int32& CursorIndex) const;
	const void MoveCursorRight(const int32& CursorIndex);
	const void MoveCursorLeft(const int32& CursorIndex);
	const void MoveCursorUp(const int32& CursorIndex);
	const void MoveCursorDown(const int32& CursorIndex);
	const Vector2D GetCursorRelativePosition(const int32& CursorIndex) const;
	const int32 GetCursorHeight() const;
	const int32 AddCursor();
	const FTextCursor& GetCursorAtIndex(const int32& CursorIndex) const;
	const void AddTextToRightOfCursor(const int32& CursorIndex, const TString& Text);

	void DebugCursorState(const int32& CursorIndex);
private:
	TString PlaceholderText;
	FBoxDrawInstruction* CursorDrawInstruction;
	ITextInputFormatter* InputFormatter;

	float BlinkTime;
	float CurrentBlinkTime;
	bool bCursorFlashOn;
	TString PreviousValidText;

	SArray<FTextCursor> Cursors;

	void SwapToPlaceholderText();
};

