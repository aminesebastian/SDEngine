#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class SeparatorWidget : public PictorumWidget {
public:
	SeparatorWidget(const TString& Name);
	virtual ~SeparatorWidget();
	virtual vec2 GetDesiredDrawSpace(const FRenderGeometry& Geometry) const override;

	void SetSize(const float& Width, const float& Height);
	const vec2& GetSize() const;
protected:
	vec2 Size;
};

