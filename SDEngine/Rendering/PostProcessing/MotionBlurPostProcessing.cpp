#include "MotionBlurPostProcessing.h"
#include "Engine/Engine.h"
#include "Rendering/GBuffer.h"


MotionBlurPostProcessing::MotionBlurPostProcessing(vec2 FinalOutputDimensions) : PostProcessingLayer("Motion Blur", FinalOutputDimensions) {
	S_MotionBlurShader = new Shader("Res/Shaders/PostProcessing/MotionBlur/MotionBlur", false);
	Amount = 10.0f;
	Samples = 20;
}
MotionBlurPostProcessing::~MotionBlurPostProcessing() {}

void MotionBlurPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* CurrentLitFrame, RenderTarget* OutputBuffer) {
	ApplyMotionBlur(Compositor, ReadBuffer, CurrentLitFrame, OutputBuffer);
}
void MotionBlurPostProcessing::ApplyMotionBlur(DefferedCompositor* Compositor, GBuffer* ReadBuffer, RenderTarget* InputBuffer, RenderTarget* OutputBuffer) {
	InputBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	S_MotionBlurShader->Bind();
	S_MotionBlurShader->SetShaderFloat("MOTION_BLUR_SCALE", Amount);
	S_MotionBlurShader->SetShaderFloat("FPS", (float)Engine::GetInstance()->GetFrameRate());
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
bool MotionBlurPostProcessing::PopulatePostProcessingDetailsPanel() {
	ImGui::SliderFloat("Amount", &Amount, 0.0f, 50.0f);
	ImGui::SliderInt("Samples", &Samples, 1, 100);

	return true;
}