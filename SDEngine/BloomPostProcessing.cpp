#include "BloomPostProcessing.h"
#include "Engine.h"

BloomPostProcessing::BloomPostProcessing() {
	S_BloomShader = new Shader("Res/Shaders/PostProcessing/Bloom");
	S_LowResBuffer = new FrontBufferObject();
	S_LowResBuffer->AddTextureIndex("xBloom");
	S_LowResBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
}
BloomPostProcessing::~BloomPostProcessing() {}

void BloomPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	RenderXPass(Compositor, Camera, ReadBuffer, OutputBuffer);
	RenderYPass(Compositor, Camera, ReadBuffer, OutputBuffer);
}
void BloomPostProcessing::RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_LowResBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BloomShader->Bind();
	S_BloomShader->SetShaderInteger("PASS", 0);
	S_BloomShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_BloomShader);

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrontBufferObject* ReadBuffer, FrontBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	OutputBuffer->BindForWriting();
	S_LowResBuffer->BindForReading();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BloomShader->Bind();
	S_BloomShader->SetShaderInteger("PASS", 1);
	S_BloomShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_BloomShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 8);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_LowResBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_BloomShader->GetProgram(), S_LowResBuffer->GetTextureName(0).c_str()), 8);

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RecompileShaders() {
	S_BloomShader->RecompileShader();
}