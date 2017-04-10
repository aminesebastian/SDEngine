#include "RenderingEngine.h"
#include "Shader.h"
#include "World.h"
#include "GBuffer.h"
#include "Entity.h"
#include "Light.h"
#include "Display.h"
#include "DefferedCompositor.h"

URenderingEngine::URenderingEngine(Display* Display) {
	S_Buffer1 = new GBuffer();
	S_Buffer2 = new GBuffer();
	S_Display = Display;
	S_DefferedCompositor = new DefferedCompositor("Res/Shaders/DefferedLighting");
	S_Buffer1->Init(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
	S_Buffer2->Init(S_Display->GetDimensions().x, S_Display->GetDimensions().y);
}
URenderingEngine::~URenderingEngine() {

}

void URenderingEngine::ChangeShader(std::string ShaderName) { S_Shader = new Shader(ShaderName); }
void URenderingEngine::RecompileShaders(UWorld* World) {
	S_Shader->RecompileShader(); 
	S_DefferedCompositor->RecompileShaders();

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
void URenderingEngine::GemoetryPass(UWorld* World, Camera* Camera, GBuffer* WriteBuffer) {
	WriteBuffer->BindForWriting();
	//S_Shader->Bind();
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

	string uniformNames[1]{ "finalComp" };

	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() == EShaderModel::TRANSLUCENT) {
					for (int i = 0; i < 1; i++) {
						glEnable(GL_TEXTURE_2D);
						glActiveTexture(GL_TEXTURE0 + i);
						glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
						glUniform1i(glGetUniformLocation(temp->GetMaterial()->GetShader()->GetProgram(), uniformNames[i].c_str()), i);
					}
					World->GetWorldEntities()[i]->Draw(Camera);
				}
			}
		}
	}
}
void URenderingEngine::RenderWorld(UWorld* World, Camera* Camera) {
	GemoetryPass(World, Camera, GetFreeGBuffer());

	FlipCurrentBufferIndex();

	S_DefferedCompositor->CompositeLighting(GetReadGBuffer(), GetFreeGBuffer(), World->GetWorldLights(), Camera);

	FlipCurrentBufferIndex();
	int translucentCount = 0;
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp && temp->GetMaterial()->GetShaderModel() == EShaderModel::TRANSLUCENT) {
				translucentCount++;
			}
		}
	}
	if (translucentCount > 0) {
		TranslucencyPass(World, Camera, GetReadGBuffer(), GetFreeGBuffer());
		FlipCurrentBufferIndex();
	}

	for (int i = 0; i < S_DefferedCompositor->GetPostProcessShaderCount(); i++) {
		S_DefferedCompositor->CompositePostProcesing(GetReadGBuffer(), GetFreeGBuffer(), Camera, i);
		FlipCurrentBufferIndex();
	}

	S_DefferedCompositor->OutputToScreen(GetReadGBuffer());
}