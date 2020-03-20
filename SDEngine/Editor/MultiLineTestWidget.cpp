#include "MultiLineTestWidget.h"

MultiLineTestWidget::MultiLineTestWidget(const TString& Name) : PictorumWidget(Name) {

}
void MultiLineTestWidget::OnCreated() {
	AssignNewChildLocal(SolidWidget, bg, "Background");
	bg->SetBackgroundColor(FColor(0.2f));

	AssignNewToChildLocal(bg, EditableTextWidget, testText, "TestTextInput");
	testText->SetFontSize(16);
	testText->SetText("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.");
}