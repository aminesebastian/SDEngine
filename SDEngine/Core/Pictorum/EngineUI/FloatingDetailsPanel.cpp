#include "FloatingDetailsPanel.h"
#include "Core/Engine/Window.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/Widgets/TextWidget.h"
#include "Core/Pictorum/Widgets/FloatEditWidget.h"
#include "Core/Pictorum/Widgets/SolidWidget.h"
#include "Core/Pictorum/Widgets/VerticalBoxWidget.h"
#include "Core/Pictorum/Widgets/LayoutWidget.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Utilities/StringUtilities.h"

FloatingDetailsPanel::FloatingDetailsPanel(const TString& Name) : PictorumWidget(Name) {

}
FloatingDetailsPanel::~FloatingDetailsPanel() {

}

void FloatingDetailsPanel::OnCreated() {
	SolidWidget* root = new SolidWidget("Background");
	AddChild(root);

	VerticalBoxWidget* vb = new VerticalBoxWidget("DetailsPanelWidgetsContainer");
	root->AddChild(vb);

	for (int i = 0; i < 10; i++) {
		TextWidget* label = new TextWidget("EditWidget" + to_string(i) + "Label");
		label->SetText("Position");
		label->SetFontSize(10);
		vb->AddChild(label);
		FloatEditWidget* editWidget = new FloatEditWidget("EditWidget" + to_string(i));
		vb->AddChild(editWidget);
	}
}
