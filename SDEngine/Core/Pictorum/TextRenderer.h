#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/Logger.h"
#include "Core/Rendering/OpenGL/GPUVertexBufferArray.h"
#include <limits>

/* Defines how dots for each point in font size.*/
#define DOTS_PER_POINT 5.0f

struct FTextLine {
private:
	friend class TextRenderer;
	friend struct FTextBlock;
	FTextLine(const float& Tracking) : Tracking(Tracking) {
		CursorPosition     = 0.0f;
		CurrentIndex       = 0;
		MaxYBaselineOffset = 0.0f;
	}
	~FTextLine() {
		Characters.Clear();
		Verticies.Clear();
		TexCoords.Clear();
		Indices.Clear();
	}
	void SetLineSize(const int32& Size) {
		Characters.PreAllocate(Size);
		Verticies.PreAllocate(Size * 4);
		Verticies.PreAllocate(Size * 4);
		Indices.PreAllocate(Size * 6);
	}
	void AddCharacter(const FDistanceFieldCharacter& Character) {
		if (Character.GetCharacter() != ' ') {
			Characters.Add(Character.GetCharacter());
			TexCoords.AddAll(Character.GetTextureCoordinates());
			for (const int32& index : Character.GetIndices()) {
				Indices.Add(index + CurrentIndex);
			}
			for (const vec2& vert : Character.GetVerticies()) {
				Verticies.Add(vec2(vert.x + CursorPosition, vert.y));
			}
			CurrentIndex += 4;
			if (Character.GetOffsets().y < MaxYBaselineOffset) {
				MaxYBaselineOffset = Character.GetOffsets().y;
			}
		}
		CursorPosition += Character.GetAdvance() + Tracking;
	}
	const float Tracking;
	int32 CurrentIndex;
	float CursorPosition;
	float MaxYBaselineOffset;
	SArray<char> Characters;
	SArray<vec2> Verticies;
	SArray<vec2> TexCoords;
	SArray<int32> Indices;
};
struct FTextBlock {
private:
	friend class TextRenderer;
	FTextBlock(const float& Leading, const float& Tracking, const ETextAlignment& Alignment) : Leading(Leading), Tracking(Tracking), Alignment(Alignment) {
		CurrentYPosition = 0.0f;
		CurrentIndex     = 0;
		CurrentLine      = new FTextLine(Tracking);
	}
	~FTextBlock() {
		Verticies.Clear();
		TexCoords.Clear();
		Indices.Clear();
	}
	FTextLine* GetCurrentLine() {
		return CurrentLine;
	}
	void CompleteLine() {
		CurrentLine = new FTextLine(Tracking);
		Lines.Add(CurrentLine);
	}
	void Finalize() {
		// Capture the max line length and capture the total count of characters.
		float maxLength = 0.0f;
		int32 characterCount = 0;
		for (FTextLine* line : Lines) {
			if (line->CursorPosition > maxLength) {
				maxLength = line->CursorPosition;
				characterCount += line->Characters.Count();
			}
		}

		// Preallocate the required amount of memory.
		Verticies.PreAllocate(characterCount * 4);
		TexCoords.PreAllocate(characterCount * 4);
		Indices.PreAllocate(characterCount * 6);

		for (FTextLine* line : Lines) {
			float alignmentOffset = maxLength - line->CursorPosition;
			int32 maxIndex = 0;
			TexCoords.AddAll(line->TexCoords);
			for (const int32& index : line->Indices) {
				Indices.Add(index + CurrentIndex);
				if (index > maxIndex) {
					maxIndex = index;
				}
			}

			for (const vec2& vert : line->Verticies) {
				if (Alignment == ETextAlignment::LEFT) {
					Verticies.Add(vec2(vert.x, vert.y + CurrentYPosition));
				}else if (Alignment == ETextAlignment::RIGHT) {
					Verticies.Add(vec2(vert.x + alignmentOffset, vert.y + CurrentYPosition));
				} else if (Alignment == ETextAlignment::CENTER) {
					Verticies.Add(vec2(vert.x + (alignmentOffset / 2.0f), vert.y + CurrentYPosition));
				}

				vec2& addedVert = Verticies[Verticies.LastIndex()];

				if (vert.x > MaxPosition.x) {
					MaxPosition.x = vert.x;
				}
				if (addedVert.y < MaxPosition.y) {
					MaxPosition.y = addedVert.y;
				}

				if (vert.x < MinPosition.x) {
					MinPosition.x = vert.x;
				}
				if (addedVert.y < MinPosition.y) {
					MinPosition.y = addedVert.y;
				}
			}
			CurrentYPosition -= Leading;
			CurrentYPosition -= line->MaxYBaselineOffset;
			CurrentIndex += (maxIndex + 1);
		}	
		MaxPosition.y *= -1;
		MaxPosition.y /= 2.0f;
	}
	void Reset() {
		MaxPosition = vec2(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::max());
		MinPosition  = vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CurrentYPosition = 0.0f;
		CurrentIndex     = 0;
		Verticies.Clear();
		TexCoords.Clear();
		Indices.Clear();
		for (FTextLine* line : Lines) {
			delete line;
		}
		Lines.Clear();
		CurrentLine = new FTextLine(Tracking);
		Lines.Add(CurrentLine);
	}
	ETextAlignment Alignment;
	float Leading;
	float Tracking;

