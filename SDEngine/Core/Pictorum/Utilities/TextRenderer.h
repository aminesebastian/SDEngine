#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
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
		// First flush the line to ensure all values are set to their base state. Then, allocate 64 characters worth of space.
		Flush();
		ExtendAllocation(64);
	}
	~FTextLine() {
		Flush();
	}
	void SetLineSize(const int32& Size) {
		if (Size > Characters.Count()) {
			ExtendAllocation(Size);
		}
	}
	void AddCharacter(const FDistanceFieldCharacter& Character) {
		// If the character is a space, skip it and just advance.
		if (Character.GetCharacter() != ' ') {
			if (CharacterCount == 0) {
				FirstCharacterOffset.x = Character.GetOffsets().x;
				//FirstCharacterOffset.y = -Character.GetOffsets().y/2.0f;
			}
			
			// Extend allocation if needed.
			if (CharacterCount > Characters.LastIndex()) {
				ExtendAllocation(5);
			}

			// Only generate new geometry if the character in that index has changed.
			//if (Character.GetCharacter() != Characters[CharacterCount]) {
				// Capture the indices of the first and last changed character.
				if (FirstChangedIndex == 0) {
					FirstChangedIndex = CharacterCount;
				}
				LastChangedIndex = CharacterCount;

				// Set the character.
				Characters[CharacterCount] = Character.GetCharacter();

				// Set the texture coordinates.
				for (int32 i = 0; i < Character.GetTextureCoordinates().Count(); i++) {
					TexCoords[TexCoordCount + i] = Character.GetTextureCoordinates()[i];
				}

				// Set the indices.
				for (int32 i = 0; i < Character.GetIndices().Count(); i++) {
					Indices[IndexCount + i] = Character.GetIndices()[i] + CurrentIndex;
				}

				// Set the vertices.
				for (int32 i = 0; i < Character.GetVerticies().Count(); i++) {
					const vec2& vert = Character.GetVerticies()[i];
					Verticies[VertexCount + i] = vec2(vert.x + CursorPosition, vert.y) - FirstCharacterOffset;
				}
				bChanged = true;
			//}

			// Increment the counts for all state tracking values.
			IncrementCounts();

			// Capture the max offset.
			if (Character.GetOffsets().y < MaxYBaselineOffset) {
				MaxYBaselineOffset = Character.GetOffsets().y;
			}
		}

		// Advance the cursor.
		CursorPosition += Character.GetAdvance() + Tracking;
	}
	void Flush() {
		CursorPosition        = 0.0f;
		MaxYBaselineOffset    = 0.0f;
		FirstCharacterOffset.x = 0.0;
		FirstCharacterOffset.y = 0.0f;
		CurrentIndex          = 0;
		CharacterCount        = 0;
		VertexCount           = 0;
		IndexCount            = 0;
		TexCoordCount         = 0;
		FirstChangedIndex     = -1;
		LastChangedIndex      = -1;
		bChanged              = false;
	}
	bool HasChanged() {
		return FirstChangedIndex >= 0;
	}

private:
	void ExtendAllocation(int32 CharactersToAllocate) {
		const int32 allocationAmount = CharactersToAllocate;
		Characters.Resize(Characters.Count() + allocationAmount, -1);
		Verticies.Resize(Verticies.Count() + allocationAmount * 4);
		TexCoords.Resize(TexCoords.Count() + allocationAmount * 4);
		Indices.Resize(Indices.Count() + allocationAmount * 6);
	}
	void IncrementCounts() {
		CurrentIndex += 4;
		CharacterCount += 1;
		VertexCount += 4;
		TexCoordCount += 4;
		IndexCount += 6;
	}
	vec2 FirstCharacterOffset;
	float Tracking;
	int32 CurrentIndex;
	float CursorPosition;
	float MaxYBaselineOffset;
	int32 FirstChangedIndex;
	int32 LastChangedIndex;
	int32 CharacterCount;
	SArray<char> Characters;
	int32 VertexCount;
	SArray<vec2> Verticies;
	int32 TexCoordCount;
	SArray<vec2> TexCoords;
	int32 IndexCount;
	SArray<int32> Indices;
	bool bChanged;
};

struct FTextBlock {
private:
	friend class TextRenderer;
	FTextBlock(const float& Leading, const float& Tracking, const ETextAlignment& Alignment) : Leading(Leading), Tracking(Tracking), Alignment(Alignment) {
		Flush();
		Lines.Add(new FTextLine(Tracking));
		ExtendAllocation(64);
		AllocatedCharacterSpace = 0;
	}
	~FTextBlock() {
		Flush();
	}

