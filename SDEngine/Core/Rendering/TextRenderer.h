#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Pictorum/DistanceFieldFont.h"

/* Defines how dots for each point in font size.*/
#define DOTS_PER_POINT 3.0f

/** The font weight to use when rendering text. */
enum class EFontWeight : uint8 {
	Light, 
	Normal,
	Bold
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
	 * Sets the text lines for this text renderer to render.
	 * Lines can be split by including a \n new line character in the Text.
	 * Lines can also be split by setting bNewLinePerEntry to true, in which case each array entry will be a new line.
	 *
	 * @param 	{const SArray{TString}&}	TextArray	The array of text to render.
	 * @param 	{bool}	bNewLinePerEntry			 	True to new line per entry.
	 */
	void SetText(const SArray<TString>& TextArray, bool bNewLinePerEntry);

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
	 * @param 	{FColor}	TextColor	The new text color.
	 */
	void SetColor(FColor TextColor);
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

protected:
	virtual void AddGlyph(const FDistanceFieldCharacter& Character);
	virtual void BindToGPU();
	virtual void NewLine();
	virtual void Reset();

private:
	/*****************/
	/*Text Properties*/
	/*****************/
	float FontSize;
	EFontWeight FontWeight;
	FColor Color;
	float DistanceFieldWidth;
	float DistanceFieldEdge;
	float Tracking;
	float Leading;
	float SpaceWidth;

	/*****************/
	/*State Properties*/
	/*****************/
	const DistanceFieldFont* Font;
	vec2 CursorPosition;

	/*****************/
	/*Render Properties*/
	/*****************/
	bool bBoundToGPU;
	SArray<vec2> Verticies;
	SArray<vec2> TexCoords;
	SArray<uint32> Indices;
	int32 CurrentIndexValue;
	SArray<GLuint> VertexArrayBuffers;
	GLuint VertexArrayObject;
};

