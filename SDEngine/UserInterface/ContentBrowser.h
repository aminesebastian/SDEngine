#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "UserInterface/Widgets/BaseWidget.h"

class ContentBrowser : public BaseWidget {
public:
	ContentBrowser();
	~ContentBrowser();
	virtual bool Draw() override;
};

