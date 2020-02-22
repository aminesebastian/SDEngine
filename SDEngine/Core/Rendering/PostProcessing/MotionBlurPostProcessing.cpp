#include "MotionBlurPostProcessing.h"
#include "Core/Engine/Engine.h"
#include "Core/Rendering/GBuffer.h"


MotionBlurPostProcessing::MotionBlurPostProcessing(RenderViewport* OwningViewport) : PostProcessingLayer("Motion Blur", OwningViewport) {
	S_MotionBlurShader = new Shader("Res/Shaders/PostProcessing/MotionBlur/MotionBlur", false);
	Amount = 10.0f;
	Samples = 20;
}
MotionBlurPostProcessing::~MotionBlurPostProcessing() {}

void MotionBlurPostProcessing::RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
	ApplyMotionBlur(Compositor, ReadBuffer, PreviousOutput, OutputBuffer);
}
void MotionBlurPostProcessing::ApplyMotionBlur(const DefferedCompositor* Compositor, GBuffer* ReadBuffer, RenderTarget* InputBuffer, RenderTarget* OutputBuffer) {
	InputBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	S_MotionBlurShader->Bind();
	S_MotionBlurShader->SetShaderFloat("MOTION_BLUR_SCALE", Amount);
	S_MotionBlurShader->SetShaderFloat("FPS", 1.0f / GetOwningViewport()->GetWorld()->GetLastFrameTime());
	S_MotionBlurShader->SetShaderInteger("SAMPLES", Samples);

	// Bind the motion vector texture as well.
	ReadBuffer->BindTexture(S_MotionBlurShader, GBuffer::GBUFFER_TEXTURE_TYPE::GBUFFER_TEXTURE_TYPE_MOTION, 0, "MotionVector");

	// Bind the texture to blur.
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D, InputBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_MotionBlurShader->GetProgram(), "Source"), 1);
	glBindSampler(1, InputBuffer->GetTexture(0));

	Compositor->DrawScreenQuad();
}
void MotionBlurPostProcessing::RecompileShaders() {
	S_MotionBlurShader->RecompileShader();
}
float MotionBlurPostProcessing::GetMotionBlurAmount() {
	return Amount;
}
void MotionBlurPostProcessing::SetMotionBlurAmount(float Amount) {
	this->Amount = Amount;
}
int MotionBlurPostProcessing::GetMotionBlurSamples() {
	return Samples;
}
void MotionBlurPostProcessing::SetMotionBlurSamples(int Samples) {
	this->Samples = Samples;
}