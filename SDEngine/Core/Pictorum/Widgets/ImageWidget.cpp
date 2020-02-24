#include "ImageWidget.h"
#include "Core/Input/InputSubsystem.h"
#include "Core/Engine/Engine.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Shader.h"

ImageWidget::ImageWidget(const TString& Name) : PictorumWidget(Name) {
	DrawInstruction        = new FImageDrawInstruction();
	DrawInstruction->Tint  = FColor(1.0f, 1.0f, 1.0f, 1.0);
	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
ImageWidget::~ImageWidget() {

}

void ImageWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	DrawInstruction->Location = Geometry.GetLocation();
	DrawInstruction->Size = Geometry.GetAllotedSpace();
	DrawImage(Geometry, *DrawInstruction);
}
vec2 ImageWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return Size;
}
void ImageWidget::SetImage(Texture2D* ImageIn, const bool& UpdateSize) {
	DrawInstruction->Texture = ImageIn;
	if (UpdateSize) {
		SetSize(ImageIn->GetDimensions());
	}
}
void ImageWidget::SetTint(const FColor& NewTint) {
	DrawInstruction->Tint = NewTint;
}
void ImageWidget::SetSize(const vec2& SizeIn) {
	Size = SizeIn;
}
void ImageWidget::OnMouseEnter(const vec2& MousePosition, FUserInterfaceEvent& Event) {
	DrawInstruction->Tint += FColor(0.1f, 0.1f, 0.1f);
}
void ImageWidget::OnMouseExit(const vec2& MousePosition, FUserInterfaceEvent& Event) {
	DrawInstruction->Tint -= FColor(0.1f, 0.1f, 0.1f);
}
