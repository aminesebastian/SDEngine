#include "EditableTextWidget.h"
#include "Core/Utilities/Math/MathLibrary.h"


EditableTextWidget::EditableTextWidget(const TString& Name) : TextWidget(Name) {
	CursorStartIndex = 0;
	CursorStartLeftSide = true;
	CursorEndIndex = 0;
	bCursorFlashOn = true;
	BlinkTime = 0.5f;
	CurrentBlinkTime = 0.0f;
	bFocusable = true;
	InputFormatter = nullptr;

	CursorDrawInstruction = new FBoxDrawInstruction();
	CursorDrawInstruction->Size.x = 1.0f;
	CursorDrawInstruction->BorderRadius = FBorderRadius(0.5f);
	CursorDrawInstruction->BackgroundColor = FColor(1.0f);
}
EditableTextWidget::~EditableTextWidget() {
	delete CursorDrawInstruction;
}
void EditableTextWidget::SetInputFormatter(ITextInputFormatter* TextFormatter) {
	InputFormatter = TextFormatter;
}
void EditableTextWidget::SetPlaceholderText(const TString& PlaceholderText) {
	this->PlaceholderText = PlaceholderText;
	if (GetText().empty()) {
		SwapToPlaceholderText();
	}
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
	// Capture the character index at the mouse location.
	int32 index;
	Renderer->GetCharacterIndexAtMouseLocation(MousePosition, LastRenderedGeometry.GetRenderResolution(), index, CursorStartLeftSide);

	// Only update the cursor start index if the index is greater than 0.
	if (index >= 0) {
		CursorStartIndex = MathLibrary::Min((int32)GetText().length(), index);
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
	} else if (KeyCode == SDL_SCANCODE_DELETE) {
		if (CursorStartIndex < GetText().length() - 1) {
			TString currentText = GetText();
			currentText = currentText.erase(CursorStartIndex + 1, 1);
			SetText(currentText);
		}
	} else if (KeyCode == SDL_SCANCODE_RETURN) {
		SubmitInput();
	}
	if (GetText().empty()) {
		SwapToPlaceholderText();
	}
}
void EditableTextWidget::OnTextInput(const TString& Text) {
	TString currentText = GetText();
	currentText = currentText.insert(CursorStartIndex, Text);
	SetText(currentText);
	CursorStartIndex++;
	OnTextChangedDelegate.Broadcast(this, Text);
}
void EditableTextWidget::OnRecievedFocus() {
	// We can assume that when this widget gains focus, it is in a valid state. Therefore, we
	// capture the text.
	PreviousValidText = GetText();
	bCursorFlashOn = true;
	CurrentBlinkTime = 0.0f;
}
void EditableTextWidget::OnFocusLost() {
	bCursorFlashOn = false;
	SubmitInput();
}
void EditableTextWidget::SubmitInput() {
	if (InputFormatter) {
		TString originalText = GetText();
		if (InputFormatter->CheckTextValidity(GetText())) {
			SetText(InputFormatter->FormatText(GetText()));
			OnTextSubmitted(GetText());
			OnTextSubmittedDelegate.Broadcast(this, GetText());
			PreviousValidText = GetText();
			SD_ENGINE_DEBUG("Text formatted & submitted for textbox: {0} with content: {1}.", GetObjectName(), GetText());
		} else {
			SetText(PreviousValidText);
			SD_ENGINE_DEBUG("Text formatting failed for textbox: {0} with content: {1}. Submitted event not raised and content reverted.", GetObjectName(), originalText);
		}
	} else {
		OnTextSubmitted(GetText());
		OnTextSubmittedDelegate.Broadcast(this, GetText());
		PreviousValidText = GetText();
		SD_ENGINE_DEBUG("Text submitted for textbox: {0} with content: {1}.", GetObjectName(), GetText());
	}
}
void EditableTextWidget::DrawCursor(const FRenderGeometry& Geometry) {
	CursorDrawInstruction->Location = Renderer->GetCursorLocationForCharacterIndex(CursorStartIndex, CursorStartLeftSide) * Geometry.GetRenderResolution();
	CursorDrawInstruction->Location.y = Geometry.GetLocation().y;
	CursorDrawInstruction->Size.y = Geometry.GetAllotedSpace().y;

	DrawBox(Geometry, *CursorDrawInstruction);
}
void EditableTextWidget::SwapToPlaceholderText() {
	if (!PlaceholderText.empty()) {
		SetText(PlaceholderText);
		SetTextColor(FColor(0.4f, 0.4f, 0.4f));
	}
}
void EditableTextWidget::OnTextSet(const TString& Text) {
	if (Text.empty()) {
		SwapToPlaceholderText();
	}
}
void EditableTextWidget::OnTextSubmitted(const TString& SubmittedText) {}