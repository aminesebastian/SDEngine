#include "DefferedCompositor.h"
#include <iostream>
#include "Texture2D.h"


DefferedCompositor::DefferedCompositor() {
	Shader tempShader("./Res/DefferedLighting");
	S_LightingShader = tempShader;
	S_LightingShader.RecompileShader();
}
DefferedCompositor::~DefferedCompositor() {}

void DefferedCompositor::Init(unsigned int WindowWidth, unsigned int WindowHeight) {

}
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
void DefferedCompositor::Composite(GBuffer* Buffer) {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	S_LightingShader.Bind();

	string uniformNames[8]{"worldPosition", "albedo", "roughness", "metalness", "emissive", "AO", "normal", "texCoord"};

	Texture2D albdeo("./res/T_BookBaseColor.tga");
	for (int i = 0; i < Buffer->GBUFFER_NUM_TEXTURES; i++) {
		GLuint tempLocation = glGetUniformLocation(S_LightingShader.GetProgram(), uniformNames[i].c_str());
		glUniform1i(tempLocation, i);
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D, Buffer->S_Textures[i]);
		//glBindTexture(GL_TEXTURE_2D, albdeo.GetTexture());
	}

	DrawToScreen();
}

//std::string uniformNames[8] = { "worldPosition", "albedo", "roughness", "metalness", "emissive", "AO", "normal", "texCoord" };
//
//for (int i = 0; i < Buffer.GBUFFER_NUM_TEXTURES; i++) {
//	glUniform1i(glGetUniformLocation(LightingShader.S_Program, uniformNames[i].c_str()), 0);
//	glActiveTexture(GL_TEXTURE0 + i);
//	glBindTexture(GL_TEXTURE_2D, Buffer.GetTexture(i));
//}