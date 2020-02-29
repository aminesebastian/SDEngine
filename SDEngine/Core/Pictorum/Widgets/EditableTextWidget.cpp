#include "EditableTextWidget.h"
#include "Core/Utilities/Math/MathLibrary.h"


EditableTextWidget::EditableTextWidget(const TString& Name) : TextWidget(Name) {
	CursorStartIndex = 0;
	CursorEndIndex = 0;
	bCursorFlashOn = true;
	BlinkTime = 0.5f;
	CurrentBlinkTime = 0.0f;
	bFocusable = true;

	CursorDrawInstruction = new FBoxDrawInstruction();
	CursorDrawInstruction->Size.x = 1.0f;
	CursorDrawInstruction->BorderRadius = FBorderRadius(1.0f);
	CursorDrawInstruction->BackgroundColor = FColor(0.1f, 0.1f, 0.1f);
}
EditableTextWidget::~EditableTextWidget() {
	delete CursorDrawInstruction;
}

void EditableTextWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	CurrentBlinkTime += DeltaTime;
	if (CurrentBlinkTime > BlinkTime) {
		bCursorFlashOn = !bCursorFlashOn;
		CurrentBlinkTime = 0.0f;
	}
}
void EditableTextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	if (bCursorFlashOn && HasFocus()) {
		DrawCursor(Geometry);
	}
	TextWidget::Draw(DeltaTime, Geometry);
}
void EditableTextWidget::OnKeyDown(SDL_Scancode KeyCode) {
	if (KeyCode == SDL_SCANCODE_RIGHT) {
		if (CursorStartIndex < GetText().length()) {
			CursorStartIndex++;
		}
	} else if (KeyCode == SDL_SCANCODE_LEFT) {
		if (CursorStartIndex > 0) {
			CursorStartIndex--;
		}
	} else if (KeyCode == SDL_SCANCODE_BACKSPACE) {
		if (CursorStartIndex > 0) {
			CursorStartIndex--;
			TString currentText = GetText();
			currentText = currentText.erase(CursorStartIndex, 1);
			SetText(currentText);
		}
	} else {
		char newCharacter = 0;
		if (KeyCode >= SDL_SCANCODE_A && KeyCode <= SDL_SCANCODE_Z) {
			newCharacter = KeyCode + 61;
		} else if (KeyCode >= SDL_SCANCODE_1 && KeyCode <= SDL_SCANCODE_9) {
			newCharacter = KeyCode + 19;
		} else if (KeyCode == SDL_SCANCODE_0) {
			newCharacter = '0';
		}

		if (newCharacter != 0) {
			TString currentText = GetText();
			TString newString = "";
			newString.push_back(newCharacter);
			currentText = currentText.insert(CursorStartIndex, newString);
			SetText(currentText);
			CursorStartIndex++;
		}
	}
}
void EditableTextWidget::OnRecievedFocus() {
	bCursorFlashOn = true;
	CurrentBlinkTime = 0.0f;
}
void EditableTextWidget::OnFocusLost() {
	bCursorFlashOn = false;
}
void EditableTextWidget::DrawCursor(const FRenderGeometry& Geometry) {
	CursorDrawInstruction->Location = ((Renderer->GetCursorLocationForCharacterIndex(CursorStartIndex) + 1.0f) / 2.0f) * Geometry.GetRenderResolution();
	CursorDrawInstruction->Location.y = Geometry.GetLocation().y;
	CursorDrawInstruction->Size.y = Geometry.GetAllotedSpace().y;

	DrawBox(Geometry, *CursorDrawInstruction);
}