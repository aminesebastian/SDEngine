#include "DefferedCompositor.h"
#include <iostream>
#include <string>
#include "Texture2D.h"
#include "Camera.h"

DefferedCompositor::DefferedCompositor(string LightingShader) {
	S_LightingShader = new Shader(LightingShader);
	S_PostProcessShader = new Shader("Res/PostProcess");

}
DefferedCompositor::~DefferedCompositor() {}

void DefferedCompositor::CompositeLighting(GBuffer* ReadBuffer, GBuffer* WriteBuffer, vector<Light*> Lights, Camera* Camera) {
	WriteBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_LightingShader->Bind();
	glUniform3fv(glGetUniformLocation(S_LightingShader->GetProgram(), "CAMERA_POS"), 1, &Camera->GetTransform().GetPosition()[0]);
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "NEAR_CLIP"), Camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "FAR_CLIP"), Camera->GetFarClipPlane());

	for (GLuint i = 0; i < Lights.size(); i++) {
		Lights[i]->SendShaderInformation(S_LightingShader, i);
	}
	
	string uniformNames[9]{ "worldPosition", "albedo", "RMAO", "normal", "texCoord", "matID", "HDR", "finalComp" };

	for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_LightingShader->GetProgram(), uniformNames[i].c_str()), i);
	}

	DrawToScreen();
}
void DefferedCompositor::CompositePostProcesing(GBuffer* ReadBuffer, GBuffer* WriteBuffer, Camera* Camera) {
	ReadBuffer->BindForReading();
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_PostProcessShader->Bind();
	glUniform3fv(glGetUniformLocation(S_PostProcessShader->GetProgram(), "CAMERA_POS"), 1, &Camera->GetTransform().GetPosition()[0]);
	glUniform1f(glGetUniformLocation(S_PostProcessShader->GetProgram(), "NEAR_CLIP"), Camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_PostProcessShader->GetProgram(), "FAR_CLIP"), Camera->GetFarClipPlane());

	string uniformNames[9]{ "worldPosition", "albedo", "RMAO", "normal", "texCoord", "matID", "HDR", "finalComp"};

	for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_PostProcessShader->GetProgram(), uniformNames[i].c_str()), i);
	}

	DrawToScreen();
}
void DefferedCompositor::DrawToScreen() {
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