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
void BaseInspectorWidget::SetTarget(const FProperty& TargetProperty, TypeDescriptor* InspectionTargetType, void* TargetObject) {
	InspectedProperty = TargetProperty;
	InspectionObject  = TargetObject;
	InspectionType    = InspectionTargetType;
	bInitialized      = true;
	OnTargetSet(TargetProperty, TargetObject);
}