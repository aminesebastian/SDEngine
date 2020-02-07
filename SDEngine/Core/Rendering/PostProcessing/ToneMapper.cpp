#include "ToneMapper.h"

ToneMapper::ToneMapper(vec2 FinalOutputDimensions) : PostProcessingLayer("Tonemapper", FinalOutputDimensions) {
	S_TonemapperShader = new Shader("Res/Shaders/PostProcessing/ToneMapping", false);
}
ToneMapper::~ToneMapper() {}

void ToneMapper::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
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