	FTextLine* GetCurrentLine() {
		return Lines[UsedLines];
	}
	void CompleteLine() {
		UsedLines++;
		if (UsedLines > Lines.LastIndex()) {
			Lines.Add(new FTextLine(Tracking));
		}
	}
	void Finalize() {
		// Capture the max line length for use when calculating the alignment offset.
		float maxLength = 0.0f;
		for (int32 i = 0; i <= UsedLines; i++) {
			FTextLine* line = Lines[i];
			if (line->CharacterCount == 0) {
				continue;
			}
			if (line->CursorPosition > maxLength) {
				maxLength = line->CursorPosition;
			}
		}

		// Iterate through all the used lines and add them to the buffer.
		for (int32 i = 0; i <= UsedLines; i++) {
			FTextLine* line = Lines[i];
			if (line->CharacterCount == 0) {
				continue;
			}

			// Determine if we need to extend the buffer allocation.
			int32 remainingCharSpace = AllocatedCharacterSpace - CurrentlyUsedCharacterSpace;
			if (line->CharacterCount > remainingCharSpace) {
				ExtendAllocation(line->CharacterCount - remainingCharSpace);
			}

			// Calculate the alignment offset.
			float alignmentOffset = maxLength - line->CursorPosition;
	
			// Add texture coordinates.
			for (int32 i = line->TexCoordCount; i >= 0 ; i--) {
				TexCoords[TexCoordCount + i] = line->TexCoords[i];
			}

			// Add Indices and capture the max index.
			int32 maxIndex = 0;
			for (int32 i = line->IndexCount - 1; i >= 0 ; i--) {
				int32& index = line->Indices[i];
				Indices[IndexCount + i] = index + CurrentIndex;
				if (index > maxIndex) {
					maxIndex = index;
				}
			}

			// Add vertices and offset for the alignment.
			for (int32 i = line->VertexCount - 1; i >= 0; i--) {
				const vec2& vert = line->Verticies[i];
				vec2& blockVert  = Verticies[VertexCount + i];

				if (Alignment == ETextAlignment::LEFT) {
					blockVert.x = vert.x;
					blockVert.y = vert.y + CurrentYPosition;
				} else if (Alignment == ETextAlignment::RIGHT) {
					blockVert.x = vert.x + alignmentOffset;
					blockVert.y = vert.y + CurrentYPosition;
				} else if (Alignment == ETextAlignment::CENTER) {
					blockVert.x = vert.x + (alignmentOffset / 2.0f);
					blockVert.y = vert.y + CurrentYPosition;
				}

				// Cache the max and min positions of this whole block by evaluating the latest vertex.	
				if (blockVert.x > MaxPosition.x) {
					MaxPosition.x = blockVert.x;
				}
				if (-blockVert.y > MaxPosition.y) {
					MaxPosition.y = -blockVert.y;
				}
				if (blockVert.x < MinPosition.x) {
					MinPosition.x = blockVert.x;
				}
				if (-blockVert.y < MinPosition.y) {
					MinPosition.y = -blockVert.y;
				}
			}

			// Offset the cursor to the next line.
			CurrentYPosition -= Leading;
			CurrentYPosition -= line->MaxYBaselineOffset;

			// Increment state keeping values.
			CurrentIndex                += (maxIndex + 1);
			VertexCount                 += line->VertexCount;
			TexCoordCount               += line->TexCoordCount;
			IndexCount                  += line->IndexCount;
			CurrentlyUsedCharacterSpace += line->CharacterCount;
		}
		MaxPosition.y = -CurrentYPosition;

		MaxPosition /= 2.0f;
		MinPosition /= 2.0f;
	}
	void Flush() {
		UsedLines = 0;
		MaxPosition = vec2(std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest());
		MinPosition = vec2(std::numeric_limits<float>::max(), std::numeric_limits<float>::max());
		CurrentYPosition = 0.0f;
		CurrentIndex = 0;
		VertexCount = 0;
		TexCoordCount = 0;
		IndexCount = 0;
		FirstChangedIndex = 0;
		LastChangedIndex = 0;
		CurrentlyUsedCharacterSpace = 0;

		for (FTextLine* line : Lines) {
			line->Flush();
		}
	}
	void SetTracking(const float& TrackingIn) {
		Tracking = TrackingIn;
		for (FTextLine* line : Lines) {
			line->Tracking = TrackingIn;
		}
	}
	void GetChangedIndices(int32& FirstChanged, int32& LastChanged) {
		FirstChanged = FirstChangedIndex;
		LastChanged = LastChangedIndex;
	}
	bool HasChanged() {
		return FirstChangedIndex >= 0;
	}

private:
	void ExtendAllocation(const int32& CharacterCount) {
		const int32 allocationAmount = CharacterCount * 2;

		AllocatedCharacterSpace += allocationAmount;
		Verticies.Resize(Verticies.Count() + allocationAmount * 4);
		TexCoords.Resize(TexCoords.Count() + allocationAmount * 4);
		Indices.Resize(Indices.Count() + allocationAmount * 6);
	}
	ETextAlignment Alignment;
	float Leading;
	float Tracking;

	SArray<FTextLine*> Lines;
	int32 UsedLines;

	vec2 MaxPosition;
	vec2 MinPosition;

	int32 AllocatedCharacterSpace;
	int32 CurrentlyUsedCharacterSpace;

	int32 VertexCount;
	SArray<vec2> Verticies;
	int32 TexCoordCount;
	SArray<vec2> TexCoords;
	int32 IndexCount;
	SArray<int32> Indices;

	int32 FirstChangedIndex;
	int32 LastChangedIndex;

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

	const void GetTextBoundingBoxDimensions(vec2& MinBounds, vec2& MaxBounds) const;
protected:
	virtual void BindToGPU();
	virtual void Flush();
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
	vec2 LastFrameMinBounds;
	vec2 LastFrameMaxBounds;
	TString Text;

	/*****************/
	/*Render Properties*/
	/*****************/
	bool bBoundToGPU;
	FTextBlock* TextBlockCache;
	GPUVertexBufferArray* VertexArrayBuffer;
};

