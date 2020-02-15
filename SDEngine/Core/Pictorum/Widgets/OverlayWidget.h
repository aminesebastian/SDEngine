#pragma once
#include "Core/Pictorum/PictorumWidget.h"
class OverlayWidget : public PictorumWidget {
public:
	OverlayWidget(const TString& Name);
	virtual ~OverlayWidget();

	virtual const bool CanAddChild() const override;
private:
	FPadding Padding;
};

