#pragma once
#include "Core/Pictorum/PictorumWidget.h"
#include "Core/Engine/WindowDataTypes.h"
#include "Core/DataStructures/Array.h"

class TextWidget;
class SolidWidget;
class HorizontalBoxWidget;
class OverlayWidget;
class LayoutWidget;

class FloatingDetailsPanel : public PictorumWidget {
public:
	FloatingDetailsPanel(const TString& Name);
	virtual ~FloatingDetailsPanel();
	virtual void OnCreated() override;

private:
	void ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta);
	void MouseUpAnywhere(const vec2& MouseLocation);

	int32 MouseDownEntry;
	uint8 EntryCount;
	SolidWidget* Background;
	HorizontalBoxWidget* Container;
	SArray<SolidWidget*> ValueBackgroundWidgets;
	SArray<TextWidget*> ValueWidgets;
	SArray<float> Values;
	SArray<FColor> Colors;
};

