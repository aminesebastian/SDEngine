#include "BaseInspectorWidget.h"

BaseInspectorWidget::BaseInspectorWidget(const TString& Name) : PictorumWidget(Name) {
	bInitialized      = false;
}
BaseInspectorWidget::~BaseInspectorWidget() {

}
void BaseInspectorWidget::SetTarget(const PropertyHandle& Property) {
	InspectedProperty  = Property;
	bInitialized      = true;
	OnTargetSet(InspectedProperty);
}
const bool& BaseInspectorWidget::IsInitialized() const {
	return bInitialized;
}
void BaseInspectorWidget::RaiseOnPropertyUpdatedDelegate() {
	OnPropertyUpdatedDelegate.Broadcast(InspectedProperty.GetRawProperty());
}