#include "FloatingDetailsPanel.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/FloatEditWidget.h"
#include "Core/Pictorum/Widgets/VerticalBoxWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"

FloatingDetailsPanel::FloatingDetailsPanel(const TString& Name) : PictorumWidget(Name) {

}
FloatingDetailsPanel::~FloatingDetailsPanel() {

}

void FloatingDetailsPanel::OnCreated() {
	VerticalBoxWidget* root = new VerticalBoxWidget("MainContainer");
	AddChild(root);

	for (int i = 0; i < 10; i++) {
		TextWidget* label = new TextWidget("EditWidget" + to_string(i) + "Label");
		label->SetText("Position");
		label->SetFontSize(10);
		root->AddChild(label);
		FloatEditWidget* editWidget = new FloatEditWidget("EditWidget" + to_string(i));
		root->AddChild(editWidget);
	}
}
