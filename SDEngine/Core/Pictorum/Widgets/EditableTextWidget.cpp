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
	TextWidget::Tick(DeltaTime, Geometry);
	CurrentBlinkTime += DeltaTime;
	if (CurrentBlinkTime > BlinkTime) {
		bCursorFlashOn = !bCursorFlashOn;
		CurrentBlinkTime = 0.0f;
	}
}
void EditableTextWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	if (/*bCursorFlashOn && */HasFocus()) {
		DrawCursor(Geometry);
	}

	TextWidget::Draw(DeltaTime, Geometry);
}
void EditableTextWidget::OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	const int32 index = GetCharacterIndexAtMouseLocation(MousePosition);
	SD_ENGINE_DEBUG("Mouse clicked at location: {0}.", index);
	MoveCursorToIndex(0, index);
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
		EraseLeftOfCursor(0);
	} else if (KeyCode == SDL_SCANCODE_DELETE) {
		EraseRightOfCursor(0);
	} else if (KeyCode == SDL_SCANCODE_RETURN) {
		AddNewLineAfterCursor(0);
		SubmitInput();
	} else if (KeyCode == SDL_SCANCODE_KP_ENTER) {
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
	CursorDrawInstruction->Size.y = (float)GetCursorHeight();
	DrawBox(Geometry, *CursorDrawInstruction);
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
	position.y += (GetCursorHeight() / LastRenderedGeometry.GetRenderResolution().y) / 2.0f;

	// Get the line
	const TextLine* line = Renderer->GetGeometryCache()->GetLine(lineIndex);

	// Add the x offset for the character if we are not on at character 0.
	if (characterIndex > 0 && characterIndex < line->GetGlyphCount()) {
		// Get the current line and the verticies.
		const SArray<Vector2D>& verticies = line->GetVerticies();

		// Get the bottom left and top right for the character.
		Vector2D bottomLeft = verticies[(int64)characterIndex * 4];

		if (characterIndex >= line->GetCursorInteractableGlyphCount()) {
			position.x += (bottomLeft.x + Renderer->GetTracking()) * Renderer->GetNdcScale().x;
		} else {
			position.x += (bottomLeft.x - Renderer->GetTracking()) * Renderer->GetNdcScale().x;
		}
	} else {
		position.x += Renderer->GetTracking() * Renderer->GetNdcScale().x;
	}
	return MathLibrary::ConvertNdcToRelativeScreenCoordinates(position);
}
const int32 EditableTextWidget::GetCharacterIndexAtMouseLocation(const Vector2D& MouseLocation) const {
	// Capture the mouse location in NDC space.
	const Vector2D mouseLocationAdjusted = MathLibrary::ConvertAbsoluteToNdcScreenCoordinates(MouseLocation, LastRenderedGeometry.GetRenderResolution());

	// Get a list of the lines.
	const SArray<TextLine*>& lines = Renderer->GetGeometryCache()->GetLines();

	// Keep track of the index offset as we traverse down the text block.
	int32 indexOffset = 0;
	for (int32 i = 0; i < lines.Count(); i++) {
		const TextLine* line = lines[i];

		// Capture the bounds in text space.
		Vector2D minBounds, maxBounds;
		line->GetBounds(minBounds, maxBounds);

		// Perform line index offset in text space.
		const float lineOffset = Renderer->GetGeometryCache()->GetLineYOffset(i - 1);
		minBounds.y -= lineOffset;
		maxBounds.y -= lineOffset;

		// Move into NDC space.
		minBounds = (minBounds * Renderer->GetNdcScale()) + Renderer->GetNdcPosition();
		maxBounds = (maxBounds * Renderer->GetNdcScale()) + Renderer->GetNdcPosition();

		// Check to see if we are in this line.
		if (mouseLocationAdjusted.y >= minBounds.y && mouseLocationAdjusted.y <= maxBounds.y) {
			const TString& text = line->GetText();
			const SArray<Vector2D>& verticies = line->GetVerticies();

			// Capture the start location of the line.
			Vector2D startLocation = verticies[0];
			startLocation *= Renderer->GetNdcScale();
			startLocation += Renderer->GetNdcPosition();

			// If the mouse location is to the left of the line's start location, assume we are at index 0.
			// Otherwise, iterate through all the characters of the line and see if we are inside their
			// respective quad. If we reach the end of the line having not entered a character quad, assume
			// we are at the end of the line.
			if (mouseLocationAdjusted.x < startLocation.x) {
				return indexOffset;
			} else {
				for (int32 j = 0; j < line->GetCursorInteractableGlyphCount(); j++) {
					if(j * 4 > verticies.LastIndex()) {
						continue;
					}
					// Capture the NDC space bottom left and top right of the quad.
					Vector2D bottomLeft = verticies[j * 4];
					bottomLeft *= Renderer->GetNdcScale();
					bottomLeft += Renderer->GetNdcPosition();

					Vector2D topRight = verticies[(j * 4) + 2];
					topRight *= Renderer->GetNdcScale();
					topRight += Renderer->GetNdcPosition();

					// Check to see if we're inside the quad in the X dimension.
					// Then, check if we're on the left or the right of the midpoint.
					if (mouseLocationAdjusted.x >= bottomLeft.x && mouseLocationAdjusted.x <= topRight.x) {
						float xMidpoint = bottomLeft.x + ((topRight.x - bottomLeft.x) / 2.0f);
						if (mouseLocationAdjusted.x > xMidpoint) {
							return indexOffset + j + 1;
						} else {
							return indexOffset + j;
						}
					}
				}
				return indexOffset + line->GetCursorInteractableGlyphCount() - 1;
			}
		} else {
			// If we were not in the line, move one line down and increment the index offset.
			indexOffset += line->GetCursorInteractableGlyphCount();
		}
	}
	return 0;
}
void EditableTextWidget::MoveCursorToIndex(const int32& CursorIndex, const int32& CharacterIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];
	if (CharacterIndex >= 0 && CharacterIndex <= Renderer->GetMaximumIndex()) {
		cursor.CharacterIndex = CharacterIndex;
	}
	DebugCursorState(0);
}

