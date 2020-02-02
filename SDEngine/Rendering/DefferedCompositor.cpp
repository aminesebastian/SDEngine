#include "DefferedCompositor.h"
#include <iostream>
#include <string>
#include "Rendering/Texture2D.h"
#include "Entities/Camera.h"

DefferedCompositor::DefferedCompositor(string LightingShader) {
	S_LightingShader = new Shader(LightingShader, false);
	S_FinalOutputShader = (new Shader("./Res/Shaders/PostProcessing/Output", false));
}
DefferedCompositor::~DefferedCompositor() {}

void DefferedCompositor::RecompileShaders() {
	S_LightingShader->RecompileShader();
	S_FinalOutputShader->RecompileShader();
	for (int i = 0; i < S_PostProcessingShaders.size(); i++) {
		S_PostProcessingShaders[i]->RecompileShader();
	}
}
void DefferedCompositor::CompositeLighting(GBuffer* GBufferIn, RenderTarget* WriteBuffer, SArray<Light*> Lights, Camera* Camera) {
	for (int32 i = 0; i < Lights.Count(); i++) {
		if (Lights[i]->IsVisible() && Lights[i]->CastsShadows()) {
			Lights[i]->GenerateShadowTexture(this);
		}
	}

	GBufferIn->BindForReading();
	WriteBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_LightingShader->Bind();

	int lightCount = 0;
	for (Light* light : Lights) {
		if (light->IsVisible()) {
			light->SendShaderInformation(S_LightingShader, lightCount);
			lightCount++;
		}
	}

	glUniform3fv(glGetUniformLocation(S_LightingShader->GetProgram(), "CAMERA_POS"), 1, &Camera->GetTransform().GetLocation()[0]);
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "NEAR_CLIP"), Camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "FAR_CLIP"), Camera->GetFarClipPlane());
	glUniform1i(glGetUniformLocation(S_LightingShader->GetProgram(), "LIGHT_COUNT"), (GLint)lightCount);
	glUniform2fv(glGetUniformLocation(S_LightingShader->GetProgram(), "SCREEN_RES"), 1, &Engine::GetInstance()->GetFocusedViewport()->GetRenderTargetDimensions()[0]);

	GBufferIn->BindTextures(S_LightingShader);



	DrawScreenQuad();
}
void DefferedCompositor::CompositePostProcesing(GBuffer* ReadBuffer, GBuffer* WriteBuffer, Camera* Camera, int PostProcessingIndex) {
	ReadBuffer->BindForReading();
	WriteBuffer->BindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	GetPostProcessShader(PostProcessingIndex)->Bind();
	glUniform3fv(glGetUniformLocation(GetPostProcessShader(PostProcessingIndex)->GetProgram(), "CAMERA_POS"), 1, &Camera->GetTransform().GetLocation()[0]);
	glUniform1f(glGetUniformLocation(GetPostProcessShader(PostProcessingIndex)->GetProgram(), "NEAR_CLIP"), Camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(GetPostProcessShader(PostProcessingIndex)->GetProgram(), "FAR_CLIP"), Camera->GetFarClipPlane());


	for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(GetPostProcessShader(PostProcessingIndex)->GetProgram(), ReadBuffer->GetTextureName(i).c_str()), i);
	}
	DrawScreenQuad();
}
void DefferedCompositor::OutputToScreen(RenderTarget* ReadBuffer) {
	ReadBuffer->BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_FinalOutputShader->Bind();


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(S_FinalOutputShader->GetProgram(), ReadBuffer->GetTextureName(0).c_str()), 0);

	DrawScreenQuad();
}
void DefferedCompositor::DrawScreenQuad() {
	if (quadVAO == 0) {
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};

		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}