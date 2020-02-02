#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"

class World;
class Shader;
class Camera;
class GBuffer;
class Display;
class DefferedCompositor;
class PostProcessingLayer;
class VariableGausianBlur;
class RenderTarget;
class Entity;
class TransformGizmo;

enum ERenderingStage {
	GEOMETRY,
	TRANSLUCENCY,
	LIGHTING,
	POST_PROCESSING,
	EDITOR_ELEMENTS,
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

class RenderViewport {
public:
	RenderViewport(vec2 RenderTargetDimensions);
	~RenderViewport();

	void Initialize();

	void ChangeRenderTargetDimensions(vec2 NewRenderTargetDimensions);
	vec2 GetRenderTargetDimensions();

	void RenderWorld(World* RenderWorld, Camera* RenderCamera);
	void GemoetryPass(World* RenderWorld, Camera* RenderCamera);
	void TranslucencyPass(World* RenderWorld, Camera* RenderCamera);
	void BlendTransparencyPass(World* RenderWorld, Camera* RenderCamera);
	void RenderPostProcessing(World* RenderWorld, Camera* RenderCamera);
	void RenderEditorElements(World* RenderWorld, Camera* RenderCamera);

	void RecompileShaders();
	int GetTranslucentObjectCount(World* RenderWorld);

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

	void OnMouseMove(vec2 MouseCoords);
protected:
	virtual void GenerateRenderTargets();
	virtual void RegisterPostProcessEffects();
private:
	Shader* S_TranslucencyBlendShader;
	Shader* SelectionOutlineShader;

	TransformGizmo* TransformGizmoEntity;

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
	bool bInitialized;
	EDebugState S_DebugState;


};

