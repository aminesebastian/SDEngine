#include "CollapsingCategoryWidget.h"
#include "Editor/UserInterface/EngineUIStyle.h"

CollapsingCategoryWidget::CollapsingCategoryWidget(const TString& Name) : PictorumWidget(Name) {
	CollapseImage                        = Engine::GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CategoryTwirlIcon.sasset");
	CollapseIconDrawInstruction          = new FImageDrawInstruction();
	CollapseIconDrawInstruction->Texture = CollapseImage;
	CollapseIconDrawInstruction->Size    = Vector2D(16.0f, 16.0f);
	bCollapsed                           = false;
}
CollapsingCategoryWidget::~CollapsingCategoryWidget() {
	delete CollapseIconDrawInstruction;
}
void CollapsingCategoryWidget::OnCreated() {
	AssignNewChild(PictorumVerticalBox, OverallContainer, "OverallContainer");
	AssignNewToChild(OverallContainer, SolidWidget, CategoryBackground, "CategoryBackground");
	AssignNewToChildLocal(OverallContainer, SeparatorWidget, separator, "Separator");
	AssignNewToChild(CategoryBackground, TextWidget, CategoryLabel, "CategoryLabel");
	CategoryLabel->SetFontSize(10);

	separator->SetSize(0.0f, 5.0f);

	CategoryBackground->GetParentSlot<VerticalBoxSlot>()->SetHorizontalAlignment(EHorizontalAlignment::STRETCH);
	CategoryBackground->SetPadding(2.0f, 0.0f, 2.0f, 20.0f);
	CategoryBackground->SetBackgroundColor(EngineUIStyles::BACKGROUND_COLOR);
	CategoryBackground->OnMouseUpDelegate.Add<CollapsingCategoryWidget, & CollapsingCategoryWidget::CategoryClicked>(this);
	CategoryBackground->SetVisibility(EPictorumVisibilityState::VISIBLE);
	AssignNewToChild(OverallContainer, PictorumVerticalBox, InternalContainer, "CategoryBackground");
}
void CollapsingCategoryWidget::PostChildrenDraw(float DeltaTime, const FRenderGeometry& Geometry) {
	CollapseIconDrawInstruction->Location.x = Geometry.GetLocation().x;
	CollapseIconDrawInstruction->Location.y = Geometry.GetLocation().y + Geometry.GetAllotedSpace().y - CollapseIconDrawInstruction->Size.y;

	DrawImage(Geometry, *CollapseIconDrawInstruction);
}
void CollapsingCategoryWidget::SetCategoryLabel(const TString& LabelIn) {
	Label = LabelIn;
	CategoryLabel->SetText(Label);
}
void CollapsingCategoryWidget::AddToContainer(PictorumWidget* Widget) {
	InternalContainer->AddChild(Widget);
}
PictorumVerticalBox* CollapsingCategoryWidget::GetContainer() const {
	return InternalContainer;
}
void CollapsingCategoryWidget::CategoryClicked(PictorumWidget* Widget, const Vector2D& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& Event) {
	bCollapsed = !bCollapsed;
	if (bCollapsed) {
		InternalContainer->SetVisibility(EPictorumVisibilityState::COLLAPSED);
	} else {
		InternalContainer->SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
	}
}