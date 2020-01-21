#pragma once
#include <string>
#include "TypeDefenitions.h"


class UWorld;
class Shader;
class Camera;
class GBuffer;
class Display;
class DefferedCompositor;
class PostProcessingLayer;
class VariableGausianBlur;
class RenderTarget;

enum ERenderingStage {
	GEOMETRY,
	TRANSLUCENCY,
	LIGHTING,
	POST_PROCESSING,
	OUTPUT
};
enum EDebugState {
	ALBEDO,
	NORMAL,
	WORLD_POSITION,
	RMAO,
	WIREFRAME,
	DETAIL_LIGHT
};

class URenderingEngine {
public:
	URenderingEngine(vec2 RenderTargetDimensions);
	~URenderingEngine();

	void ChangeRenderTargetDimensions(vec2 NewRenderTargetDimensions);
	vec2 GetRenderTargetDimensions();

	void RenderWorld(UWorld* World, Camera* Camera);
	void GemoetryPass(UWorld* World, Camera* Camera);
	void TranslucencyPass(UWorld* World, Camera* Camera);
	void BlendTransparencyPass(UWorld* World, Camera* Camera);
	void RecompileShaders(UWorld* World);
	int GetTranslucentObjectCount(UWorld* World);

	ERenderingStage GetRenderingStage() { return S_CurrentStage; }
	bool GetDebugEnabled();
	void SetDebugEnabled(bool bEnabled);

	EDebugState GetDebugState();
	void SetDebugState(EDebugState NewState);

	DefferedCompositor* GetDefferedCompositor() { return S_DefferedCompositor; }

	void FlipOutputBuffers();
	RenderTarget* GetCurrentOutputBuffer();
	RenderTarget* GetPreviousOutputBuffer();

	SArray<PostProcessingLayer*> GetPostProcessingLayers();
	void EnablePostProcessingLayer(TString Layer);
	void DisablePostProcessingLayer(TString Layer);
	void TogglePostProcessingLayer(TString Layer);
	bool IsPostProcessingLayerEnabled(TString Layer);

	PostProcessingLayer* GetPostProcessingLayer(TString Layer);

	inline float* GetGeometryPassTime() {
		return &S_GeometryPassTime;
	}
	inline float* GetLightingPassTime() {
		return &S_LightingPassTime;
	}
protected:
	virtual void GenerateRenderTargets();
	virtual void RegisterPostProcessEffects();
private:
	Shader* S_TranslucencyBlendShader;
	
	GBuffer* S_GBuffer;

	int32 CurrentBuffer;
	RenderTarget* S_OutputBuffer1;
	RenderTarget* S_OutputBuffer2;

	RenderTarget* S_TranslucencyBuffer;

	SArray<PostProcessingLayer*> S_PostProcessingLayers;

	vec2 RenderTargetDimensions;
	DefferedCompositor* S_DefferedCompositor;
	ERenderingStage S_CurrentStage;

	bool bDebugMode;
	EDebugState S_DebugState;

	float S_GeometryPassTime;
	float S_LightingPassTime;
	float S_AOPassTime;
	float S_BloomPassTime;
	float S_TonemappingTime;
};

