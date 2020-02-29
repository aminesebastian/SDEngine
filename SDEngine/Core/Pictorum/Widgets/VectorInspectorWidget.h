#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Pictorum/Pictorum.h"

class VectorInspectorWidget : public PictorumWidget {
public:
	VectorInspectorWidget(const TString& Name);
	virtual ~VectorInspectorWidget();
	virtual void OnCreated() override;
	void SetControlledValue(float* ValuePointer, uint8 Count);

private:
	PictorumHorizontalBox* Container;

	SArray<SolidWidget*> ValueBackgroundWidgets;
	SArray<EditableTextWidget*> ValueWidgets;
	SArray<FColor> Colors;

	SArray<float*> Values;
	int32 MouseDownEntry;

	void ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	void ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta);
	void MouseUpAnywhere(const vec2& MouseLocation);

	void AddEntry(float* InitialValue, const int32& Index);
};

