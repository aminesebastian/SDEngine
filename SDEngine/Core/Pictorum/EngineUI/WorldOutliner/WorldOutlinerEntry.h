#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Pictorum.h"
#include "Core/Objects/Entities/Entity.h"

class WorldOutlinerEntry : public PictorumWidget {
public:
	WorldOutlinerEntry(const TString& Name);
	virtual ~WorldOutlinerEntry();

	virtual void OnCreated() override;
	void Initialize(Entity* EntityIn);

private:
	TextWidget* EntityNameWidget;
	TextWidget* EntityTypeWidget;
	ImageWidget* VisibilityWidget;
	Texture2D* VisibleIcon;
	Texture2D* HiddenIcon;
};

