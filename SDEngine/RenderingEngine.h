#pragma once
#include <string>

class UWorld;
class Shader;
class Camera;
class GBuffer;
class Display;
class DefferedCompositor;

class URenderingEngine {
public:
	URenderingEngine(Display* Display);
	~URenderingEngine();

	void ChangeShader(std::string ShaderName);
	void RenderWorld(UWorld* World, Camera* Camera);
	void GemoetryPass(UWorld* World, Camera* Camera, GBuffer* WriteBuffer);
	void TranslucencyPass(UWorld* World, Camera* Camera, GBuffer* ReadBuffer, GBuffer* WriteBuffer);
	void RecompileShaders(UWorld* World);
	void DebugGBuffer();

	GBuffer* GetReadGBuffer() { return S_CurrentBuffer == 1 ? S_Buffer1 : S_Buffer2; }
	GBuffer* GetFreeGBuffer() { return S_CurrentBuffer == 1 ? S_Buffer2 : S_Buffer1; }
	void FlipCurrentBufferIndex() { S_CurrentBuffer == 1 ? S_CurrentBuffer = 2 : S_CurrentBuffer = 1; }

private:
	Shader* S_Shader;
	GBuffer* S_Buffer1;
	GBuffer* S_Buffer2;
	int S_CurrentBuffer = 1;
	Display* S_Display;
	DefferedCompositor* S_DefferedCompositor;
};

