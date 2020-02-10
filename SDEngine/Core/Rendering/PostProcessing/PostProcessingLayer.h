#pragma once
#include "Core/Engine/Window.h"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Rendering/GBuffer.h"
#include "Core/Rendering/RenderViewport.h"
#include "UserInterface/DetailsPanelProvider.h"
#include "UserInterface/UserInterface.h"

class RenderViewport;

class PostProcessingLayer : public IDetailsPanelProvider {
public:
	PostProcessingLayer(const TString& Name, RenderViewport* OwningViewport);
	~PostProcessingLayer();

	virtual void RenderLayer(const DefferedCompositor* Compositor, const Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) = 0;
	virtual void RecompileShaders() {}
	virtual void OnScreenResolutionChanged() {}

	virtual bool PopulateDetailsPanel() override;
	virtual bool PopulatePostProcessingDetailsPanel();
	virtual TString GetDetailsPanelName() override;

	void Enable();
	void Disable();
	void ToggleEnabled();
	const bool& IsEnabled() const;
	const TString& GetPostProcessingLayerName() const;
	const RenderViewport* GetOwningViewport() const;

private:
	TString LayerName;
	bool bEnabled;
	RenderViewport* OwningViewport;
};

