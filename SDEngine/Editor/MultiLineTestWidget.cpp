#include "MultiLineTestWidget.h"

MultiLineTestWidget::MultiLineTestWidget(const TString& Name) : PictorumWidget(Name) {

}
void MultiLineTestWidget::OnCreated() {
	AssignNewChildLocal(SolidWidget, bg, "Background");
	bg->SetBackgroundColor(FColor(0.2f));

	AssignNewToChildLocal(bg, EditableTextWidget, testText, "TestTextInput");
	testText->SetFontSize(12);
	testText->SetText("abcd\nfgt");
}