void EditableTextWidget::MoveCursorRight(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];
	MoveCursorToIndex(CursorIndex, cursor.CharacterIndex + 1);

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);
}
void EditableTextWidget::MoveCursorLeft(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];
	MoveCursorToIndex(CursorIndex, cursor.CharacterIndex - 1);
}
void EditableTextWidget::MoveCursorUp(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// Cache the current index.
	int32 tempIndex = cursor.CharacterIndex;

	// If we have more lines above us, go up.
	if (lineIndex > 0) {
		// Get the current line.
		const TextLine* currentLine = Renderer->GetGeometryCache()->GetLine(lineIndex);

		// Regardless of where we go, the first step is to go the start of the current line so might as
		// well do that first.
		tempIndex -= characterIndex;

		// Get the previous line and if our character index is greater than that, simply move to the end of that line.
		// Otherwise, move to the equivalent position in the previous line.
		const TextLine* prevLine = Renderer->GetGeometryCache()->GetLine(lineIndex - 1);
		if (characterIndex > prevLine->GetGlyphCount() - 1) {
			tempIndex -= 1;
			cursor.bRightSide = true;
		} else {
			tempIndex -= (prevLine->GetGlyphCount() - characterIndex);
			cursor.bRightSide = false;
		}
	}
	MoveCursorToIndex(CursorIndex, tempIndex);
}
void EditableTextWidget::MoveCursorDown(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// Cache the current index.
	int32 tempIndex = cursor.CharacterIndex;

	// If we have more lines to go, go down.
	if (lineIndex < Renderer->GetGeometryCache()->GetLineCount() - 1) {
		// Get the current line.
		const TextLine* currentLine = Renderer->GetGeometryCache()->GetLine(lineIndex);

		// Regardless of where we go, the first step is to go the end of the current line so might as
		// well do that first.
		tempIndex += (currentLine->GetGlyphCount() - characterIndex);

		const TextLine* nextLine = Renderer->GetGeometryCache()->GetLine(lineIndex + 1);
		if (characterIndex > nextLine->GetGlyphCount() - 1) {

			// Perform the max as this may be an empty line with a hidden \n.
			tempIndex += MathLibrary::Max(1, nextLine->GetGlyphCount() - 1);

			// Only go to the right if the line has a real character and not just a \n.
			if (nextLine->GetGlyphCount() > 0) {
				cursor.bRightSide = true;
			}
		} else {
			tempIndex += characterIndex;
			cursor.bRightSide = false;
		}
	}
	MoveCursorToIndex(CursorIndex, tempIndex);
}

