#include "MultiLineTestWidget.h"

MultiLineTestWidget::MultiLineTestWidget(const TString& Name) : PictorumWidget(Name) {

}
void MultiLineTestWidget::OnCreated() {
	AssignNewChildLocal(SolidWidget, bg, "Background");
	bg->SetBackgroundColor(FColor(0.2f));

	AssignNewToChildLocal(bg, EditableTextWidget, testText, "TestTextInput");
	testText->SetFontSize(12);
	testText->SetText("this is a test of some\nmulti line text will this support\nthat feature very well? who knows let us\nsee if that is true\n\n\n");
}