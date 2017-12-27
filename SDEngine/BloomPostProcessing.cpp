#include "BloomPostProcessing.h"
#include "Engine.h"

BloomPostProcessing::BloomPostProcessing() {
	S_BloomShader = new Shader("Res/Shaders/PostProcessing/Bloom", false);

	S_XBloomBuffer = new FrameBufferObject();
	S_XBloomBuffer->AddTextureIndex(new FFBOTextureEntry("xBloom", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_XBloomBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	S_YBloomBuffer = new FrameBufferObject();
	S_YBloomBuffer->AddTextureIndex(new FFBOTextureEntry("yBloom", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_YBloomBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
}
BloomPostProcessing::~BloomPostProcessing() {}

void BloomPostProcessing::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	int Passes = 5;						//Passes must be odd
	bool horizontal = true;
	bool firstPass = true;
	for (int i = 0; i < Passes*2; i++) {
		if(horizontal) {
			RenderXPass(Compositor, Camera, ReadBuffer, OutputBuffer, firstPass);
		}else{
			RenderYPass(Compositor, Camera, ReadBuffer, OutputBuffer);
		}
		horizontal = !horizontal;
		firstPass = false;
	}
	BlendOutput(Compositor, Camera, ReadBuffer, OutputBuffer);
}
void BloomPostProcessing::RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer, bool bFirstPass) {
	ReadBuffer->BindForReading();
	S_YBloomBuffer->BindForReading();
	S_XBloomBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BloomShader->Bind();
	S_BloomShader->SetShaderInteger("PASS", 0);
	S_BloomShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_BloomShader);

	if(bFirstPass) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + 9);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(6));
		glUniform1i(glGetUniformLocation(S_BloomShader->GetProgram(), S_YBloomBuffer->GetTextureName(0).c_str()), 9);
	}else{
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + 9);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, S_YBloomBuffer->GetTexture(0));
		glUniform1i(glGetUniformLocation(S_BloomShader->GetProgram(), S_YBloomBuffer->GetTextureName(0).c_str()), 9);
	}
	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_XBloomBuffer->BindForReading();
	S_YBloomBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BloomShader->Bind();
	S_BloomShader->SetShaderInteger("PASS", 1);
	ReadBuffer->BindTextures(S_BloomShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 10);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_XBloomBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_BloomShader->GetProgram(), S_XBloomBuffer->GetTextureName(0).c_str()), 10);

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::BlendOutput(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_YBloomBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_BloomShader->Bind();
	S_BloomShader->SetShaderInteger("PASS", 2);
	ReadBuffer->BindTextures(S_BloomShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 10);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_YBloomBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_BloomShader->GetProgram(), S_YBloomBuffer->GetTextureName(0).c_str()), 10);

	Compositor->DrawScreenQuad();
}
void BloomPostProcessing::RecompileShaders() {
	S_BloomShader->RecompileShader();
}