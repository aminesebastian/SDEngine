#include "SolidWidget.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "UserInterface/Widgets/DragFloat.h"

SolidWidget::SolidWidget(const TString& Name) : PictorumWidget(Name) {
	quadVAO = 0;
	quadVBO = 0;
	BorderRadius = 0;
	BackgroundColor = FColor(0.5f, 0.5f, 0.5f, 1.0);
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

	SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
}
SolidWidget::~SolidWidget() {

}

void SolidWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	Shader* shader = EngineStatics::GetUISolidShader();
	shader->Bind();

	vec2 screenResolution      = Geometry.GetRenderResolution();
	vec2 adjustedScale         = Geometry.GetAllotedSpace(EPictorumScaleBasis::RELATIVE);
	float screenAspectRatio    = Geometry.GetRenderResolutionAspectRatio();
	float shapeAspectRatio     = Geometry.GetAllotedSpaceAspectRatio();
	float adjustedBorderRadius = BorderRadius / (glm::max(screenResolution.x, screenResolution.y));
	mat4 modelMatrix		   = CalculateModelMatrix(Geometry);

	shader->SetShaderVector2("MIN_CLIP", Geometry.GetMinimumClipPoint(EPictorumLocationBasis::RELATIVE));
	shader->SetShaderVector2("MAX_CLIP", Geometry.GetMaximumClipPoint(EPictorumLocationBasis::ABSOLUTE));

	shader->SetShaderMatrix4("MODEL_MATRIX", modelMatrix);
	shader->SetShaderVector2("RENDER_TARGET_RESOLUTION", screenResolution);
	shader->SetShaderVector2("SHAPE_SIZE", adjustedScale);
	shader->SetShaderFloat("SCREEN_ASPECT_RATIO", screenAspectRatio);
	shader->SetShaderFloat("SHAPE_ASPECT_RATIO", shapeAspectRatio);
	shader->SetShaderFloat("BORDER_RADIUS", adjustedBorderRadius);
	shader->SetShaderFloat("X_BORDER_RADIUS", adjustedBorderRadius / screenResolution.x);
	shader->SetShaderFloat("Y_BORDER_RADIUS", adjustedBorderRadius / screenResolution.y);
	shader->SetShaderVector4("COLOR", BackgroundColor);

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
	glEnable(GL_DEPTH_TEST);
}
const bool SolidWidget::CanAddChild() const {
	return true;
}
void SolidWidget::CalculateChildRenderGeometry(const FRenderGeometry& CurrentRenderGeometry, FRenderGeometry& OutputGeometry, int32 ChildIndex) const {
	PictorumWidget::CalculateChildRenderGeometry(CurrentRenderGeometry, OutputGeometry, ChildIndex);
	Padding.ApplyToGeometry(OutputGeometry, OutputGeometry);
}
vec2 SolidWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	vec2 output = PictorumWidget::GetDesiredDrawSpace(Geometry);
	output.x += Padding.GetLeft() + Padding.GetRight();
	output.y += Padding.GetTop() + Padding.GetBottom();
	return output;
}

const FPadding& SolidWidget::GetPadding() const {
	return Padding;
}
void SolidWidget::SetPadding(const float& AllPadding) {
	Padding.SetTop(AllPadding);
	Padding.SetBottom(AllPadding);
	Padding.SetRight(AllPadding);
	Padding.SetLeft(AllPadding);
}
void SolidWidget::SetPadding(const float& TopBottomPadding, const float& RightLeftPadding) {
	Padding.SetTop(TopBottomPadding);
	Padding.SetBottom(TopBottomPadding);
	Padding.SetRight(RightLeftPadding);
	Padding.SetLeft(RightLeftPadding);
}
void SolidWidget::SetPadding(const float& Top, const float& Right, const float& Bottom, const float& Left) {
	Padding.SetTop(Top);
	Padding.SetBottom(Bottom);
	Padding.SetRight(Right);
	Padding.SetLeft(Left);
}
void SolidWidget::SetBackgroundColor(const FColor& NewColor) {
	BackgroundColor = NewColor;
}
void SolidWidget::SetBorderRadius(const float& Radius) {
	BorderRadius = Radius;
}

void SolidWidget::OnMouseEnter(vec2 MousePosition, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseEnter(MousePosition, Event);
	BackgroundColor += FColor(0.1f, 0.1f, 0.1f);
}
void SolidWidget::OnMouseExit(vec2 MousePosition, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseExit(MousePosition, Event);
	BackgroundColor -= FColor(0.1f, 0.1f, 0.1f);
}
void SolidWidget::OnMouseDown(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseDown(MousePosition, Button, Event);
	Event.CaptureMouse();
	bWasMouseDownInWidget = true;
}
void SolidWidget::OnMouseUp(vec2 MousePosition, EMouseButton Button, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseUp(MousePosition, Button, Event);
	bWasMouseDownInWidget = false;
}
void SolidWidget::OnMouseMove(vec2 MousePosition, vec2 MouseDelta, FUserInterfaceEvent& Event) {
	PictorumWidget::OnMouseMove(MousePosition, MouseDelta, Event);
}
bool SolidWidget::PopulateDetailsPanel() {
	PictorumWidget::PopulateDetailsPanel();

	//ImGui::Text("Color");
	//ImGui::ColorEdit4("Color", &BackgroundColor[0]);

	return true;
}