#include "EntityInspector.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/EngineUI/EngineUIStyle.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/FloatEditWidget.h"
#include "Core/Pictorum/Containers/PictorumCanvas.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/SeparatorWidget.h"
#include "Core/Pictorum/Containers/PictorumHorizontalBox.h"
#include "Core/Pictorum/Containers/PictorumVerticalBox.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"
#include "Core/Reflection/ReflectionHelpers.h"
#include "Test/ReflectionTest.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Utilities/Logger.h"

EntityInspector::EntityInspector(const TString& Name) : PictorumWidget(Name) {
	DetailsPanelListBox = nullptr;
	DisplayedEntity = nullptr;
}
EntityInspector::~EntityInspector() {

}

void EntityInspector::OnCreated() {
	AssignNewChildLocal(SolidWidget, root, "DetailsPanelBG")
		root->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	root->SetPadding(8.0f);

	AssignNewToChild(root, PictorumVerticalBox, DetailsPanelListBox, "ControlsContainer")
}
void EntityInspector::SetSelectedEntity(Entity* SelectedEntity) {
	DetailsPanelListBox->ClearChildren();

	if (!SelectedEntity) {
		return;
	}

	SArray<FProperty> properties;
	ReflectionHelpers::GetAllMembersOfClass(properties, SelectedEntity);
	for (const FProperty& member : properties) {
		if (member.bInspectorHidden) {
			continue;
		}
		if (DetailsPanelListBox->GetChildCount() > 0) {
			SeparatorWidget* sep = new SeparatorWidget("Separator");
			DetailsPanelListBox->AddChild(sep);
			sep->SetSize(0.0f, 5.0f);
		}

		AssignNewToChildLocal(DetailsPanelListBox, TextWidget, label, (TString(member.Name) + "EditControl"));
		TString contents = member.InspectorName + TString(": ");

		if (member.Type == TypeResolver<bool>::Get()) {
			bool result = *ReflectionHelpers::GetProperty<bool>(member, SelectedEntity);
			if (result) {
				contents += ": True";
			} else {
				contents += ": False";
			}	
		}
		if (member.Type->Name == TypeResolver<TString>::Get()->Name) {
			contents += *ReflectionHelpers::GetProperty<TString>(member, SelectedEntity);
		}
		if (member.Type->Name == TypeResolver<int32>::Get()->Name) {
			contents += to_string(*ReflectionHelpers::GetProperty<int32>(member, SelectedEntity));
		}
		if (member.Type->Name == TypeResolver<float>::Get()->Name) {
			contents += to_string(*ReflectionHelpers::GetProperty<float>(member, SelectedEntity));
		}
		if (member.Type->Name == "Transform") {
			Transform* transform = ReflectionHelpers::GetProperty<Transform>(member.Name, SelectedEntity);
			TypeDescriptor_Struct* transformType = Cast<TypeDescriptor_Struct>(member.Type);
			vector<FProperty>& transformMembers = transformType->Properties;
			for (FProperty& transMem : transformMembers) {
				AssignNewToChildLocal(DetailsPanelListBox, FloatEditWidget, floatEditor, (TString(member.Name) + "FloatEditWidget"));
				Vector3D* vec = ReflectionHelpers::GetProperty<Vector3D>(transMem, transform);
				floatEditor->SetControlledValue(&(*vec)[0], 3);
			}
		}
		label->SetText(contents);
		label->SetFontSize(10);
	}
}
