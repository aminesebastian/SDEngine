#pragma once
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/Entities/Camera.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Rendering/GBuffer.h"
#include "UserInterface/DetailsPanelProvider.h"
#include "UserInterface/UserInterface.h"

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

