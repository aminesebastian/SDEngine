#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class PictorumScrollBox : public PictorumWidget {
public:
	PictorumScrollBox(const TString& Name);
	virtual ~PictorumScrollBox();

	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	void AddScroll(const float& ScrollAmount);
	void SetScrollPosition(const float& ScrollPosition);

protected:
	float ScrollBarHeight;
	float ScrollBarThickness;
	float ScrollBarOffset;
	float ScrollSpeed;
	
	const float GetOffsetForChild(const int32& ChildIndex) const;
	const float GetScrollAlpha() const;

	virtual void OnMouseMove(const vec2& MousePosition, const vec2& MouseDelta, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseDown(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
	virtual void OnMouseScroll(const float Delta, FUserInterfaceEvent& EventIn) override;

	void PreventOverscroll();
private:
	float ScrollOffset;
	float ScrollEnergy;
	float ScrollDampening;
	bool bWasRightClickedIn;
	vec2 LastMouseDelta;
};

