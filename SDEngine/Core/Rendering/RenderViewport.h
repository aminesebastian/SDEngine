#pragma once
#include "Core/DataStructures/DataStructures.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Engine/WindowDataTypes.h"

class World;
class Shader;
class Camera;
class GBuffer;
class DefferedCompositor;
class PostProcessingLayer;
class VariableGausianBlur;
class RenderTarget;
class Entity;
class TransformGizmo;
class Window;

enum ERenderingStage {
	GEOMETRY,
	TRANSLUCENCY,
	LIGHTING,
	POST_PROCESSING,
	EDITOR_ELEMENTS,
	OUTPUT
};
class RenderViewport {
public:
	RenderViewport(const World* RenderWorld, const Window* OwningWindow);
	~RenderViewport();

	void Initialize();

	void ChangeRenderTargetDimensions(vec2 NewRenderTargetDimensions);

	const bool BindNewShader(Shader* ShaderIn);

	void Render(const Camera* RenderCamera);

	void RecompileShaders();
	ERenderingStage GetRenderingStage() { return CurrentRenderStage; }

	DefferedCompositor* GetDefferedCompositor() { return Compositor; }

	void FlipOutputBuffers();
	RenderTarget* GetCurrentOutputBuffer();
	RenderTarget* GetPreviousOutputBuffer();

	SArray<PostProcessingLayer*> GetPostProcessingLayers();
	void EnablePostProcessingLayer(TString Layer);
	void DisablePostProcessingLayer(TString Layer);
	void TogglePostProcessingLayer(TString Layer);
	bool IsPostProcessingLayerEnabled(TString Layer);

	PostProcessingLayer* GetPostProcessingLayer(TString Layer);

	const Window* GetOwningWindow() const;
	const World* GetWorld() const;
	const float& GetRenderFrameTime() const;
	const float& GetRenderFrameRate() const;

protected:
	virtual void GenerateRenderTargets();
	virtual void RegisterPostProcessEffects();

	void GemoetryPass(const Camera* RenderCamera);
	void RenderPostProcessing(const Camera* RenderCamera);
	void RenderEditorElements(const Camera* RenderCamera);

private:
	Shader* CurrentlyActiveShader;
	Shader* SelectionOutlineShader;
	const Window* OwningWindow;
	const World* RenderWorld;

	float RenderDeltaTime;
	float RenderFrameRate;
	uint64 LastFrameRenderTime;

	int32 CurrentBuffer;
	GBuffer* MainGBuffer;
	RenderTarget* OutputBuffer1;
	RenderTarget* OutputBuffer2;

	SArray<PostProcessingLayer*> S_PostProcessingLayers;

	DefferedCompositor* Compositor;
	ERenderingStage CurrentRenderStage;

	bool bDebugMode;
	bool bInitialized;

	void OnWindowResized(const FDisplayState& State);
};

