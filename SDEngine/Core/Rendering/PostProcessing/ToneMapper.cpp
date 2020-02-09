#include "ToneMapper.h"

ToneMapper::ToneMapper(RenderViewport* OwningViewport) : PostProcessingLayer("Tonemapper", OwningViewport) {
	S_TonemapperShader = new Shader("Res/Shaders/PostProcessing/ToneMapping", false);
}
ToneMapper::~ToneMapper() {}

void ToneMapper::RenderLayer(const DefferedCompositor* Compositor, const Camera* RenderCamera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
	PreviousOutput->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_TonemapperShader->Bind();
	PreviousOutput->BindTextures(S_TonemapperShader);

	Compositor->DrawScreenQuad();
}
void ToneMapper::RecompileShaders() {
	S_TonemapperShader->RecompileShader();
}