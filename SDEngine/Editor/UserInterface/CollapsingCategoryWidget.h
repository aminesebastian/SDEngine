#pragma once
#include "Core/Pictorum/Pictorum.h"
#include "Core/Objects/CoreTypes/Texture2D.h"

class CollapsingCategoryWidget : public PictorumWidget {
public:
	CollapsingCategoryWidget(const TString& Name);
	virtual ~CollapsingCategoryWidget();
	void AddToContainer(PictorumWidget* Widget);
	void SetCategoryLabel(const TString& LabelIn);
	PictorumVerticalBox* GetContainer() const;
	const TString& GetLabel() const;
protected:
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnDrawCompleted(const float& DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;
	virtual Vector2D GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;
	void CategoryClicked(PictorumWidget* Widget, const Vector2D& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& Event);
	const float GetHeaderHeight() const;
private:
	TextWidget* CategoryLabel;
	SolidWidget* CategoryBackground;
	PictorumVerticalBox* OverallContainer;
	PictorumVerticalBox* InternalContainer;
	Texture2D* CollapseImage;
	TString Label;
	bool bCollapsed;


	FImageDrawInstruction* CollapseIconDrawInstruction;
	FBoxDrawInstruction* SeparatorLine;
	float SeparatorLineHeight;
	FBoxDrawInstruction* ContentBackground;
};

