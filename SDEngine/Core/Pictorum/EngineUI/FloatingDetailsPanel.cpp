#include "FloatingDetailsPanel.h"
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

FloatingDetailsPanel::FloatingDetailsPanel(const TString& Name) : PictorumWidget(Name) {
	DetailsPanelListBox = nullptr;
	DisplayedEntity = nullptr;
}
FloatingDetailsPanel::~FloatingDetailsPanel() {

}

void FloatingDetailsPanel::OnCreated() {
	AssignNewChildLocal(SolidWidget, root, "DetailsPanelBG")
		root->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	root->SetPadding(8.0f);

	AssignNewToChild(root, PictorumVerticalBox, DetailsPanelListBox, "ControlsContainer")
}
void FloatingDetailsPanel::SetSelectedEntity(Entity* SelectedEntity) {
	DetailsPanelListBox->ClearChildren();

	if (!SelectedEntity) {
		return;
	}

	SArray<TypeDescriptor_Class::Member> members = ReflectionHelpers::GetAllMembersOfClass(SelectedEntity);
	for (const TypeDescriptor_Class::Member& member : members) {
		if (DetailsPanelListBox->GetChildCount() > 0) {
			SeparatorWidget* sep = new SeparatorWidget("Separator");
			DetailsPanelListBox->AddChild(sep);
			sep->SetSize(0.0f, 5.0f);
		}



		AssignNewToChildLocal(DetailsPanelListBox, TextWidget, label, (TString(member.Name) + "EditControl"));
		TString contents = member.Name + TString(": ");

		if (member.Type == TypeResolver<bool>::Get()) {
			bool result = *(bool*)((char*)SelectedEntity + member.Offset);
			if (result) {
				contents += ": True";
			} else {
				contents += ": False";
			}	
		}
		if (member.Type->Name == TypeResolver<TString>::Get()->Name) {
			contents += *(TString*)((char*)SelectedEntity + member.Offset);
		}
		if (member.Type->Name == TypeResolver<int32>::Get()->Name) {
			contents += to_string(*(int32*)((char*)SelectedEntity + member.Offset));
		}
		if (member.Type->Name == TypeResolver<float>::Get()->Name) {
			contents += to_string(*(float*)((char*)SelectedEntity + member.Offset));
		}
		if (member.Type->Name == "Transform") {
			TypeDescriptor_Struct* transformType = Cast<TypeDescriptor_Struct>(member.Type);
			Transform* transform = (Transform*)((char*)SelectedEntity + member.Offset);
			vector<TypeDescriptor_Struct::Member> transformMembers = transformType->Members;
			for (TypeDescriptor_Struct::Member transMem : transformMembers) {
				AssignNewToChildLocal(DetailsPanelListBox, FloatEditWidget, floatEditor, (TString(member.Name) + "FloatEditWidget"));
				Vector3D vec = *(Vector3D*)((char*)transform + transMem.Offset);
				floatEditor->SetControlledValue(&vec[0], 3);
			}
		}
		label->SetText(contents);
		label->SetFontSize(10);
	}
}
