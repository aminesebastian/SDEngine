#include "TextRenderer.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Utilities/Math/MathLibrary.h"
#include "Core/Utilities/StringUtilities.h"


TextRenderer::TextRenderer(int32 FontSize, const DistanceFieldFont* Font) : Font(Font) {
	TextBlockCache = new FTextBlock(Leading, Tracking, Alignment);

	SetLeading(0.85f);
	SetTracking(-0.15f);
	SetColor(FColor(1.0f, 1.0f, 1.0f, 1.0f));
	SetFontSize(FontSize);
	SetFontWeight(EFontWeight::Normal);
	SetTextAlignment(ETextAlignment::LEFT);

	LastFrameMinBounds = ZERO_VECTOR2D;
	LastFrameMaxBounds = ZERO_VECTOR2D;

	VertexArrayBuffer = new GPUVertexBufferArray();
	VertexArrayBuffer->AddBuffer("Vertex", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("TexCoord", EGPUBufferType::ArrayBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Float);
	VertexArrayBuffer->AddBuffer("Index", EGPUBufferType::ElementBuffer, EGPUBufferUsage::StaticDraw, EGPUBufferDataType::Index);
	Flush();
}
TextRenderer::~TextRenderer() {
	delete VertexArrayBuffer;
	delete TextBlockCache;
}

void TextRenderer::Draw(const Vector2D& Position, const Vector2D& RenderTargetResolution, const Vector2D& DisplayDPI) {
	if (!bBoundToGPU) {
		BindToGPU();
	}

	LastFrameScale = (FontSize / RenderTargetResolution) * (DisplayDPI / DOTS_PER_POINT);
	LastFrameMinBounds = LastFrameScale * (TextBlockCache->MinPosition) * RenderTargetResolution;
	LastFrameMaxBounds = LastFrameScale * (TextBlockCache->MaxPosition) * RenderTargetResolution;
	LastFrameRenderPosition = Position + (LastFrameScale * (TextBlockCache->MinPosition));

	Shader* fontShader = EngineStatics::GetFontShader();
	fontShader->Bind();
	Font->BindAtlas(fontShader, "Atlas", 0);

	fontShader->SetShaderVector2("LOCATION", LastFrameRenderPosition);
	fontShader->SetShaderVector2("SCALE", LastFrameScale);
	fontShader->SetShaderVector4("COLOR", Color);
	fontShader->SetShaderVector2("RENDER_TARGET_RESOLUTION", RenderTargetResolution);
	fontShader->SetShaderFloat("WIDTH", DistanceFieldWidth);
	fontShader->SetShaderFloat("EDGE", DistanceFieldEdge);

	VertexArrayBuffer->DrawTriangleElements(2, TextBlockCache->IndexCount);
}
void TextRenderer::SetText(const TString& Text) {
	this->Text = Text;
	Flush();
	AddLine(Text);
	TextBlockCache->Finalize();
}
const TString& TextRenderer::GetText() const {
	return Text;
}
void TextRenderer::AddLine(const TString& Line) {
	TextBlockCache->GetCurrentLine()->SetLineSize((int32)Line.length());
	for (int32 i = 0; i < Line.length(); i++) {
		const char& character = Line[i];
		if (character == '\n') {
			TextBlockCache->CompleteLine();
		} else {
			const FDistanceFieldCharacter& dfChar = Font->GetDistanceFieldCharacter(character);
			TextBlockCache->GetCurrentLine()->AddCharacter(dfChar);
		}
	}

	// Make sure we complete the final line.
	TextBlockCache->CompleteLine();
}

void TextRenderer::SetFontSize(const int32& Size) {
	FontSize = (float)Size / DOTS_PER_POINT;
}
const int32 TextRenderer::GetFontSize() const {
	return (int32)(FontSize * DOTS_PER_POINT);
}
void TextRenderer::SetColor(const FColor& TextColor) {
	Color = TextColor;
}
const FColor& TextRenderer::GetColor() const {
	return Color;
}
void TextRenderer::SetTracking(const float& TextTracking) {
	Tracking = TextTracking;
	TextBlockCache->SetTracking(TextTracking);
}
const float& TextRenderer::GetTracking() const {
	return Tracking;
}
void TextRenderer::SetLeading(const float& TextLeading) {
	Leading = TextLeading;
	TextBlockCache->Leading = TextLeading;
}
const float& TextRenderer::GetLeading() const {
	return Leading;
}
void TextRenderer::SetFontWeight(const EFontWeight& Weight) {
	FontWeight = Weight;
	if (FontWeight == EFontWeight::Light) {
		DistanceFieldWidth = 0.45f;
		DistanceFieldEdge = 0.52f;
	} else if (FontWeight == EFontWeight::Normal) {
		DistanceFieldWidth = 0.47f;
		DistanceFieldEdge = 0.59f;
	} else if (FontWeight == EFontWeight::Bold) {
		DistanceFieldWidth = 0.54f;
		DistanceFieldEdge = 0.645f;
	}
}
const EFontWeight& TextRenderer::GetFontWeight() const {
	return FontWeight;
}
void TextRenderer::SetTextAlignment(const ETextAlignment& AlignmentIn) {
	Alignment = AlignmentIn;
	TextBlockCache->Alignment = AlignmentIn;
}
const ETextAlignment& TextRenderer::GetTextAlignment() const {
	return Alignment;
}
const FTextBlock* TextRenderer::GetInternalDataStructure() const {
	return TextBlockCache;
}
const void TextRenderer::GetTextBoundingBoxDimensions(Vector2D& MinBounds, Vector2D& MaxBounds) const {
	MinBounds = LastFrameMinBounds;
	MaxBounds = LastFrameMaxBounds;
}
const void TextRenderer::GetCharacterIndexAtMouseLocation(const Vector2D& MouseLocation, const Vector2D ScreenResolution, int32& Index, bool& RightSide) const {
	//Vector2D mouseLocationAdjusted = MouseLocation / ScreenResolution;
	//mouseLocationAdjusted          = (mouseLocationAdjusted - 0.5f) * 2.0f;
	//Index                          = -1;
	//RightSide                      = false;

	//Vector2D bottomLeft, topRight;
	//int32 lineStartIndex = 0;
	//bool wasInLine = false;

	//SArray<FTextLine*> Lines;
	//TextBlockCache->GetLines(Lines);

	//for (const FTextLine* line : Lines) {
	//	for (int32 i = 0; i < line->CharacterCount; i++) {
	//		const char character = line->Characters[i];

	//		GetAbsoluteCharacterBounds(lineStartIndex + i, bottomLeft, topRight);
	//		if (mouseLocationAdjusted.y >= bottomLeft.y && mouseLocationAdjusted.y <= topRight.y) {
	//			wasInLine = true;
	//			if (mouseLocationAdjusted.x >= bottomLeft.x && mouseLocationAdjusted.x <= topRight.x) {
	//				float xMidpoint = bottomLeft.x + ((topRight.x - bottomLeft.x) / 2.0f);
	//				bool shouldBeLeftSize = mouseLocationAdjusted.x < xMidpoint;
	//				if (shouldBeLeftSize) {
	//					Index = lineStartIndex + i;
	//				} else {
	//					if (i < line->CharacterCount - 1) {
	//						Index = lineStartIndex + i + 1;
	//					} else {
	//						Index = lineStartIndex + i;
	//						RightSide = true;
	//					}
	//				}					
	//				return;
	//			}
	//		}
	//	}
	//	if (wasInLine) {
	//		if (mouseLocationAdjusted.x > topRight.x) {
	//			Index = lineStartIndex + line->CharacterCount - 1;
	//			RightSide = true;
	//			return;
	//		} else {
	//			Index = lineStartIndex;
	//			RightSide = false;
	//			return;
	//		}
	//	}
	//	lineStartIndex = lineStartIndex + line->CharacterCount;
	//}
}
void TextRenderer::BindToGPU() {
	// Do not bind if there is no data.
	if (TextBlockCache->Verticies.Count() == 0 || TextBlockCache->TexCoords.Count() == 0 || TextBlockCache->Indices.Count() == 0) {
		SD_ENGINE_ERROR("There was an attempt to bind text geometry to the GPU where no geometry was defined!");
		return;
	}

	VertexArrayBuffer->SetBufferData(0, TextBlockCache->Verticies, TextBlockCache->VertexCount);
	VertexArrayBuffer->SetBufferData(1, TextBlockCache->TexCoords, TextBlockCache->TexCoordCount);
	VertexArrayBuffer->SetBufferData(2, TextBlockCache->Indices, TextBlockCache->IndexCount);
	VertexArrayBuffer->Update();

	bBoundToGPU = true;
}
void TextRenderer::Flush() {
	TextBlockCache->Flush();
	bBoundToGPU = false;
}

const void TextRenderer::MoveCursorRight() {
	const FTextLine* line = TextBlockCache->GetLine(Cursor.LineIndex);
	// If we are not at the end of the line, move towards the end.
	if (Cursor.CharacterIndex < line->CharacterCount - 1) {
		Cursor.CharacterIndex++;
	} else if (Cursor.CharacterIndex == line->CharacterCount - 1 && !Cursor.bRightSide) {
		Cursor.bRightSide = true;
	} else {
		// If we are not at the bottom right of the text block, move down a line and to the start of it.
		if (Cursor.LineIndex < TextBlockCache->UsedLines - 1) {
			Cursor.LineIndex++;
			Cursor.CharacterIndex = 0;
			Cursor.bRightSide = false;
		}
	}
	SD_ENGINE_DEBUG("Cursor is on line: {0} at character index: {1}.", Cursor.LineIndex, Cursor.CharacterIndex);
}
const void TextRenderer::MoveCursorLeft() {
	const FTextLine* line = TextBlockCache->GetLine(Cursor.LineIndex);
	// If we are not at the beginning of the line, move towards the beginning.

	if (Cursor.CharacterIndex == line->CharacterCount - 1 && Cursor.bRightSide) {
		Cursor.bRightSide = false;
	} else if (Cursor.CharacterIndex > 0) {
		Cursor.CharacterIndex--;
	} else {
		// If we are not at the top line, move up one line and move the cursor to the end of that line.
		if (Cursor.LineIndex > 0) {
			Cursor.LineIndex--;
			Cursor.CharacterIndex = TextBlockCache->GetLine(Cursor.LineIndex)->CharacterCount - 1;
			Cursor.bRightSide = false;
		}
	}
	SD_ENGINE_DEBUG("Cursor is on line: {0} at character index: {1}.", Cursor.LineIndex, Cursor.CharacterIndex);
}
const Vector2D TextRenderer::GetCursorRelativePosition() const {
	// Get the character bounds for the absolute character index and return the bottom left corner.
	Vector2D bottomLeft, topRight;
	GetNdcCharacterBounds(Cursor.LineIndex, Cursor.CharacterIndex, bottomLeft, topRight);

	// Capture the bottom left coordinate in relative coordinates since both left and right paths need this value.
	Vector2D relativeBottomLeft = MathLibrary::ConvertNdcToRelativeScreenCoordinates(bottomLeft);

	// If on the right side, shift the X coord to be the right of the character quad, otherwise, use the left side.
	// Additionally, if on the right side, undo the adjustment for tracking.
	if (Cursor.bRightSide) {
		topRight.x += Tracking * LastFrameScale.x;
		Vector2D relativeTopRight = MathLibrary::ConvertNdcToRelativeScreenCoordinates(topRight);
		return Vector2D(relativeTopRight.x, relativeBottomLeft.y);
	} else {
		return relativeBottomLeft;
	}
}
const int32 TextRenderer::GetAbsoluteIndexFromLineRelative(const int32& LineIndex, const int32& CharacterIndex) const {
	// Return a -1 if the LineIndex is out of bounds.
	if (LineIndex > TextBlockCache->UsedLines) {
		return -1;
	}

	// Return a -1 if the CharacterIndex is out of bounds.
	if (CharacterIndex > TextBlockCache->GetLine(LineIndex)->CharacterCount) {
		return -1;
	}

	// Get all the lines of text.
	SArray<FTextLine*> lines;
	TextBlockCache->GetLines(lines);

	// Get the absolute character index from the line relative.
	int32 characterIndex = Cursor.CharacterIndex;
	for (int32 i = 0; i < Cursor.LineIndex; i++) {
		characterIndex += lines[i]->CharacterCount;
	}
	return characterIndex;
}
const void TextRenderer::GetNdcCharacterBounds(const int32& LineIndex, const int32& Index, Vector2D& BottomLeft, Vector2D& TopRight) const {
	if (LineIndex < TextBlockCache->UsedLines) {
		// Get the overall character index.
		int32 characterIndex = GetAbsoluteIndexFromLineRelative(LineIndex, Index);

		// Ensure we recieved a valid character index.
		if (characterIndex == -1) {
			SD_ENGINE_WARN("Attempting to get the NDC bounds for an out of bounds character. Values provided were LineIndex={0} and CharacterIndex={1}.", LineIndex, Index);
			return;
		}

		// Capture the current line.
		const FTextLine* line = TextBlockCache->GetLine(LineIndex);

		// Capture the text space coordinates.
		BottomLeft = TextBlockCache->Verticies[Index * 4];
		TopRight = TextBlockCache->Verticies[(Index * 4) + 2];

		// Add the tracking if we are beyond the first character.
		if (Index > 0 && Index < line->CharacterCount - 1) {
			BottomLeft.x -= Tracking;
			TopRight.x -= Tracking;
		}

		// Transform from text space to ndc coordinates.
		BottomLeft *= LastFrameScale;
		BottomLeft += LastFrameRenderPosition;
		TopRight *= LastFrameScale;
		TopRight += LastFrameRenderPosition;
	} else {
		SD_ENGINE_WARN("Attempting to get the NDC bounds for a character with either invalid LineIndex or Index. Values provided were LineIndex={0} and CharacterIndex={1}.", LineIndex, Index);
	}
}