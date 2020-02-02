#include "SolidWidget.h"
#include "Core/Input/InputSubsystem.h"
#include "Engine/Engine.h"
#include "Engine/EngineStatics.h"
#include "Rendering/Shader.h"
#include "UserInterface/Widgets/DragFloat.h"

SolidWidget::SolidWidget(TString Name) : PictorumWidget(Name) {
	quadVAO = 0;
	quadVBO = 0;
	BorderRadius = 0;
	BackgroundColor = FColor(0.5f, 0.5f, 0.5f, 1.0);
	bWasMouseDownInWidget = false;

	DragFloat* borderRadiusControl = new DragFloat("Border Radius");
	borderRadiusControl->AddEntry(&BorderRadius, "Radius %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(borderRadiusControl);
}
SolidWidget::~SolidWidget() {

}

void SolidWidget::Draw(float DeltaTime, FRenderGeometry Geometry) {
	EngineStatics::GetUIShader()->Bind();
	vec2 screenResolution = Geometry.RenderResolution;
	float screenAspectRatio = screenResolution.x / screenResolution.y;

	vec2 adjustedScale = GetAbsoluteLayoutScale();

	float shapeAspectRatio = adjustedScale.x / adjustedScale.y;
	float adjustedBorderRadius = BorderRadius * shapeAspectRatio;

	EngineStatics::GetUIShader()->SetShaderMatrix4("MODEL_MATRIX", CalculateModelMatrix(screenResolution));
	EngineStatics::GetUIShader()->SetShaderVector2("RENDER_TARGET_RESOLUTION", screenResolution);
	EngineStatics::GetUIShader()->SetShaderVector2("SHAPE_SIZE", adjustedScale);
	EngineStatics::GetUIShader()->SetShaderFloat("SCREEN_ASPECT_RATIO", screenAspectRatio);
	EngineStatics::GetUIShader()->SetShaderFloat("SHAPE_ASPECT_RATIO", shapeAspectRatio);
	EngineStatics::GetUIShader()->SetShaderFloat("BORDER_RADIUS", BorderRadius);
	EngineStatics::GetUIShader()->SetShaderFloat("X_BORDER_RADIUS", adjustedBorderRadius / screenResolution.x);
	EngineStatics::GetUIShader()->SetShaderFloat("Y_BORDER_RADIUS", adjustedBorderRadius / screenResolution.y);
	EngineStatics::GetUIShader()->SetShaderVector4("COLOR", BackgroundColor);

	//Engine::GetInstance()->GetFocusedViewport()->GetDefferedCompositor()->DrawScreenQuad();
	if (quadVAO == 0) {
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
void SolidWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {
	BackgroundColor += FColor(0.1f, 0.1f, 0.1f);
}
void SolidWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {
	BackgroundColor -= FColor(0.1f, 0.1f, 0.1f);
}
void SolidWidget::OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	Event.CaptureMouse();
	bWasMouseDownInWidget = true;
}
void SolidWidget::OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	bWasMouseDownInWidget = false;
}
void SolidWidget::OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) {
	if (bWasMouseDownInWidget) {
		Location += MouseDelta;
	}
}
bool SolidWidget::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	ImGui::Text("Color");
	ImGui::ColorEdit4("Color", &BackgroundColor[0]);

	return true;
}