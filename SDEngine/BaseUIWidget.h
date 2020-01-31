#pragma once
#include "Core/DataStructures/Array.h"
#include <GLEW/glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "Utilities/Transform.h"
#include "UserInterface/DetailsPanelProvider.h"

class Shader;
class BaseWidget;

class BaseUIWidget : public IDetailsPanelProvider {
public:
	BaseUIWidget();
	~BaseUIWidget();
	void Draw();
	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

protected:
	inline mat4 CalculateModelMatrix(vec2 ScreenDimensions) const {
		mat4 posMatrix = glm::translate(vec3(Location.x/ScreenDimensions.x, Location.y/ScreenDimensions.y, 0.0f));
		mat4 scaleMatrix = glm::scale(vec3(Scale.x/ScreenDimensions.x, Scale.y/ScreenDimensions.y, 1.0f));
		mat4 rotXMatrix = glm::rotate(0.0f, vec3(1, 0, 0));
		mat4 rotYMatrix = glm::rotate(0.0f, vec3(0, 1, 0));
		mat4 rotZMatrix = glm::rotate(Rotation, vec3(0, 0, 1));


		mat4 combinedRotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * combinedRotMatrix * scaleMatrix;
	}
private:
	vec2 Location;
	vec2 Scale;
	float Rotation;

	SArray<BaseWidget*> DetailsPanelWidgets;
	float BorderRadius;
	FColor BackgroundColor;

	GLuint quadVAO = 0;
	GLuint quadVBO;
};

