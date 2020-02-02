#pragma once
#include "Rendering/RenderTarget.h"
#include "Downsampler.h"
#include "Rendering/Shader.h"
#include "Engine/Engine.h"
#include "Core/DataTypes/TypeDefenitions.h"

#define MAX_DOWNSAMPLE 2

class VariableGausianBlur {
public:
	VariableGausianBlur(vec2 RenderTargetResolution);
	~VariableGausianBlur();

	void GausianBlur(DefferedCompositor* Compositor, int32 Passes, RenderTarget* InputFBO, RenderTarget* OutputFBO);
	void RecompileShaders();

	void SetBlurSize(float Size);
	float GetBlurSize();

private:
	void GausianBlurX(DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass);
	void GausianBlurY(DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass);
	void BlendBlurs(DefferedCompositor* Compositor, RenderTarget* OutputFBO);

	int DownsampleSteps;
	float BlurSize;

	SArray<float> DownSampleFactors;
	SArray<TString> BufferNames;
	SArray<SPair<RenderTarget*, RenderTarget*>> Buffers;

	Downsampler* S_Downsampler;

	Shader* S_GBlur5x1;
	Shader* S_BlurBlend;
};

