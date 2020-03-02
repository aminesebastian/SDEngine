#include "BaseInspectorWidget.h"

BaseInspectorWidget::BaseInspectorWidget(const TString& Name) : PictorumWidget(Name) {
	InspectedProperty = nullptr;
	InspectionObject = nullptr;
}
BaseInspectorWidget::~BaseInspectorWidget() {

}
const FProperty* BaseInspectorWidget::GetInspectedProperty() const {
	return InspectedProperty;
}
const EngineObject* BaseInspectorWidget::GetInspectedObject() const {
	return InspectionObject;
}
void BaseInspectorWidget::SetTarget(FProperty* TargetProperty, EngineObject* TargetObject) {
	InspectedProperty = TargetProperty;
	InspectionObject  = TargetObject;
}