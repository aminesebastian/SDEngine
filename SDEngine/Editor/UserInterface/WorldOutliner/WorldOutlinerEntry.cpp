#include "WorldOutlinerEntry.h"
#include "Editor/UserInterface/EngineUIStyle.h"

WorldOutlinerEntry::WorldOutlinerEntry(const TString& Name) : PictorumWidget(Name) {
	VisibilityWidget = nullptr;
	EntityNameWidget = nullptr;
	EntityTypeWidget = nullptr;
	VisibleIcon      = Engine::GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/EyeOpenIcon.sasset");
	HiddenIcon       = Engine::GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/UI/EyeClosedIcon.sasset");

	SetVisibility(EPictorumVisibilityState::VISIBLE);
}
WorldOutlinerEntry::~WorldOutlinerEntry() {

}

void WorldOutlinerEntry::OnCreated() {
	AssignNewChildLocal(PictorumHorizontalBox, hBox, "EntryHBox");
	//hBox->SetPadding(8.0f);

	AssignNewToChild(hBox, ImageWidget, VisibilityWidget, "VisbilityIcon");
	AssignNewToChild(hBox, TextWidget, EntityNameWidget, "EntityName");
	AssignNewToChild(hBox, TextWidget, EntityTypeWidget, "EntityType");

	EntityNameWidget->SetFontSize(11);
	EntityNameWidget->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f);
	EntityTypeWidget->SetFontSize(11);
	EntityTypeWidget->GetParentSlot<HorizontalBoxSlot>()->SetFillAvilableSpace(0.5f);

	VisibilityWidget->SetSize(Vector2D(16.0f, 16.0f));
	VisibilityWidget->SetTint(EngineUIStyles::DIM_ICON_TINT);
}
void WorldOutlinerEntry::Initialize(Entity* EntityIn) {
	EntityNameWidget->SetText(EntityIn->GetObjectName());
	EntityTypeWidget->SetText(EntityIn->StaticDescriptor()->Name);
	VisibilityWidget->SetImage(VisibleIcon);
}