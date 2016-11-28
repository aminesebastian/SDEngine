#include "DefferedCompositor.h"
#include <iostream>
#include <string>
#include "Texture2D.h"


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
void DefferedCompositor::Composite(GBuffer* Buffer, vector<Light>& Lights, Camera* camera) {
	S_LightingShader.Bind();
	glUniform3fv(glGetUniformLocation(S_LightingShader.GetProgram(), "cameraPos"), 1, &camera->GetCameraTransform().GetPosition()[0]);
	for (GLuint i = 0; i < Lights.size(); i++) {
		Lights[i].SendShaderInformation(S_LightingShader, i);
	}

	string uniformNames[8]{"worldPosition", "albedo", "emissive", "RMAO", "normal", "texCoord"};

	for (int i = 0; i < Buffer->GBUFFER_NUM_TEXTURES; i++) {	
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Buffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_LightingShader.GetProgram(), uniformNames[i].c_str()), i);
	}
	DrawToScreen();
}
