#include "DefferedCompositor.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Engine/Window.h"
#include "Core/Objects/Entities/Camera.h"

DefferedCompositor::DefferedCompositor(RenderViewport* OwningViewport) : OwningViewport(OwningViewport) {
	S_LightingShader    = new Shader("./Res/Shaders/DefferedLighting", false);
	S_FinalOutputShader = (new Shader("./Res/Shaders/PostProcessing/Output", false));
	quadVBO             = 0;

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
DefferedCompositor::~DefferedCompositor() {}

void DefferedCompositor::RecompileShaders() {
	S_LightingShader->RecompileShader();
	S_FinalOutputShader->RecompileShader();
	for (int i = 0; i < S_PostProcessingShaders.size(); i++) {
		S_PostProcessingShaders[i]->RecompileShader();
	}
}
void DefferedCompositor::CompositeLighting(GBuffer* GBufferIn, RenderTarget* WriteBuffer, SArray<Light*> Lights, const Camera* RenderCamera) {
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

	glUniform3fv(glGetUniformLocation(S_LightingShader->GetProgram(), "CAMERA_POS"), 1, &RenderCamera->GetTransform().GetLocation()[0]);
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "NEAR_CLIP"), RenderCamera->GetNearClipPlane());
	glUniform1f(glGetUniformLocation(S_LightingShader->GetProgram(), "FAR_CLIP"), RenderCamera->GetFarClipPlane());
	glUniform1i(glGetUniformLocation(S_LightingShader->GetProgram(), "LIGHT_COUNT"), (GLint)lightCount);
	glUniform2fv(glGetUniformLocation(S_LightingShader->GetProgram(), "SCREEN_RES"), 1, &OwningViewport->GetOwningWindow()->GetDimensions()[0]);

	GBufferIn->BindTextures(S_LightingShader);



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
void DefferedCompositor::DrawScreenQuad() const {
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}