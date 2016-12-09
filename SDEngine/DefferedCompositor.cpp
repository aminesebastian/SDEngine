#include "DefferedCompositor.h"
#include <iostream>
#include <string>
#include "Texture2D.h"
#include "Camera.h"

DefferedCompositor::DefferedCompositor() {
	Shader tempShader("./Res/DefferedLighting");
	S_LightingShader = tempShader;
	S_LightingShader.RecompileShader();
}
DefferedCompositor::~DefferedCompositor() {}

void DefferedCompositor::DrawToScreen() {
	if (quadVAO == 0) {
		GLfloat quadVertices[] = {
			-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// Setup plane VAO
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
void DefferedCompositor::CompositeLighting(GBuffer* Buffer, vector<Light>& Lights, Camera* camera) {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	S_LightingShader.Bind();
	glUniform3fv(glGetUniformLocation(S_LightingShader.GetProgram(), "cameraPos"), 1, &camera->GetTransform().GetPosition()[0]);
	glUniform1f(glGetUniformLocation(S_LightingShader.GetProgram(), "NEAR_CLIP"), camera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_LightingShader.GetProgram(), "FAR_CLIP"), camera->GetFarClipPlane());
	for (GLuint i = 0; i < Lights.size(); i++) {
		Lights[i].SendShaderInformation(S_LightingShader, i);
	}
	
	string uniformNames[8]{"worldPosition", "albedo", "RMAO", "emissive", "normal", "texCoord"};

	for (int i = 0; i < Buffer->GBUFFER_NUM_TEXTURES; i++) {	
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Buffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_LightingShader.GetProgram(), uniformNames[i].c_str()), i);
	}
	DrawToScreen();
}
void DefferedCompositor::CompositePostProcesing(GBuffer* Buffer, Camera* Camera) {

}