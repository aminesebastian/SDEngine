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
void DefferedCompositor::Composite(GBuffer* Buffer, vector<Light*>& Lights) {
	S_LightingShader.Bind();

	for (GLuint i = 0; i < Lights.size(); i++) {
		glUniform1f(glGetUniformLocation(S_LightingShader.GetProgram(), ("lights[" + std::to_string(i) + "].Intensity").c_str()), Lights[i]->GetLightInfo().Intensity);
		glUniform1f(glGetUniformLocation(S_LightingShader.GetProgram(), ("lights[" + std::to_string(i) + "].Attenuation").c_str()), Lights[i]->GetLightInfo().Attenuation);
		glUniform3fv(glGetUniformLocation(S_LightingShader.GetProgram(), ("lights[" + std::to_string(i) + "].Color").c_str()), 1, &Lights[i]->GetLightInfo().Color[0]);
		glUniform3fv(glGetUniformLocation(S_LightingShader.GetProgram(), ("lights[" + std::to_string(i) + "].Position").c_str()), 1, &Lights[i]->GetTransform().GetPosition()[0]);
		glUniform3fv(glGetUniformLocation(S_LightingShader.GetProgram(), ("lights[" + std::to_string(i) + "].Direction").c_str()), 1, &Lights[i]->GetTransform().GetForwardVector()[0]);
	}

	string uniformNames[8]{"worldPosition", "albedo", "roughness", "metalness", "emissive", "AO", "normal", "texCoord"};

	for (int i = 0; i < Buffer->GBUFFER_NUM_TEXTURES; i++) {	
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, Buffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_LightingShader.GetProgram(), uniformNames[i].c_str()), i);
	}
	DrawToScreen();
}
