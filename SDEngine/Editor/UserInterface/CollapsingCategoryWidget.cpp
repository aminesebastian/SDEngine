#include "CollapsingCategoryWidget.h"
#include "Editor/UserInterface/EngineUIStyle.h"

CollapsingCategoryWidget::CollapsingCategoryWidget(const TString& Name) : PictorumWidget(Name) {
	CollapseImage                        = Engine::GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/CategoryTwirlIcon.sasset");
	CollapseIconDrawInstruction          = new FImageDrawInstruction();
	CollapseIconDrawInstruction->Tint    = EngineUIStyles::DIM_ICON_TINT;
	CollapseIconDrawInstruction->Texture = CollapseImage;
	CollapseIconDrawInstruction->Size    = Vector2D(12.0f, 12.0f);
	bCollapsed                           = false;

	SeparatorLineHeight = 1.0f;
	SeparatorLine = new FBoxDrawInstruction();
	SeparatorLine->BackgroundColor = EngineUIStyles::SEPARATOR_COLOR;

	ContentBackground = new FBoxDrawInstruction();
	ContentBackground->BackgroundColor = EngineUIStyles::LIGHT_BACKGROUND_COLOR;
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
	CategoryLabel->SetText("ERROR");

	separator->SetSize(0.0f, 5.0f);

	CategoryBackground->GetParentSlot<VerticalBoxSlot>()->SetHorizontalAlignment(EHorizontalAlignment::STRETCH);
	CategoryBackground->SetPadding(8.0f, 0.0f, 8.0f, 20.0f);
	if (bCollapsed) {
		CategoryBackground->SetBackgroundColor(EngineUIStyles::BACKGROUND_COLOR);
	} else {
		CategoryBackground->SetBackgroundColor(EngineUIStyles::LIGHT_BACKGROUND_COLOR);
	}

	CategoryBackground->OnMouseUpDelegate.Add<CollapsingCategoryWidget, & CollapsingCategoryWidget::CategoryClicked>(this);
	CategoryBackground->SetVisibility(EPictorumVisibilityState::VISIBLE);
	AssignNewToChild(OverallContainer, PictorumVerticalBox, InternalContainer, "CategoryBackground");
	InternalContainer->SetPadding(0.0f, 8.0f, 2.0f, 8.0f);
	AssignNewToChild(OverallContainer, SeparatorWidget, separator, "BottomSeparator");
	separator->SetSize(0.0f, 5.0f);
}
void CollapsingCategoryWidget::Draw(float DeltaTime, const FRenderGeometry& Geometry) {
	if (!bCollapsed) {
		ContentBackground->Location = Geometry.GetLocation();
		ContentBackground->Size = Geometry.GetAllotedSpace();
		DrawBox(Geometry, *ContentBackground);
	}
}
void CollapsingCategoryWidget::PostChildrenDraw(float DeltaTime, const FRenderGeometry& Geometry) {
	CollapseIconDrawInstruction->Location.x = Geometry.GetLocation().x + 4.0f;
	CollapseIconDrawInstruction->Location.y = Geometry.GetTopLeft().y - CollapseIconDrawInstruction->Size.y - GetHeaderHeight() / 4.0f - 2.0f;
	DrawImage(Geometry, *CollapseIconDrawInstruction);

	SeparatorLine->Location = Vector2D( Geometry.GetLocation());
	SeparatorLine->Size = Vector2D(Geometry.GetAllotedSpace().x, SeparatorLineHeight);
	DrawBox(Geometry, *SeparatorLine);
}
Vector2D CollapsingCategoryWidget::GetDesiredDrawSpace(const FRenderGeometry& Geometry) const {
	if (bCollapsed) {
		return Vector2D(Geometry.GetAllotedSpace().x, GetHeaderHeight());
	} else {
		return PictorumWidget::GetDesiredDrawSpace(Geometry);
	}
}
void CollapsingCategoryWidget::SetCategoryLabel(const TString& LabelIn) {
	Label = LabelIn;
	CategoryLabel->SetText(Label);
}
const TString& CollapsingCategoryWidget::GetLabel() const {
	return Label;
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
		CategoryBackground->SetBackgroundColor(EngineUIStyles::BACKGROUND_COLOR);
	} else {
		InternalContainer->SetVisibility(EPictorumVisibilityState::SELF_HIT_TEST_INVISIBLE);
		CategoryBackground->SetBackgroundColor(EngineUIStyles::LIGHT_BACKGROUND_COLOR);
	}
}
const float CollapsingCategoryWidget::GetHeaderHeight() const {
	return CategoryBackground->GetDesiredDrawSpace(LastRenderedGeometry).y;
}