#pragma once
#include "Editor/UserInterface/Inspector/BaseInspectorWidget.h"
#include "Core/Pictorum/Pictorum.h"

class FloatInspectorWidget : public BaseInspectorWidget {
public:
	FloatInspectorWidget(const TString& Name);
	virtual ~FloatInspectorWidget();
	virtual void OnCreated() override;
	virtual void OnTargetSet(FProperty* TargetProperty, EngineObject* TargetObject) override;

private:
	SolidWidget* ValueBackgroundWidget;
	EditableTextWidget* ValueWidget;

	bool bMouseWasDownInside;
	float ContentPadding;

	void ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	void ValueMouseMove(PictorumWidget* Widget, const Vector2D& MouseLocation, const Vector2D& Delta, FUserInterfaceEvent& EventIn);
	void ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta);
	void MouseUpAnywhere(const vec2& MouseLocation);
};

