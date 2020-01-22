#include "DOFPostProcessing.h"

DOFLayer::DOFLayer(vec2 FinalOutputDimensions) : PostProcessingLayer("Depth of Field", FinalOutputDimensions) {
	S_DepthOfFieldShader = new Shader("Res/Shaders/PostProcessing/DOF", false);

	S_XBlurBuffer = new RenderTarget(FinalOutputDimensions);
	S_XBlurBuffer->AddTextureIndex(new FRenderTargetTextureEntry("xBlur", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_XBlurBuffer->FinalizeRenderTarget();

	S_YBlurBuffer = new RenderTarget(FinalOutputDimensions);
	S_YBlurBuffer->AddTextureIndex(new FRenderTargetTextureEntry("yBlur", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_YBlurBuffer->FinalizeRenderTarget();
}
DOFLayer::~DOFLayer() {}

void DOFLayer::RenderLayer(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* PreviousOutput, RenderTarget* OutputBuffer) {
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
void DOFLayer::RenderXPass(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer, bool bFirstPass) {
	ReadBuffer->BindForReading();
	S_YBlurBuffer->BindForReading();
	S_XBlurBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 0);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetLocation());
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
void DOFLayer::RenderYPass(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_XBlurBuffer->BindForReading();
	S_YBlurBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 1);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetLocation());
	ReadBuffer->BindTextures(S_DepthOfFieldShader);

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + 10);
	glGenerateMipmapEXT(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, S_XBlurBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_DepthOfFieldShader->GetProgram(), S_XBlurBuffer->GetTextureName(0).c_str()), 10);

	Compositor->DrawScreenQuad();
}
void DOFLayer::BlendOutput(DefferedCompositor* Compositor, Camera* Camera, GBuffer* ReadBuffer, RenderTarget* OutputBuffer) {
	ReadBuffer->BindForReading();
	S_YBlurBuffer->BindForReading();
	OutputBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_DepthOfFieldShader->Bind();
	S_DepthOfFieldShader->SetShaderInteger("PASS", 2);
	S_DepthOfFieldShader->SetShaderVector3("CAMERA_POS", Camera->GetTransform().GetLocation());
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