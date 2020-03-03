#pragma once
#include "Editor/UserInterface/Inspector/BaseInspectorWidget.h"
#include "Core/Pictorum/Pictorum.h"

class VectorInspectorWidget : public BaseInspectorWidget {
public:
	VectorInspectorWidget(const TString& Name);
	virtual ~VectorInspectorWidget();
	virtual void OnCreated() override;
	virtual void SetLabels(SArray<TString> LabelsIn);
	virtual void OnTargetSet(const FProperty& TargetProperty, EngineObject* TargetObject) override;

private:
	PictorumHorizontalBox* Container;

	SArray<SolidWidget*> ValueBackgroundWidgets;
	SArray<EditableTextWidget*> ValueWidgets;
	SArray<FColor> Colors;
	SArray<TString> Labels;
	SArray<float*> Values;
	int32 MouseDownEntry;
	float ContentPadding;

	void ValueMouseDown(PictorumWidget* Widget, const vec2& MouseLocation, const EMouseButton& Button, FUserInterfaceEvent& EventIn);
	void ValueMouseMove(PictorumWidget* Widget, const Vector2D& MouseLocation, const Vector2D& Delta, FUserInterfaceEvent& EventIn);
	void ValueHovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void ValueUnhovered(PictorumWidget* Widget, const vec2& MouseLocation, FUserInterfaceEvent& EventIn);
	void MouseMoveAnywhere(const vec2& MouseLocation, const vec2& Delta);
	void MouseUpAnywhere(const vec2& MouseLocation);

	void AddEntry(const TString& Label, float* InitialValue, const int32& Index);
};

