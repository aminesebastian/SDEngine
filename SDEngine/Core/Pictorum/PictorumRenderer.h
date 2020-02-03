#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/Input/IUserInputReciever.h"
#include "Core/Input/InputUtilities.h"
#include "Core/Pictorum/PictorumDataTypes.h"
#include "Engine/EngineObject.h"
#include "UserInterface/DetailsPanelProvider.h"


class PictorumWidget;
class TextQuadBuffer;
class DistanceFieldFont;

class PictorumRenderer : public EngineObject, public IUserInputReciever, public IDetailsPanelProvider {
public:
	PictorumRenderer(TString ViewportName, vec2 RenderTargetResolution);
	~PictorumRenderer();

	void Tick(float DeltaTime);
	void Draw(float DeltaTime);

	virtual void OnKeyDown(SDL_Scancode KeyCode) override;
	virtual void OnKeyUp(SDL_Scancode KeyCode) override;
	virtual void OnKeyHeld(SDL_Scancode KeyCode, float HeldTime) override;

	virtual void OnMouseButtonDown(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseButtonUp(vec2 ScreenPosition, EMouseButton Button) override;
	virtual void OnMouseAxis(vec2 ScreenPosition, vec2 Delta) override;
	virtual void OnMouseScrollAxis(float Delta) override;

	bool AddToViewport(PictorumWidget* Widget);
	bool RemoveFromViewport(PictorumWidget* Widget);

	void OnRenderTargetResolutionChanged(vec2 NewResolution);

	PictorumWidget* GetMouseOverWidget();

	const SArray<PictorumWidget*>& GetWidgets();

	virtual TString GetDetailsPanelName() override;
	virtual bool PopulateDetailsPanel() override;
private:
	SArray<PictorumWidget*> Widgets;
	PictorumWidget* MouseOverWidget;
	TextQuadBuffer* QuadBuffer;
	DistanceFieldFont* DistanceField;

	bool bMouseCaptured;
	FRenderGeometry TopLevelRenderGeometry;

	void CacheMouseOverWidget(vec2 MousePosition);
};

