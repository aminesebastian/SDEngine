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
	const FColor& GetTextColor();
	void SetFontSize(const int32& FontSize);
	const int32 GetFontSize();

	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds) const override;
	virtual bool CanAddChild() const override;

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) override;
protected:
	TextRenderer* Renderer;
	DistanceFieldFont* Font;
	vec2 LastRenderedAbsoluteLocation;
};

