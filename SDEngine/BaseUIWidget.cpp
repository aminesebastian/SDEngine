#include "BaseUIWidget.h"
#include "Engine/EngineStatics.h"
#include "Engine/Engine.h"
#include "Engine/RenderViewport.h"
#include "Rendering/DefferedCompositor.h"
#include "Rendering/Shader.h"
#include "UserInterface/UserInterface.h"
#include "UserInterface/UserInterfaceUtilities.h"
#include "UserInterface/Widgets/DragFloat.h"

BaseUIWidget::BaseUIWidget() {
	quadVAO = 0;
	quadVBO = 0;
	Location = vec2(0.5f, 0.5f);
	Scale = vec2(100.0f, 100.0f);
	BorderRadius = 0;
	BackgroundColor = FColor(1.0f, 1.0f, 1.0f, 1.0);
	Rotation = 0.0f;

	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&Location.x, "X %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	location->AddEntry(&Location.y, "Y %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(location);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&Scale.x, "Width %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	scale->AddEntry(&Scale.y, "Height %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(scale);

	DragFloat* borderRadiusControl = new DragFloat("Border Radius");
	borderRadiusControl->AddEntry(&BorderRadius, "Radius %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(borderRadiusControl);

	std::function<float(float)> toDegrees = ([](float radians) { return glm::degrees(radians); });
	std::function<float(float)> toRadians = ([](float degrees) { return glm::radians(degrees); });

	DragFloat* rotation = new DragFloat("Angle");
	FDragFloatEntry* z = rotation->AddEntry(&Rotation, "%.3f", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	z->OnFormatForViewFunction = toDegrees;
	z->OnFormatFromView = toRadians;

	DetailsPanelWidgets.Add(rotation);
}
BaseUIWidget::~BaseUIWidget() {

}
void BaseUIWidget::Draw() {
	EngineStatics::GetUIShader()->Bind();
	vec2 screenResolution = Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions();
	float screenAspectRatio = screenResolution.x / screenResolution.y;
	float shapeAspectRatio = Scale.x / Scale.y;
	float adjustedBorderRadius = BorderRadius * shapeAspectRatio;

	EngineStatics::GetUIShader()->SetShaderMatrix4("MODEL_MATRIX", CalculateModelMatrix(screenResolution));
	EngineStatics::GetUIShader()->SetShaderVector2("RENDER_TARGET_RESOLUTION", screenResolution);
	EngineStatics::GetUIShader()->SetShaderVector2("SHAPE_SIZE", Scale);
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
TString BaseUIWidget::GetDetailsPanelName() {
	return "UI Widget";
}
bool BaseUIWidget::PopulateDetailsPanel() {
	ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen;
	if (ImGui::CollapsingHeader("Transform", flags)) {
		for (BaseWidget* widget : DetailsPanelWidgets) {
			widget->Draw();
		}
	}

	ImGui::Text("Color");
	ImGui::ColorEdit4("Color", &BackgroundColor[0]);

	return true;
}