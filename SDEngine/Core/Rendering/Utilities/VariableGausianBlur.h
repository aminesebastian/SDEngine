#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Engine/Engine.h"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/DataStructures/Array.h"
#include "Downsampler.h"

#define MAX_DOWNSAMPLE 2

class DefferedCompositor;

class VariableGausianBlur {
public:
	VariableGausianBlur(vec2 RenderTargetResolution);
	~VariableGausianBlur();

	void GausianBlur(const DefferedCompositor* Compositor, int32 Passes, RenderTarget* InputFBO, RenderTarget* OutputFBO);
	void RecompileShaders();

	void SetBlurSize(float Size);
	float GetBlurSize();

private:
	void GausianBlurX(const DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass);
	void GausianBlurY(const DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass);
	void BlendBlurs(const DefferedCompositor* Compositor, RenderTarget* OutputFBO);

	int DownsampleSteps;
	float BlurSize;

	SArray<float> DownSampleFactors;
	SArray<TString> BufferNames;
	SArray<SPair<RenderTarget*, RenderTarget*>> Buffers;

	Downsampler* S_Downsampler;

	Shader* S_GBlur5x1;
	Shader* S_BlurBlend;
};