const int32 EditableTextWidget::GetCharacterIndexOfCursor(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	int32 totalIndex = characterIndex;
	const SArray<TextLine*>& lines = Renderer->GetGeometryCache()->GetLines();
	for (int32 i = 0; i <= (lineIndex-1); i++) {
		totalIndex += lines[i]->GetGlyphCount() - 1;
		if (i > 0 && lines[i]->EndsWithIncompleteWord()) {
			totalIndex -= 1;
		}
	}
	if (totalIndex < GetText().length() - 1) {
		SD_ENGINE_DEBUG("Charcter at cursor is: {0}.", GetText()[totalIndex]);
	}
	return totalIndex;
}
const int32 EditableTextWidget::GetCursorHeight() const {
	return (int32)(Font->GetMaximumCharacterHeight() / 2.0f * Renderer->GetNdcScale().y * LastRenderedGeometry.GetRenderResolution().y);
}
const int32 EditableTextWidget::AddCursor() {
	Cursors.Add(FTextCursor());
	return Cursors.LastIndex();
}
const FTextCursor& EditableTextWidget::GetCursorAtIndex(const int32& CursorIndex) const {
	return Cursors[CursorIndex];
}

void EditableTextWidget::AddTextToRightOfCursor(const int32& CursorIndex, const TString& Text) {
	// Get the cursor index.
	const int32 characterIndex = GetCharacterIndexOfCursor(CursorIndex);

	// Modify the text (three cases for start of string, middle of string, end of string).
	TString currentText = GetText();
	if (characterIndex > currentText.length() - 1) {
		currentText += Text;
	} else if (currentText.length() == 0) {
		currentText = Text;
	} else {
		currentText = StringUtilities::InsertAtIndex(currentText, characterIndex, Text);
	}

	// Set the text and then move the cursor to the right one spot.
	SetText(currentText);
	MoveCursorRight(CursorIndex);
}
void EditableTextWidget::EraseLeftOfCursor(const int32& CursorIndex) {
	// Get the cursor index.
	const int32 characterIndex = GetCharacterIndexOfCursor(CursorIndex);

	TString currentText = GetText();
	if (characterIndex > 0) {
		currentText = StringUtilities::RemoveAtIndex(currentText, characterIndex - 1, 1);
		MoveCursorLeft(CursorIndex);
	}
	SetText(currentText);
}
void EditableTextWidget::EraseRightOfCursor(const int32& CursorIndex) {
	// Get the cursor index.
	const int32 characterIndex = GetCharacterIndexOfCursor(CursorIndex);

	TString currentText = GetText();
	if (characterIndex <= currentText.length() - 1) {
		currentText = StringUtilities::RemoveAtIndex(currentText, characterIndex, 1);
		if (characterIndex > Renderer->GetText().length() - 1) {
			MoveCursorLeft(CursorIndex);
		}
	}
	SetText(currentText);
}
void EditableTextWidget::AddNewLineAfterCursor(const int32& CursorIndex) {
	AddTextToRightOfCursor(CursorIndex, "\n");
}
void EditableTextWidget::DebugCursorState(const int32& CursorIndex) {
	// Get the cursor.
	FTextCursor& cursor = Cursors[CursorIndex];

	// Capture the line index and character index for the cursor.
	int32 lineIndex, characterIndex;
	Renderer->GetLineForCharacterIndex(cursor.CharacterIndex, lineIndex, characterIndex);

	// Get the text relative index.
	int32 realIndex = GetCharacterIndexOfCursor(CursorIndex);

	// Log the results.
	int32 lastIndex = Renderer->GetText().length() - 1;
	if (realIndex > lastIndex) {
		SD_ENGINE_DEBUG("Cursor is on line: {0} at index: {1} at the end of the text.", lineIndex, cursor.CharacterIndex);
	} else {
		SD_ENGINE_DEBUG("Cursor is on line: {0} at index: {1} to the left of character: {2}.", lineIndex, cursor.CharacterIndex, Renderer->GetText()[realIndex]);
	}

}