#include "BloomPostProcessing.h"
#include "Core/Rendering/Utilities/VariableGausianBlur.h"
#include "Core/Engine/Engine.h"


BloomPostProcessing::BloomPostProcessing(RenderViewport* OwningViewport) : PostProcessingLayer("Bloom", OwningViewport) {
	S_ClipHDRShader = new Shader("Res/Shaders/PostProcessing/Bloom/ClipHDR", false);
	S_BlendBloom = new Shader("Res/Shaders/PostProcessing/Bloom/BlendBloom", false);
	S_GausBlur = new VariableGausianBlur(OwningViewport->GetOwningWindow()->GetDimensions());

	ClippedHDRBuffer = new RenderTarget(OwningViewport->GetOwningWindow()->GetDimensions());
	ClippedHDRBuffer->AddTextureIndex(new FRenderTargetTextureEntry("HDR", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP, GL_RGBA, GL_FLOAT, false));
	ClippedHDRBuffer->FinalizeRenderTarget();

	BloomOutputBuffer = new RenderTarget(OwningViewport->GetOwningWindow()->GetDimensions());
	BloomOutputBuffer->AddTextureIndex(new FRenderTargetTextureEntry("Bloom", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_RGBA, GL_FLOAT));
	BloomOutputBuffer->FinalizeRenderTarget();

	BlurPasses = 2;
	BloomThreshold = 1.0f;
	BloomWeight = 1.0f;
	SetBloomSize(3.0f);
}
BloomPostProcessing::~BloomPostProcessing() {
	delete ClippedHDRBuffer;
	delete BloomOutputBuffer;
}

void BloomPostProcessing::RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
	ClipHDR(Compositor, PreviousOutput, ClippedHDRBuffer);
	S_GausBlur->GausianBlur(Compositor, BlurPasses, ClippedHDRBuffer, BloomOutputBuffer);
	BlendOutput(Compositor, BloomOutputBuffer, PreviousOutput, OutputBuffer);
}

void BloomPostProcessing::ClipHDR(const DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* ClipBuffer) {
	ReadBuffer->BindForReading();
	ClipBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_ClipHDRShader->Bind();
	S_ClipHDRShader->SetShaderFloat("THRESHOLD", BloomThreshold);

	ReadBuffer->BindTexture(S_ClipHDRShader, 0, 0, "Input");

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::BlendOutput(const DefferedCompositor* Compositor, RenderTarget* BloomBuffer, RenderTarget* LitBuffer, RenderTarget* OutputBuffer) {
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BlendBloom->Bind();
	S_BlendBloom->SetShaderFloat("BLOOM_WEIGHT", BloomWeight);

	BloomBuffer->BindForReading();
	BloomBuffer->BindTexture(S_BlendBloom, 0, 0, "Bloom");
	LitBuffer->BindForReading();
	LitBuffer->BindTexture(S_BlendBloom, 0, 1, "Input");

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RecompileShaders() {
	S_ClipHDRShader->RecompileShader();
	S_BlendBloom->RecompileShader();
	S_GausBlur->RecompileShaders();
}

void BloomPostProcessing::SetBloomThreshold(float Threshold) {
	BloomThreshold = Threshold;
}
float BloomPostProcessing::GetBloomThreshold() {
	return BloomThreshold;
}

void BloomPostProcessing::SetBloomSize(float Size) {
	S_GausBlur->SetBlurSize(Size);
}
float BloomPostProcessing::GetBloomSize() {
	return S_GausBlur->GetBlurSize();
}

void BloomPostProcessing::SetBloomWeight(float Weight) {
	BloomWeight = Weight;
}
float BloomPostProcessing::GetBloomWeight() {
	return BloomWeight;
}

bool BloomPostProcessing::PopulatePostProcessingDetailsPanel() {
	float bloomSize = GetBloomSize();
	if (ImGui::SliderFloat("Size", &bloomSize, 0.0f, 10.0f)) {
		SetBloomSize(bloomSize);
	}
	ImGui::SliderFloat("Weight", &BloomWeight, 0.0f, 10.0f);
	ImGui::SliderFloat("Threshold", &BloomThreshold, 0.0f, 5.0f);
	ImGui::SliderInt("Passes", &BlurPasses, 1, 5, "%d Passes");

	return true;
}