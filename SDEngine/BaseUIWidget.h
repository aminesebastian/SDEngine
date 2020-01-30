#pragma once
#include <GLEW/glew.h>
#include "Utilities/Transform.h"
#include "UserInterface/DetailsPanelProvider.h"
#include "Core/DataStructures/Array.h"

class Shader;
class BaseWidget;

class BaseUIWidget : public IDetailsPanelProvider {
public:
	BaseUIWidget();
	~BaseUIWidget();
	void Draw();
	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;
private:
	Transform ScreenTransform;
	SArray<BaseWidget*> DetailsPanelWidgets;

	GLuint quadVAO = 0;
	GLuint quadVBO;
};

