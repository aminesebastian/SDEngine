#pragma once
#include "Core/Objects/CoreTypes/DistanceFieldFont.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Utilities/TextFormatters/ITextInputFormatter.h"
#include "Core/Pictorum/Utilities/TextRenderer.h"

class TextWidget : public PictorumWidget {
public:
	TextWidget(const TString& Name);
	virtual ~TextWidget();

	void SetText(const TString& Text);
	const TString& GetText() const;
	void SetTextColor(const FColor& Color);
	const FColor& GetTextColor() const;
	void SetFontSize(const int32& FontSize);
	const int32 GetFontSize() const;
	void SetTextAlignment(const ETextAlignment& Alignment);
	const ETextAlignment& GetAlignment() const; 
	void SetFontWeight(const EFontWeight& Weight);
	const EFontWeight& GetWeight() const;
	void SetWordWrapRule(const ETextWrapRule& Rule);
	const ETextWrapRule& GetWordWrapRule() const;

	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const override;
	virtual const bool CanAddChild() const override;

protected:
	virtual void OnTextSet(const TString& Text);
	virtual void OnRenderGeometryChanged(const FRenderGeometry& NewRenderGeometry) override;

	TextRenderer* Renderer;
	DistanceFieldFont* Font;
	ETextWrapValueType WordWrapValueType;
};

