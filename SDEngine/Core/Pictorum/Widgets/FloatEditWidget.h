#pragma once
#include "Core/Pictorum/PictorumWidget.h"

class TextWidget;
class SolidWidget;
class HorizontalBoxWidget;

class FloatEditWidget : PictorumWidget {
public:
	FloatEditWidget(const TString& Name);
	virtual ~FloatEditWidget();
	virtual void OnCreated() override;
	void SetControlledValue(float* ValuePointer, uint8 Count);

private:
	SolidWidget* Background;
	HorizontalBoxWidget* Container;

	SArray<SolidWidget*> ValueBackgroundWidgets;
	SArray<TextWidget*> ValueWidgets;
	SArray<FColor> Colors;

	float* Values;
	uint8 ValueCount;
	int32 MouseDownEntry;

	void ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta);
	void MouseUpAnywhere(const vec2& MouseLocation);

	void AddEntry(const float& InitialValue, const int32& Index);
};

