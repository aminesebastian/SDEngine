#include "BaseInspectorWidget.h"

BaseInspectorWidget::BaseInspectorWidget(const TString& Name) : PictorumWidget(Name) {
	InspectionObject  = nullptr;
	bInitialized      = false;
}
BaseInspectorWidget::~BaseInspectorWidget() {

}
const FProperty& BaseInspectorWidget::GetInspectedProperty() const {
	return InspectedProperty;
}
const EngineObject* BaseInspectorWidget::GetInspectedObject() const {
	return InspectionObject;
}
void BaseInspectorWidget::SetTarget(const FProperty& TargetProperty, EngineObject* TargetObject) {
	InspectedProperty = TargetProperty;
	InspectionObject  = TargetObject;
	bInitialized      = true;
	OnTargetSet(TargetProperty, TargetObject);
}