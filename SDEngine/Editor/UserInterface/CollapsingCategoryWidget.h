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

protected:
	virtual void PostChildrenDraw(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void OnCreated() override;
	void CategoryClicked(PictorumWidget* Widget, const Vector2D& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& Event);
private:
	TextWidget* CategoryLabel;
	SolidWidget* CategoryBackground;
	PictorumVerticalBox* OverallContainer;
	PictorumVerticalBox* InternalContainer;
	Texture2D* CollapseImage;
	FImageDrawInstruction* CollapseIconDrawInstruction;
	TString Label;
	bool bCollapsed;
};

