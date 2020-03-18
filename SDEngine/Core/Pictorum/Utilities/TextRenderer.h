#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Core/Utilities/Logger.h"
#include "Core/Rendering/OpenGL/GPUVertexBufferArray.h"
#include "Core/Pictorum/Utilities/TextGeometryCache.h"

/* Defines how dots for each point in font size.*/
#define DOTS_PER_POINT 5.0f

/**
 * Class responsible for rendering text to the screen. Supports any distance field font and
 * exposes basic customization options.
 *
 * @author	Amine
 * @date	2/4/2020
 */
class TextRenderer {
public:

	/**
	 * Constructor
	 *
	 * @param 	FontSize	FontSize					Size of the font.
	 * @param 	Font		DistanceFieldFont*}	Font	The distance field font.
	 */
	TextRenderer(int32 FontSize, const DistanceFieldFont* Font);
	virtual ~TextRenderer();

	/**
	 * Draws the text to the screen at the provided NDC position. Automatically adjusts for the
	 * provided render target resolution.
	 *
	 * @param 	Position			  	Vector2D&amp;}	Position			  	The position of the text in NDC
	 * 									coordinates.
	 * @param 	RenderTargetResolution	Vector2D&amp;}	RenderTargetResolution	The render target
	 * 									resolution.
	 * @param 	DisplayDPI			  	Vector2D&amp;}	DisplayDPI			  	The display DPI.
	 */
	void Draw(const Vector2D& Position, const Vector2D& RenderTargetResolution, const Vector2D& DisplayDPI);

	/**
	 * Sets the text lines for this text renderer to render. Lines can be split by including a \n
	 * new line character in the Text.
	 *
	 * @param 	TextIn	TString&amp;}	TextIn	The text to render.
	 */
	void SetText(const TString& TextIn);
	/**
	 * Gets the text
	 *
	 * @returns	The text.
	 */
	const TString& GetText() const;

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
	 * Sets word wrap width in relative space.
	 *
	 * @param 	Width	The width in relative space.
	 */
	void SetWordWrapWidth(const float& Width);

	/**
	 * Gets word wrap width in relative space.
	 *
	 * @returns	The word wrap width in relative space.
	 */
	const float GetWordWrapWidth() const;
	/**
	 * Gets text bounding box dimensions
	 *
	 * @param [in,out]	MinBounds	The minimum bounds.
	 * @param [in,out]	MaxBounds	The maximum bounds.
	 *
	 * @returns	The text bounding box dimensions.
	 */
	const void GetTextBoundingBoxDimensions(Vector2D& MinBounds, Vector2D& MaxBounds) const;

	/**
	 * Gets the screen NDC character bounds.
	 *
	 * @param 		  	LineIndex 	Zero-based index of the line.
	 * @param 		  	Index	  	Zero-based index of the.
	 * @param [in,out]	BottomLeft	The bottom left of the character.
	 * @param [in,out]	TopRight  	The top right of the character.
	 *
	 * @returns	The relative character bounds.
	 */
	const void GetNdcCharacterBounds(const int32& CharacterIndex, Vector2D& BottomLeft, Vector2D& TopRight) const;
	const TextGeometryCache* GetGeometryCache() const;
	void GetLineForCharacterIndex(const int32& AbsoluteIndex, int32& LineIndex, int32& CharacterIndex) const;
	const Vector2D& GetNdcScale() const;
	const Vector2D& GetNdcPosition() const;
protected:
	virtual void BindToGPU();
	virtual void Flush();

private:
	friend class EditableTextWidget;

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
	Vector2D LastFrameMinBounds;
	Vector2D LastFrameMaxBounds;
	TString Text;

	/*****************/
	/*Render Properties*/
	/*****************/
	bool bBoundToGPU;
	Vector2D CalculatedRenderScale;
	Vector2D LastFrameRenderPosition;
	TextGeometryCache* TextBlockCache;
	GPUVertexBufferArray* VertexArrayBuffer;
};

