#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class SolidWidget : public PictorumWidget {
public:
	SolidWidget(const TString& Name);
	virtual  ~SolidWidget();
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	virtual const bool CanAddChild() const override;

	const FPadding& GetPadding() const;
	void SetPadding(const float& AllPadding);
	void SetPadding(const float& TopBottomPadding, const float& RightLeftPadding);
	void SetPadding(const float& Top, const float& Right, const float& Bottom, const float& Left);
	void SetBackgroundColor(const FColor& NewColor);
	void SetBorderRadius(const float& TopLeft, const float& TopRight, const float& BottomLeft, const float& BottomRight);

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) override;
	virtual void OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) override;
	virtual void OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) override;

	virtual bool PopulateDetailsPanel() override;

private:
	FPadding Padding;
	FBorderRadius Radius;
	float EdgeSoftness;
	FColor BackgroundColor;
	bool bWasMouseDownInWidget;

	GLuint quadVAO;
	GLuint quadVBO;
};

