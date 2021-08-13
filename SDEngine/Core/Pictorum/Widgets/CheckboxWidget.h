#pragma once
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Pictorum/PictorumWidget.h"

class CheckboxWidget : public PictorumWidget {
public:
	CheckboxWidget(const TString& Name);
	virtual  ~CheckboxWidget();
	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual Vector2D GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	void Bind(const bool* BoundTarget);
	void Unbind();
	void SetChcecked();
	void SetUnchecked();
	const bool GetChecked() const;

protected:
	void OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) override;
private:
	bool* bWatchedValue;
	bool bChecked;
	FImageDrawInstruction CheckmarkDrawInstruction;
	FBoxDrawInstruction BackgroundDrawInstruction;
};



