#pragma once
#include "Core/DataStructures/Array.h"
#include "Core/Input/InputUtilities.h"
#include <GLEW/glew.h>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "Utilities/Transform.h"
#include "UserInterface/DetailsPanelProvider.h"

class Shader;
class DragFloat;

struct FUserInterfaceEvent {
	FUserInterfaceEvent() {
		bHandled      = false;
		bCaptureMouse = false;
	}

	bool GetHandled() { return bHandled; }
	void SetHandled() { bHandled = true; }
	void SetUnhandled() { bHandled = false; }

	bool ShouldCaptureMouse() { return bCaptureMouse; }
	void CaptureMouse() { bCaptureMouse = true; }
private:
	bool bHandled;
	bool bCaptureMouse;
};

class PictorumWidget : public IDetailsPanelProvider {
public:
	PictorumWidget();
	~PictorumWidget();

	virtual void Tick(float DeltaTime);
	virtual void Draw(float DeltaTime);
	virtual void CalculateBounds(vec2 RenderTargetResolution, vec2& MinBounds, vec2& MaxBounds);

	virtual void OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event);
	virtual void OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event);
	virtual void OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);
	virtual void OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event);

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;

protected:
	mat4 CalculateModelMatrix(const vec2& ScreenDimensions) const;
private:
	vec2 Location;
	vec2 Scale;
	float Rotation;
	bool IsMouseDown;

	SArray<DragFloat*> DetailsPanelWidgets;
	float BorderRadius;
	FColor BackgroundColor;

	GLuint quadVAO = 0;
	GLuint quadVBO;
};

