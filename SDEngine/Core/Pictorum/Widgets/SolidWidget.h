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

	virtual void OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) override;
	virtual void OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) override;

private:
	FPadding Padding;
	
	FBoxDrawInstruction* DrawInstruction;
};

