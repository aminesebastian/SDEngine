#include "CheckboxWidget.h"

CheckboxWidget::CheckboxWidget(const TString& Name) : PictorumWidget(Name){
	bWatchedValue = nullptr;
	bChecked = false;
	BackgroundDrawInstruction.Size = Vector2D(16, 16);
	BackgroundDrawInstruction.BackgroundColor = FColor(0.8f);
	BackgroundDrawInstruction.BorderRadius = FBorderRadius(5.0f);

	CheckmarkDrawInstruction.Size = Vector2D(16, 16);
	CheckmarkDrawInstruction.Texture = Engine::GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CheckmarkIcon.sasset");
	CheckmarkDrawInstruction.Tint = FColor(0.2f);

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
CheckboxWidget:: ~CheckboxWidget() {}
void CheckboxWidget::Tick(float DeltaTime, const FRenderGeometry& Geometry) {
	if (bWatchedValue) {
		bChecked = *bWatchedValue;
	}
}
void CheckboxWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	BackgroundDrawInstruction.Location = Geometry.GetLocation();
	if (IsHovered()) {
		BackgroundDrawInstruction.BackgroundColor = FColor(0.75f);
	} else if(bChecked){
		BackgroundDrawInstruction.BackgroundColor = FColor(1.0f);
	} else {
		BackgroundDrawInstruction.BackgroundColor = FColor(0.85f);
	}
	DrawBox(Geometry, BackgroundDrawInstruction);
	if (bChecked) {
		CheckmarkDrawInstruction.Location = Geometry.GetLocation();
		DrawImage(Geometry, CheckmarkDrawInstruction);
	}
}
Vector2D CheckboxWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	return BackgroundDrawInstruction.Size;
}
void CheckboxWidget::Bind(bool* BoundTarget) {
	bWatchedValue = BoundTarget;
}
void CheckboxWidget::Unbind() {
	bWatchedValue = nullptr;
}
void CheckboxWidget::SetChcecked() {
	bChecked = true;
}
void CheckboxWidget::SetUnchecked() {
	bChecked = false;
}
const bool CheckboxWidget::GetChecked() const {
	return bChecked;
}
void CheckboxWidget::OnMouseUp(const vec2& MousePosition, const EMouseButton& Button, FUserInterfaceEvent& EventIn) {
	if (Button != EMouseButton::LEFT) {
		return;
	}
	bChecked = !bChecked;
	if (bWatchedValue) {
		*bWatchedValue = bChecked;
	}
}