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

	AddCursor();
	AddCursor();
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
	//if (bCursorFlashOn && HasFocus()) {
	DrawCursor(Geometry);
	//}
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
		MoveCursorRight(0);
	} else if (KeyCode == SDL_SCANCODE_LEFT) {
		MoveCursorLeft(0);
	} else if (KeyCode == SDL_SCANCODE_UP) {
		MoveCursorUp(0);
	} else if (KeyCode == SDL_SCANCODE_DOWN) {
		MoveCursorDown(0);
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
	AddTextToRightOfCursor(0, Text);
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

void EditableTextWidget::DrawCursor(const FRenderGeometry& Geometry) {
	CursorDrawInstruction->Location = GetCursorRelativePosition(0) * Geometry.GetRenderResolution();
	//CursorDrawInstruction->Location.y = Geometry.GetLocation().y;
	CursorDrawInstruction->Size.y = GetCursorHeight();

	DrawBox(Geometry, *CursorDrawInstruction);
}
const void EditableTextWidget::MoveCursorRight(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	if (cursor.CharacterIndex < GetText().length() - 1) {
		cursor.CharacterIndex++;
	}
	DebugCursorState(CursorIndex);
}
const void EditableTextWidget::MoveCursorLeft(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	if (cursor.CharacterIndex > 0) {
		cursor.CharacterIndex--;
	}
	DebugCursorState(CursorIndex);
}
const void EditableTextWidget::MoveCursorUp(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// If we have more lines above us, go up.
	if (lineIndex > 0) {
		// Get the current line.
		const TextLine* currentLine = Renderer->GetGeometryCache()->GetLine(lineIndex);

		// Regardless of where we go, the first step is to go the start of the current line so might as
		// well do that first.
		cursor.CharacterIndex -= characterIndex;

		const TextLine* prevLine = Renderer->GetGeometryCache()->GetLine(lineIndex - 1);
		if (characterIndex > prevLine->GetLength() - 1) {
			cursor.CharacterIndex -= 1;
			cursor.bRightSide = true;
		} else {
			cursor.CharacterIndex -= (prevLine->GetLength() - characterIndex);
			cursor.bRightSide = false;
		}
	}
	DebugCursorState(CursorIndex);
}
const void EditableTextWidget::MoveCursorDown(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// If we have more lines to go, go down.
	if (lineIndex < Renderer->GetGeometryCache()->GetLineCount() - 1) {
		// Get the current line.
		const TextLine* currentLine = Renderer->GetGeometryCache()->GetLine(lineIndex);

		// Regardless of where we go, the first step is to go the end of the current line so might as
		// well do that first.
		cursor.CharacterIndex += (currentLine->GetLength() - characterIndex);

		const TextLine* nextLine = Renderer->GetGeometryCache()->GetLine(lineIndex + 1);
		if (characterIndex > nextLine->GetLength() - 1) {

			// Perform the max as this may be an empty line with a hidden \n.
			cursor.CharacterIndex += MathLibrary::Max(1, nextLine->GetLength());

			// Only go to the right if the line has a real character and not just a \n.
			if (nextLine->GetLength() > 0) {
				cursor.bRightSide = true;
			}
		} else {
			cursor.CharacterIndex += characterIndex;
			cursor.bRightSide = false;
		}
	}
	DebugCursorState(CursorIndex);
}
const Vector2D EditableTextWidget::GetCursorRelativePosition(const int32& CursorIndex) const {
	// Get the cursor.
	const FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// Calculate the position.
	Vector2D position = Renderer->GetNdcPosition();
	position.y -= Renderer->GetGeometryCache()->GetLineYOffset(lineIndex) * Renderer->GetNdcScale().y;

	// Add the x offset for the character if we are not on at character 0.
	if (characterIndex > 0) {
		// Get the current line and the verticies.
		const TextLine* line = Renderer->GetGeometryCache()->GetLine(lineIndex);
		const SArray<Vector2D>& verticies = line->GetVerticies();

		// Get the bottom left and top right for the character.
		Vector2D bottomLeft = verticies[characterIndex * 4];
		Vector2D topRight = verticies[(characterIndex * 4) + 2];

		// If on the right side, move the cursor there.
		// If on the left, move the cursor there and apply the tracking as well.
		if (cursor.bRightSide) {
			position.x += (topRight.x + Renderer->GetTracking()) * Renderer->GetNdcScale().x;
		} else {
			position.x += (bottomLeft.x - Renderer->GetTracking()) * Renderer->GetNdcScale().x;
		}
	} else {
		position.x += Renderer->GetTracking() * Renderer->GetNdcScale().x;
	}
	return MathLibrary::ConvertNdcToRelativeScreenCoordinates(position);
}
const int32 EditableTextWidget::GetCursorHeight() const {
	return Font->GetMaximumCharacterHeight() / 2.0f * Renderer->GetNdcScale().y * LastRenderedGeometry.GetRenderResolution().y;
}
const int32 EditableTextWidget::AddCursor() {
	Cursors.Add(FTextCursor());
	return Cursors.LastIndex();
}
const FTextCursor& EditableTextWidget::GetCursorAtIndex(const int32& CursorIndex) const {
	return Cursors[CursorIndex];
}
const void EditableTextWidget::AddTextToRightOfCursor(const int32& CursorIndex, const TString& Text) {
	// Get the cursor.
	const FTextCursor& cursor = Cursors[CursorIndex];

	TString currentText = GetText();
	if (cursor.CharacterIndex >= currentText.length() - 1) {
		currentText += Text;
	} else {
		currentText = currentText.insert(cursor.CharacterIndex, Text);
		MoveCursorRight(CursorIndex);
	}
	SetText(currentText);
}

void EditableTextWidget::DebugCursorState(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// Log the results.
	SD_ENGINE_DEBUG("Cursor is on line: {0} at index: {1} to the left of character: {2}.", lineIndex, cursor.CharacterIndex, GetText()[cursor.CharacterIndex]);
}