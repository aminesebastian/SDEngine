#include "ToneMapper.h"

ToneMapper::ToneMapper() {
	S_TonemapperShader = new Shader("Res/Shaders/PostProcessing/ToneMapping", false);
}
ToneMapper::~ToneMapper() {}

void ToneMapper::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_TonemapperShader->Bind();
	glUniform3fv(glGetUniformLocation(S_TonemapperShader->GetProgram(), "CAMERA_POS"), 1, &Camera->GetTransform().GetPosition()[0]);
	glUniform1f(glGetUniformLocation(S_TonemapperShader->GetProgram(), "NEAR_CLIP"), Camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_TonemapperShader->GetProgram(), "FAR_CLIP"), Camera->GetFarClipPlane());

	ReadBuffer->BindTextures(S_TonemapperShader);

	Compositor->DrawScreenQuad();
}
void ToneMapper::RecompileShaders() {
	S_TonemapperShader->RecompileShader();
}