	SArray<FTextLine*> Lines;
	FTextLine* CurrentLine;

	vec2 MaxPosition;
	vec2 MinPosition;

	SArray<vec2> Verticies;
	SArray<vec2> TexCoords;
	SArray<int32> Indices;

	float CurrentYPosition;
	int32 CurrentIndex;
};

/**
 * Class responsible for rendering text to the screen.
 * Supports any distance field font and exposes basic customization options.
 *
 * @author	Amine
 * @date	2/4/2020
 */
class TextRenderer {
public:
	/**
	 * Constructor
	 *
	 * @param 	{int32}	FontSize					Size of the font.
	 * @param 	{const DistanceFieldFont*}	Font	The distance field font.
	 */
	TextRenderer(int32 FontSize, const DistanceFieldFont* Font);
	virtual ~TextRenderer();

	/**
	 * Draws the text to the screen at the provided NDC position. Automatically adjusts for the
	 * provided render target resolution.
	 *
	 * @param 	{const vec2&}	Position			  	The position of the text in NDC coordinates.
	 * @param 	{const vec2&}	RenderTargetResolution	The render target resolution.
	 * @param 	{const vec2&}	DisplayDPI			  	The display DPI.
	 */
	void Draw(const vec2& Position, const vec2& RenderTargetResolution, const vec2& DisplayDPI);

	/**
	 * Sets the text lines for this text renderer to render.
	 * Lines can be split by including a \n new line character in the Text.
	 *
	 * @param 	{const TString&}	Text	The text to render.
	 */
	void SetText(const TString& Text);

	/**
	 * Sets the font size.
	 *
	 * @param 	{const int32&}	Size	The new font size.
	 */
	void SetFontSize(const int32& Size);
	/**
	 * Gets the font size.
	 *
	 * @returns	{const int32&}	The font size.
	 */
	const int32 GetFontSize() const;
	/**
	 * Sets the font color.
	 *
	 * @param 	{const FColor&}	TextColor	The new text color.
	 */
	void SetColor(const FColor& TextColor);
	/**
	 * Gets the font color.
	 *
	 * @returns	{const FColor&}	The color.
	 */
	const FColor& GetColor() const;
	/**
	 * Sets the font tracking.
	 *
	 * @param 	{const float&}	TextTracking	The new text tracking.
	 */
	void SetTracking(const float& TextTracking);
	/**
	 * Gets the font tracking.
	 *
	 * @returns	{const float&}	The tracking.
	 */
	const float& GetTracking() const;
	/**
	 * Sets the font leading.
	 *
	 * @param 	{const float&}	TextLeading	The new text leading.
	 */
	void SetLeading(const float& TextLeading);
	/**
	 * Gets the font leading.
	 *
	 * @returns	{const float&}	The leading.
	 */
	const float& GetLeading() const;
	/**
	 * Sets the font weight.
	 *
	 * @param 	{const EFontWeight&}	Weight	The new font weight.
	 */
	void SetFontWeight(const EFontWeight& Weight);
	/**
	 * Gets the font weight.
	 *
	 * @returns	{const EFontWeight&}	The font weight.
	 */
	const EFontWeight& GetFontWeight() const;

	/**
	 * Sets the text alignment
	 *
	 * @param 	{const ETextAlignment&}	AlignmentIn	The alignment.
	 */
	void SetTextAlignment(const ETextAlignment& AlignmentIn);

	/**
	 * Gets the text alignment
	 *
	 * @returns	{const ETextAlignment&}	The alignment.
	 */
	const ETextAlignment& GetTextAlignment() const;

	/**
	 * Gets text bounding box dimensions.
	 *
	 * @returns	{const vec2&}	The text bounding box dimensions.
	 */
	const vec2& GetTextBoundingBoxDimensions() const;
protected:
	virtual void BindToGPU();
	virtual void Reset();
	virtual void AddLine(const TString& Line);
private:
	/*****************/
	/*Text Properties*/
	/*****************/
	float FontSize;
	EFontWeight FontWeight;
	ETextAlignment Alignment;
	FColor Color;
	float DistanceFieldWidth;
	float DistanceFieldEdge;
	float Tracking;
	float Leading;

	/*****************/
	/*State Properties*/
	/*****************/
	const DistanceFieldFont* Font;
	vec2 LastBoundingBoxDimensions;

	/*****************/
	/*Render Properties*/
	/*****************/
	bool bBoundToGPU;
	FTextBlock* TextBlockCache;
	GPUVertexBufferArray* TestBuffer;
};

