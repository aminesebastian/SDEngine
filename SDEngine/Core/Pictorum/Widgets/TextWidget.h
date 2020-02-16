#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TextRenderer;
class DistanceFieldFont;

class TextWidget : public PictorumWidget {
public:
	TextWidget(const TString& Name);
	virtual ~TextWidget();

	void SetText(const TString& Text);
	void SetTextColor(const FColor& Color);
	const FColor& GetTextColor() const;
	void SetFontSize(const int32& FontSize);
	const int32 GetFontSize() const;
	void SetTextAlignment(const ETextAlignment& Alignment);
	const ETextAlignment& GetAlignment() const; 
	void SetFontWeight(const EFontWeight& Weight);
	const EFontWeight& GetWeight() const;

	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const override;
	virtual const bool CanAddChild() const override;

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& EventIn) override;

protected:
	TextRenderer* Renderer;
	DistanceFieldFont* Font;
	vec2 LastRenderedAbsoluteLocation;
};

