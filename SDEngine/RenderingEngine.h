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
	URenderingEngine(Display* Display);
	~URenderingEngine();

	void ChangeShader(std::string ShaderName);
	void RenderWorld(UWorld* World, Camera* Camera);
	void GemoetryPass(UWorld* World, Camera* Camera, GBuffer* WriteBuffer);
	void TranslucencyPass(UWorld* World, Camera* Camera, GBuffer* ReadBuffer, GBuffer* WriteBuffer);
	void BlendTransparencyPass(UWorld* World, Camera* Camera, GBuffer* TranslucencyBuffer, GBuffer* ReadBuffer, GBuffer* WriteBuffer);
	void RecompileShaders(UWorld* World);
	void DebugGBuffer();
	int GetTranslucentObjectCount(UWorld* World);

	GBuffer* GetReadGBuffer() { return S_CurrentBuffer == 1 ? S_Buffer1 : S_Buffer2; }
	GBuffer* GetFreeGBuffer() { return S_CurrentBuffer == 1 ? S_Buffer2 : S_Buffer1; }
	void FlipCurrentBufferIndex();

	ERenderingStage GetRenderingStage() { return S_CurrentStage; }
	bool GetDebugEnabled();
	void SetDebugEnabled(bool bEnabled);

	EDebugState GetDebugState();
	void SetDebugState(EDebugState NewState);

	DefferedCompositor* GetDefferedCompositor() { return S_DefferedCompositor; }
private:
	Shader* S_Shader;
	Shader* S_TranslucencyBlendShader;
	
	GBuffer* S_Buffer1;
	GBuffer* S_Buffer2;
	GBuffer* S_TranslucencyBuffer;

	SArray<PostProcessingLayer*> S_PostProcessingLayers;

	int S_CurrentBuffer = 1;
	Display* S_Display;
	DefferedCompositor* S_DefferedCompositor;
	ERenderingStage S_CurrentStage;

	bool bDebugMode;
	EDebugState S_DebugState;
};

