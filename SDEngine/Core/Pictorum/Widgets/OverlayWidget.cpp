#include "OverlayWidget.h"

OverlayWidget::OverlayWidget(const TString& Name) : PictorumWidget(Name) {

}
OverlayWidget::~OverlayWidget() {

}

const bool OverlayWidget::CanAddChild() const {
	return true;
}