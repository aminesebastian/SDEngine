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

FloatingDetailsPanel::FloatingDetailsPanel(const TString& Name) : PictorumWidget(Name) {

}
FloatingDetailsPanel::~FloatingDetailsPanel() {

}

void FloatingDetailsPanel::OnCreated() {
	SolidWidget* root = new SolidWidget("Background");
	root->SetBackgroundColor(EngineUIStyles::DARK_BACKGROUND_COLOR);
	root->SetPadding(8.0f);
	AddChild(root);

	PictorumVerticalBox* vb = new PictorumVerticalBox("DetailsPanelWidgetsContainer");
	root->AddChild(vb);

	for (int i = 0; i < 5; i++) {
		if (i > 0) {
			SeparatorWidget* sep = new SeparatorWidget("Sep");
			vb->AddChild(sep);
			sep->SetSize(0.0f, 5.0f);
		}
		PictorumHorizontalBox* hbox = new PictorumHorizontalBox("EditWidgetHBox" + to_string(i) + "Label");
		vb->AddChild(hbox);

		TextWidget* label = new TextWidget("EditWidget" + to_string(i) + "Label");
		hbox->AddChild(label);
		label->SetText("Position  ");
		label->SetFontSize(10);

		FloatEditWidget* editWidget = new FloatEditWidget("EditWidget" + to_string(i));
		hbox->AddChild(editWidget);
	}
}
