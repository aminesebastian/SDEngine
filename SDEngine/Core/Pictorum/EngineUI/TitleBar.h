#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class Texture2D;

class TitleBar : public PictorumWidget {
public:
	TitleBar(const TString& Name);
	virtual ~TitleBar();

	virtual bool CanAddChild() const override;
	virtual void CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const override;

private:
	Texture2D* CloseTexture;
	Texture2D* MinimizeTexture;
	Texture2D* RestoreTexture;
	Texture2D* MaximizeTexture;
};

