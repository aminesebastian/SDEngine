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
	ScreenTransform.SetLocation(vec3(0.5f, 0.5f, 0.0f));
	ScreenTransform.SetScale(vec3(100.0f, 100.0f, 100.0f));

	DragFloat* location = new DragFloat("Location");
	location->AddEntry(&ScreenTransform.GetLocation().x, "X %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	location->AddEntry(&ScreenTransform.GetLocation().y, "Y %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	DetailsPanelWidgets.Add(location);

	DragFloat* scale = new DragFloat("Scale");
	scale->AddEntry(&ScreenTransform.GetScale().x, "Width %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	scale->AddEntry(&ScreenTransform.GetScale().y, "Height %.3fpx", FColor(0.0f, 1.0f, 0.0f), 0.0f, 0.0f, 0.1f, 2.0f);
	DetailsPanelWidgets.Add(scale);
}
BaseUIWidget::~BaseUIWidget() {

}
void BaseUIWidget::Draw() {
	EngineStatics::GetUIShader()->Bind();
	vec2 screenResolution = Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions();

	Transform tempTransform(ScreenTransform);
	tempTransform.SetLocation(vec3(ScreenTransform.GetLocation().x / screenResolution.x, ScreenTransform.GetLocation().y / screenResolution.y, 0.0f));
	tempTransform.SetScale(vec3(ScreenTransform.GetScale().x / screenResolution.x, ScreenTransform.GetScale().y / screenResolution.y, 1.0f));

	EngineStatics::GetUIShader()->SetShaderMatrix4("MODEL_MATRIX", tempTransform.GetModelMatrix());
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
	return true;
}