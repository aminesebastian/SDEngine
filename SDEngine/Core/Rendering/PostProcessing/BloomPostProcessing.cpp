#include "BloomPostProcessing.h"
#include "Core/Rendering/Utilities/VariableGausianBlur.h"
#include "Core/Engine/Engine.h"


BloomPostProcessing::BloomPostProcessing(RenderViewport* OwningViewport) : PostProcessingLayer("Bloom", OwningViewport) {
	ClipHDRShader = new Shader("Res/Shaders/PostProcessing/Bloom/ClipHDR", false);
	BloomBlendShader = new Shader("Res/Shaders/PostProcessing/Bloom/BlendBloom", false);
	GausBlurUtilitiy = new VariableGausianBlur(OwningViewport->GetOwningWindow()->GetDimensions());

	ClippedHDRBuffer = new RenderTarget(OwningViewport->GetOwningWindow()->GetDimensions());
	ClippedHDRBuffer->AddTextureIndex(new FRenderTargetTextureEntry("HDR", GL_RGBA32F, GL_LINEAR, GL_CLAMP, GL_RGBA, GL_FLOAT, false));
	ClippedHDRBuffer->FinalizeRenderTarget();

	BloomOutputBuffer = new RenderTarget(OwningViewport->GetOwningWindow()->GetDimensions());
	BloomOutputBuffer->AddTextureIndex(new FRenderTargetTextureEntry("Bloom", GL_RGBA32F, GL_LINEAR, GL_REPEAT, GL_RGBA, GL_FLOAT));
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
	GausBlurUtilitiy->GausianBlur(Compositor, BlurPasses, ClippedHDRBuffer, BloomOutputBuffer);
	BlendOutput(Compositor, BloomOutputBuffer, PreviousOutput, OutputBuffer);
}

void BloomPostProcessing::ClipHDR(const DefferedCompositor* Compositor, RenderTarget* ReadBuffer, RenderTarget* ClipBuffer) {
	ReadBuffer->BindForReading();
	ClipBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ClipHDRShader->Bind();
	ClipHDRShader->SetShaderFloat("THRESHOLD", BloomThreshold);

	ReadBuffer->BindTexture(ClipHDRShader, 0, 0, "Input");

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::BlendOutput(const DefferedCompositor* Compositor, RenderTarget* BloomBuffer, RenderTarget* LitBuffer, RenderTarget* OutputBuffer) {
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	BloomBlendShader->Bind();
	BloomBlendShader->SetShaderFloat("BLOOM_WEIGHT", BloomWeight);

	BloomBuffer->BindForReading();
	BloomBuffer->BindTexture(BloomBlendShader, 0, 0, "Bloom");
	LitBuffer->BindForReading();
	LitBuffer->BindTexture(BloomBlendShader, 0, 1, "Input");

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RecompileShaders() {
	ClipHDRShader->RecompileShader();
	BloomBlendShader->RecompileShader();
	GausBlurUtilitiy->RecompileShaders();
}
void BloomPostProcessing::OnScreenResolutionChanged() {
	delete ClippedHDRBuffer;
	delete BloomOutputBuffer;
	delete GausBlurUtilitiy;
	
	ClippedHDRBuffer = new RenderTarget(GetOwningViewport()->GetOwningWindow()->GetDimensions());
	ClippedHDRBuffer->AddTextureIndex(new FRenderTargetTextureEntry("HDR", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP, GL_RGBA, GL_FLOAT, false));
	ClippedHDRBuffer->FinalizeRenderTarget();

	BloomOutputBuffer = new RenderTarget(GetOwningViewport()->GetOwningWindow()->GetDimensions());
	BloomOutputBuffer->AddTextureIndex(new FRenderTargetTextureEntry("Bloom", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_REPEAT, GL_RGBA, GL_FLOAT));
	BloomOutputBuffer->FinalizeRenderTarget();

	GausBlurUtilitiy = new VariableGausianBlur(GetOwningViewport()->GetOwningWindow()->GetDimensions());
}
void BloomPostProcessing::SetBloomThreshold(float Threshold) {
	BloomThreshold = Threshold;
}
float BloomPostProcessing::GetBloomThreshold() {
	return BloomThreshold;
}

void BloomPostProcessing::SetBloomSize(float Size) {
	GausBlurUtilitiy->SetBlurSize(Size);
}
float BloomPostProcessing::GetBloomSize() {
	return GausBlurUtilitiy->GetBlurSize();
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