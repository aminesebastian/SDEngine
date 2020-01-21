#pragma once
#include "RenderTarget.h"
#include "GBuffer.h"
#include "DefferedCompositor.h"
#include "Camera.h"
#include "DetailsPanelProvider.h"
#include "UserInterface.h"

class PostProcessingLayer : public IDetailsPanelProvider {
public:
	PostProcessingLayer(TString Name, vec2 RenderTargetDimensions);
	~PostProcessingLayer();

	virtual void RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) = 0;
	virtual void RecompileShaders() {}

	virtual bool PopulateDetailsPanel() override;
	virtual bool PopulatePostProcessingDetailsPanel();
	virtual TString GetDetailsPanelName() override;

	void Enable();
	void Disable();
	void ToggleEnabled();
	bool IsEnabled();
	TString GetPostProcessingLayerName();
	vec2 GetRenderTargetDimensions();

private:
	TString LayerName;
	bool bEnabled;
	vec2 RenderTargetDimensions;
};

