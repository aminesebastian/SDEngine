#include "VariableGausianBlur.h"
#include "Rendering/DefferedCompositor.h"

VariableGausianBlur::VariableGausianBlur(vec2 RenderTargetResolution) {
	DownsampleSteps = MAX_DOWNSAMPLE;

	BufferNames.Add("Full");
	BufferNames.Add("Half");
	BufferNames.Add("Quarter");
	BufferNames.Add("Eighth");

	for (int i = 0; i < DownsampleSteps; i++) {
		float downsampleFactor = (float)glm::sqrt(i + 1);
		DownSampleFactors.Add(downsampleFactor);

		RenderTarget* firstBuffer = new RenderTarget(RenderTargetResolution / downsampleFactor);
		firstBuffer->AddTextureIndex(new FRenderTargetTextureEntry("BlurAccumulation", GL_RGB32F, GL_LINEAR, GL_CLAMP, GL_RGB, GL_FLOAT, false));
		firstBuffer->FinalizeRenderTarget();

		RenderTarget* secondBuffer = new RenderTarget(RenderTargetResolution / downsampleFactor);
		secondBuffer->AddTextureIndex(new FRenderTargetTextureEntry("BlurAccumulation", GL_RGB32F, GL_LINEAR, GL_CLAMP, GL_RGB, GL_FLOAT, false));
		secondBuffer->FinalizeRenderTarget();

		Buffers.Emplace(firstBuffer, secondBuffer);
	}

	S_Downsampler = new Downsampler();

	BlurSize = 1.0f;

	S_GBlur5x1 = new Shader("Res/Shaders/PostProcessing/GBlur5x1", false);
	S_BlurBlend = new Shader("Res/Shaders/PostProcessing/BlendGausBlur", false);
}
VariableGausianBlur::~VariableGausianBlur() {
	delete S_GBlur5x1;
	delete S_BlurBlend;

	for (SPair<RenderTarget*, RenderTarget*> pair : Buffers) {
		delete pair.first;
		delete pair.second;
	}
}
void VariableGausianBlur::GausianBlur(DefferedCompositor* Compositor, int32 Passes, RenderTarget* HDRBuffer, RenderTarget* OutputFBO) {
	// Downsample the frame into the second buffer for each pair.
	// The blur will the proceed as second->first and then first->second.
	for (int i = 0; i < DownsampleSteps; i++) {
		S_Downsampler->Downsample(glm::pow(DownSampleFactors[i], Passes), HDRBuffer, Buffers[i].first);
	}

	// Bind the gauss blur shader.
	S_GBlur5x1->Bind();

	for (int k = 0; k < Passes; k++) {
		for (int i = 0; i < DownsampleSteps; i++) {
			GausianBlurX(Compositor, Buffers[i].first, Buffers[i].second, DownSampleFactors[i], k);
			GausianBlurY(Compositor, Buffers[i].second, Buffers[i].first, DownSampleFactors[i], k);
		}
	}

	BlendBlurs(Compositor, OutputFBO);
}
void VariableGausianBlur::GausianBlurX(DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass) {
	InputFBO->BindForReading();
	OutputFBO->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_GBlur5x1->SetShaderVector2("DIRECTION", vec2(1.0f, 0.0f));
	S_GBlur5x1->SetShaderFloat("BLUR_SIZE", BlurSize);
	S_GBlur5x1->SetShaderFloat("DOWNSAMPLE_FACTOR", DownsampleFactor);
	S_GBlur5x1->SetShaderInteger("PASS", Pass+1);

	InputFBO->BindTextures(S_GBlur5x1);
	Compositor->DrawScreenQuad();
}
void VariableGausianBlur::GausianBlurY(DefferedCompositor* Compositor, RenderTarget* InputFBO, RenderTarget* OutputFBO, float DownsampleFactor, int Pass) {
	InputFBO->BindForReading();
	OutputFBO->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_GBlur5x1->SetShaderVector2("DIRECTION", vec2(0.0f, 1.0f));
	S_GBlur5x1->SetShaderFloat("BLUR_SIZE", BlurSize);
	S_GBlur5x1->SetShaderFloat("DOWNSAMPLE_FACTOR", DownsampleFactor);
	S_GBlur5x1->SetShaderInteger("PASS", Pass+1);

	InputFBO->BindTexture(S_GBlur5x1);
	Compositor->DrawScreenQuad();
}
void VariableGausianBlur::BlendBlurs(DefferedCompositor* Compositor, RenderTarget* OutputFBO) {
	OutputFBO->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BlurBlend->Bind();

	for (int i = 0; i < DownsampleSteps; i++) {
		Buffers[i].first->BindForReading();
		Buffers[i].first->BindTexture(S_BlurBlend, 0, i, BufferNames[i]);
	}

	Compositor->DrawScreenQuad();
}
void VariableGausianBlur::SetBlurSize(float Size) {
	BlurSize = Size;
}
float VariableGausianBlur::GetBlurSize() {
	return BlurSize;
}
void VariableGausianBlur::RecompileShaders() {
	S_GBlur5x1->RecompileShader();
	S_BlurBlend->RecompileShader();
	S_Downsampler->RecompileShaders();
}