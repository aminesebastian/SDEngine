#include "ImageWidget.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "UserInterface/Widgets/DragFloat.h"

ImageWidget::ImageWidget(const TString& Name) : PictorumWidget(Name) {
	quadVAO = 0;
	quadVBO = 0;
	BorderRadius = 0;
	Tint = FColor(0.5f, 0.5f, 0.5f, 1.0);
	bWasMouseDownInWidget = false;

	DragFloat* borderRadiusControl = new DragFloat("Border Radius");
	borderRadiusControl->AddEntry(&BorderRadius, "Radius %.3fpx", FColor(1.0f, 0.0f, 0.0f), 0.0f, 0.0f, 1.0f, 2.0f);
	DetailsPanelWidgets.Add(borderRadiusControl);

	GLfloat quadVertices[] = {
		0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
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

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
ImageWidget::~ImageWidget() {

}

void ImageWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	Shader* shader = EngineStatics::GetUIImageShader();
	shader->Bind();

	vec2 screenResolution      = Geometry.GetRenderResolution();
	vec2 adjustedScale         = Geometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
	float screenAspectRatio    = Geometry.GetRenderResolutionAspectRatio();
	float shapeAspectRatio     = Geometry.GetAllotedSpaceAspectRatio();
	float adjustedBorderRadius = BorderRadius / (glm::max(screenResolution.x, screenResolution.y));
	mat4 modelMatrix		   = CalculateModelMatrix(Geometry);

	shader->SetShaderMatrix4("MODEL_MATRIX", modelMatrix);
	shader->SetShaderVector2("RENDER_TARGET_RESOLUTION", screenResolution);
	shader->SetShaderVector2("SHAPE_SIZE", adjustedScale);
	shader->SetShaderFloat("SCREEN_ASPECT_RATIO", screenAspectRatio);
	shader->SetShaderFloat("SHAPE_ASPECT_RATIO", shapeAspectRatio);
	shader->SetShaderFloat("BORDER_RADIUS", adjustedBorderRadius);
	shader->SetShaderFloat("X_BORDER_RADIUS", adjustedBorderRadius / screenResolution.x);
	shader->SetShaderFloat("Y_BORDER_RADIUS", adjustedBorderRadius / screenResolution.y);
	shader->SetShaderVector4("TINT", Tint);

	Image->Bind("IMAGE", shader, 0);

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
vec2 ImageWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return Size;
}
void ImageWidget::SetImage(Texture2D* ImageIn, const bool& MaintainSize) {
	Image = ImageIn;
	if (!MaintainSize) {
		Size  = Image->GetDimensions();
	}
}
void ImageWidget::SetTint(const FColor& NewTint) {
	Tint = NewTint;
}
void ImageWidget::SetSize(const vec2& SizeIn) {
	Size = SizeIn;
}
void ImageWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {
	Tint += FColor(0.1f, 0.1f, 0.1f);
}
void ImageWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {
	Tint -= FColor(0.1f, 0.1f, 0.1f);
}
