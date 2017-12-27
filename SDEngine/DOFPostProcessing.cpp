#include "DOFPostProcessing.h"

DOFLayer::DOFLayer() {
	S_DepthOfFieldShader = new Shader("Res/Shaders/PostProcessing/DOF", false);

	S_XBlurBuffer = new FrameBufferObject();
	S_XBlurBuffer->AddTextureIndex(new FFBOTextureEntry("xBlur", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_XBlurBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);

	S_YBlurBuffer = new FrameBufferObject();
	S_YBlurBuffer->AddTextureIndex(new FFBOTextureEntry("yBlur", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_YBlurBuffer->Init(WINDOW_WIDTH, WINDOW_HEIGHT);
}
DOFLayer::~DOFLayer() {}

void DOFLayer::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	int Passes = 2;					
	bool horizontal = true;
	bool firstPass = true;
	for (int i = 0; i < Passes * 2; i++) {
		if (horizontal) {
			RenderXPass(Compositor, Camera, ReadBuffer, OutputBuffer, firstPass);
		} else {
			RenderYPass(Compositor, Camera, ReadBuffer, OutputBuffer);
		}
		horizontal = !horizontal;
		firstPass = false;
	}
	BlendOutput(Compositor, Camera, ReadBuffer, OutputBuffer);
}
void DOFLayer::RenderXPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer, bool bFirstPass) {
	ReadBuffer->BindForReading();
	S_YBlurBuffer->BindForReading();
	S_XBlurBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 0);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_DepthOfFieldShader);

	if (bFirstPass) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + 9);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(7));
		glUniform1i(glGetUniformLocation(S_DepthOfFieldShader->GetProgram(), S_YBlurBuffer->GetTextureName(0).c_str()), 9);
	}else {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + 9);
		glGenerateMipmapEXT(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, S_YBlurBuffer->GetTexture(0));
		glUniform1i(glGetUniformLocation(S_DepthOfFieldShader->GetProgram(), S_YBlurBuffer->GetTextureName(0).c_str()), 9);
	}


	Compositor->DrawScreenQuad();
}
void DOFLayer::RenderYPass(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_XBlurBuffer->BindForReading();
	S_YBlurBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 1);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_DepthOfFieldShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 10);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_XBlurBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_DepthOfFieldShader->GetProgram(), S_XBlurBuffer->GetTextureName(0).c_str()), 10);

	Compositor->DrawScreenQuad();
}
void DOFLayer::BlendOutput(DefferedCompositor* Compositor, Camera* Camera, FrameBufferObject* ReadBuffer, FrameBufferObject* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_YBlurBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 2);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetPosition());
	ReadBuffer->BindTextures(S_DepthOfFieldShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 10);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_YBlurBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_DepthOfFieldShader->GetProgram(), S_YBlurBuffer->GetTextureName(0).c_str()), 10);

	Compositor->DrawScreenQuad();
}

void DOFLayer::RecompileShaders() {
	S_DepthOfFieldShader->RecompileShader();
}