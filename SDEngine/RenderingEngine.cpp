#include "RenderingEngine.h"
#include "Shader.h"
#include "World.h"
#include "GBuffer.h"
#include "Entity.h"
#include "Light.h"
#include "Display.h"
#include "DefferedCompositor.h"
#include <GLEW/glew.h>

URenderingEngine::URenderingEngine(Display* Display) {
	S_Buffer1 = new GBuffer();
	S_Buffer2 = new GBuffer();
	S_TranslucencyBuffer = new GBuffer();
	S_Display = Display;
	S_DefferedCompositor = new DefferedCompositor("Res/Shaders/DefferedLighting");
	S_TranslucencyBlendShader = new Shader("./Res/Shaders/TranslucencyCompositor");
	S_Buffer1->Init(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
	S_Buffer2->Init(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
	S_TranslucencyBuffer->Init(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
}
URenderingEngine::~URenderingEngine() {

}

void URenderingEngine::ChangeShader(std::string ShaderName) { S_Shader = new Shader(ShaderName); }
void URenderingEngine::RecompileShaders(UWorld* World) {
	S_Shader->RecompileShader(); 
	S_DefferedCompositor->RecompileShaders();
	S_TranslucencyBlendShader->RecompileShader();
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
		if (temp && temp->GetMaterial() != NULL) {
			temp->GetMaterial()->GetShader()->RecompileShader();
		}
	}
}

void URenderingEngine::DebugGBuffer() {
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
	GBuffer* currentBuffer = GetReadGBuffer();
	currentBuffer->BindForReading();

	GLint ThirdWidth = (GLint)(S_Display->GetDimensions().x / 2.0f);
	GLint ThirdHeight = (GLint)(S_Display->GetDimensions().y / 2.0f);

	GLint FirstThirdWidth = (GLint)(S_Display->GetDimensions().x / 2.0f);
	GLint FirstThirdHeight = (GLint)(S_Display->GetDimensions().y / 2.0f);
	GLint SecondThirdWidth = (GLint)(S_Display->GetDimensions().x / 2.0f);
	GLint SecondThirdHeight = (GLint)(S_Display->GetDimensions().y / 2.0f);

	currentBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_ALBEDO);
	glBlitFramebuffer(0, 0, S_Display->GetDimensions().x, S_Display->GetDimensions().y, 0, ThirdHeight, ThirdWidth, S_Display->GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	currentBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_NORMAL);
	glBlitFramebuffer(0, 0, S_Display->GetDimensions().x, S_Display->GetDimensions().y, 0, 0, ThirdWidth, ThirdHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

	currentBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_POSITION);
	glBlitFramebuffer(0, 0, S_Display->GetDimensions().x, S_Display->GetDimensions().y, ThirdWidth, ThirdHeight, S_Display->GetDimensions().x, S_Display->GetDimensions().y, GL_COLOR_BUFFER_BIT, GL_LINEAR);
	
	currentBuffer->SetReadBuffer(GBuffer::GBUFFER_TEXTURE_TYPE_RMAO);
	glBlitFramebuffer(0, 0, S_Display->GetDimensions().x, S_Display->GetDimensions().y, ThirdWidth, 0, S_Display->GetDimensions().x, ThirdHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR);

}
int URenderingEngine::GetTranslucentObjectCount(UWorld* World) {
	int count = 0;
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() != EShaderModel::TRANSLUCENT) {
					count++;
				}
			}
		}
	}
	return count;
}
void URenderingEngine::GemoetryPass(UWorld* World, Camera* Camera, GBuffer* WriteBuffer) {
	WriteBuffer->BindForWriting();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() != EShaderModel::TRANSLUCENT) {
					World->GetWorldEntities()[i]->Draw(Camera);
				}
			}
			else {
				World->GetWorldEntities()[i]->Draw(Camera);
			}
		}
	}
	for (int i = 0; i < World->GetWorldLights().size(); i++) {
		if (World->GetWorldLights()[i]->IsVisible()) {
			World->GetWorldLights()[i]->Draw(Camera);
		}
	}
}
void URenderingEngine::TranslucencyPass(UWorld* World, Camera* Camera, GBuffer* ReadBuffer, GBuffer* WriteBuffer) {
	WriteBuffer->BindForWriting();
	ReadBuffer->BindForReading();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, WriteBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY, &vec3(0.0f, 0.0f, 0.0f)[0]);
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() == EShaderModel::TRANSLUCENT) {
					temp->GetMaterial()->GetShader()->Bind();

					string uniformNames[9]{ "worldPosition", "albedo", "RMAO", "normal", "texCoord", "matID", "HDR", "finalComp" };
					for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
						glEnable(GL_TEXTURE_2D);
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
						glUniform1i(glGetUniformLocation(temp->GetMaterial()->GetShader()->GetProgram(), uniformNames[i].c_str()), i);
					}
					for (int i = 0; i < World->GetWorldLights().size(); i++) {
						World->GetWorldLights()[i]->SendShaderInformation(temp->GetMaterial()->GetShader(), i);
					}
					World->GetWorldEntities()[i]->Draw(Camera);
				}
			}
		}
	}
}
void URenderingEngine::BlendTransparencyPass(UWorld* World, Camera* Camera, GBuffer* TranslucencyBuffer, GBuffer* ReadBuffer, GBuffer* WriteBuffer) {
	TranslucencyBuffer->BindForReading();
	WriteBuffer->BindForWriting();
	S_TranslucencyBlendShader->Bind();

	for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
		glUniform1i(glGetUniformLocation(S_TranslucencyBlendShader->GetProgram(), ReadBuffer->GetGBufferTextureName(i).c_str()), i);
	}
	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0 + TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY);
	glBindTexture(GL_TEXTURE_2D, TranslucencyBuffer->GetTexture(TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY));
	glUniform1i(glGetUniformLocation(S_TranslucencyBlendShader->GetProgram(), TranslucencyBuffer->GetGBufferTextureName(TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY).c_str()), TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY);
	S_DefferedCompositor->DrawScreenQuad();
}
void URenderingEngine::RenderWorld(UWorld* World, Camera* Camera) {
	S_CurrentStage = ERenderingStage::GEOMETRY;
	GemoetryPass(World, Camera, GetFreeGBuffer());
	FlipCurrentBufferIndex();

	S_CurrentStage = ERenderingStage::LIGHTING;
	S_DefferedCompositor->CompositeLighting(GetReadGBuffer(), GetFreeGBuffer(), World->GetWorldLights(), Camera);
	FlipCurrentBufferIndex();

	S_CurrentStage = ERenderingStage::TRANSLUCENCY;
	if (GetTranslucentObjectCount(World) > 0) {
		TranslucencyPass(World, Camera, GetReadGBuffer(), S_TranslucencyBuffer);
		BlendTransparencyPass(World, Camera, S_TranslucencyBuffer, GetReadGBuffer(), GetFreeGBuffer());
		FlipCurrentBufferIndex();
	}

	S_CurrentStage = ERenderingStage::POST_PROCESSING;
	for (int i = 0; i < S_DefferedCompositor->GetPostProcessShaderCount(); i++) {
		S_DefferedCompositor->CompositePostProcesing(GetReadGBuffer(), GetFreeGBuffer(), Camera, i);
		FlipCurrentBufferIndex();
	}

	S_CurrentStage = ERenderingStage::OUTPUT;
	S_DefferedCompositor->OutputToScreen(GetReadGBuffer());
}
void URenderingEngine::FlipCurrentBufferIndex() {
	S_CurrentBuffer == 1 ? S_CurrentBuffer = 2 : S_CurrentBuffer = 1;

}