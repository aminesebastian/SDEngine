#include "EditableTextWidget.h"
#include "Core/Utilities/Math/MathLibrary.h"


EditableTextWidget::EditableTextWidget(const TString& Name) : TextWidget(Name) {
	bCursorFlashOn = true;
	BlinkTime = 0.5f;
	CurrentBlinkTime = 0.0f;
	bFocusable = true;
	InputFormatter = nullptr;

	CursorDrawInstruction = new FBoxDrawInstruction();
	CursorDrawInstruction->Size.x = 1.0f;
	CursorDrawInstruction->BorderRadius = FBorderRadius(0.5f);
	CursorDrawInstruction->BackgroundColor = FColor(1.0f);

	Renderer->AddCursor();
	Renderer->AddCursor();
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
	//// Capture the character index at the mouse location.
	//int32 index;
	//bool right;
	//Renderer->GetCharacterIndexAtMouseLocation(MousePosition, LastRenderedGeometry.GetRenderResolution(), index, right);

	//// Only update the cursor start index if the index is greater than 0.
	//if (index >= 0) {
	//	SetCursorLocation(index, index, index);
	//	EventIn.Handled();
	//}
}
void EditableTextWidget::OnKeyDown(SDL_Scancode KeyCode) {
	if (KeyCode == SDL_SCANCODE_RIGHT) {
		Renderer->MoveCursorRight(0);
	} else if (KeyCode == SDL_SCANCODE_LEFT) {
		Renderer->MoveCursorLeft(0);
	} else if (KeyCode == SDL_SCANCODE_BACKSPACE) {
		//if (CursorStartIndex > 0) {
		//	CursorStartIndex--;
		//	TString currentText = GetText();
		//	currentText = currentText.erase(CursorStartIndex, 1);
		//	SetText(currentText);
		//}
	} else if (KeyCode == SDL_SCANCODE_DELETE) {
		//if (CursorStartIndex < GetText().length() - 1) {
		//	TString currentText = GetText();
		//	currentText = currentText.erase(CursorStartIndex + 1, 1);
		//	SetText(currentText);
		//}
	} else if (KeyCode == SDL_SCANCODE_RETURN) {
		SubmitInput();
	}
	if (GetText().empty()) {
		SwapToPlaceholderText();
	}
}
void EditableTextWidget::OnTextInput(const TString& Text) {
	Renderer->AddTextToRightOfCursor(0, Text);
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
	CursorDrawInstruction->Location = Renderer->GetCursorRelativePosition(0) * Geometry.GetRenderResolution();
	//CursorDrawInstruction->Location.y = Geometry.GetLocation().y;
	CursorDrawInstruction->Size.y = Renderer->GetCursorHeight();

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