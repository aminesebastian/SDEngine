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
	CursorDrawInstruction->BorderRadius = FBorderRadius(0.5f);
	CursorDrawInstruction->BackgroundColor = FColor(1.0f);
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
void EditableTextWidget::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	int32 index = Renderer->GetCharacterIndexAtMouseLocation(MousePosition, LastRenderedGeometry.GetRenderResolution());
	if (index >= 0) {
		CursorStartIndex = MathLibrary::Min((int32)GetText().length(), index + 1);
		EventIn.Handled();
	}
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
	}
}
void EditableTextWidget::OnTextInput(const TString& Text) {
	TString currentText = GetText();
	currentText = currentText.insert(CursorStartIndex, Text);
	SetText(currentText);
	CursorStartIndex++;
}
void EditableTextWidget::OnRecievedFocus() {
	bCursorFlashOn = true;
	CurrentBlinkTime = 0.0f;
}
void EditableTextWidget::OnFocusLost() {
	bCursorFlashOn = false;
}
void EditableTextWidget::DrawCursor(const FRenderGeometry& Geometry) {
	CursorDrawInstruction->Location = Renderer->GetCursorLocationForCharacterIndex(CursorStartIndex) * Geometry.GetRenderResolution();
	CursorDrawInstruction->Location.y = Geometry.GetLocation().y;
	CursorDrawInstruction->Size.y = Geometry.GetAllotedSpace().y;

	DrawBox(Geometry, *CursorDrawInstruction);
}