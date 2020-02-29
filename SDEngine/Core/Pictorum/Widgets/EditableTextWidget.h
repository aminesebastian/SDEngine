#pragma once
#include "Core/Pictorum/Widgets/TextWidget.h"

class EditableTextWidget : public TextWidget {
public:
	EditableTextWidget(const TString& Name);
	virtual ~EditableTextWidget();

	virtual void Tick(float DeltaTime, const FRenderGeometry& Geometry) override;
	virtual void Draw(float DeltaTime, const FRenderGeometry& Geometry) override;

protected:
	void DrawCursor(const FRenderGeometry& Geometry);
	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnRecievedFocus() override;
	virtual void OnFocusLost() override;
private:
	FBoxDrawInstruction* CursorDrawInstruction;
	int32 CursorStartIndex;
	int32 CursorEndIndex;
	float BlinkTime;
	float CurrentBlinkTime;
	bool bCursorFlashOn;
};

