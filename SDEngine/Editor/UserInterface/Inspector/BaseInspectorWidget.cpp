#include "BaseInspectorWidget.h"

BaseInspectorWidget::BaseInspectorWidget(const TString& Name) : PictorumWidget(Name) {
	bInitialized      = false;
	InspectedProperty = nullptr;
}
BaseInspectorWidget::~BaseInspectorWidget() {

}
void BaseInspectorWidget::SetTarget(const ReflectionWrapper& Wrapper, const FProperty* TargetProperty) {
	InspectionTarget  = Wrapper;
	InspectedProperty = TargetProperty;
	bInitialized      = true;
	OnTargetSet(InspectionTarget, InspectedProperty);
}
const bool& BaseInspectorWidget::IsInitialized() const {
	return bInitialized;
}