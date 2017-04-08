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
void URenderingEngine::RecompileShaders() {
	S_Shader->RecompileShader(); 
	S_DefferedCompositor->RecompileShaders();
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
void URenderingEngine::CopyToFreeGBuffer() {

}
void URenderingEngine::RenderWorld(UWorld* World, Camera* Camera) {
	GetFreeGBuffer()->BindForWriting();
	S_Shader->Bind();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		S_Shader->Update(World->GetWorldEntities()[i]->GetTransform(), Camera);
		if (World->GetWorldEntities()[i]->IsVisible()) {
			World->GetWorldEntities()[i]->Draw(*S_Shader);
		}
	}
	for (int i = 0; i < World->GetWorldLights().size(); i++) {
		S_Shader->Update(World->GetWorldLights()[i]->GetTransform(), Camera);
		if (World->GetWorldLights()[i]->IsVisible()) {
			World->GetWorldLights()[i]->Draw(*S_Shader);
		}
	}
	FlipCurrentBufferIndex();

	S_DefferedCompositor->CompositeLighting(GetReadGBuffer(), GetFreeGBuffer(), World->GetWorldLights(), Camera);
	//CopyToFreeGBuffer();
	FlipCurrentBufferIndex();

	/*Writing straight to screen, ignore write buffer*/
	for (int i = 0; i < S_DefferedCompositor->GetPostProcessShaderCount(); i++) {
		S_DefferedCompositor->CompositePostProcesing(GetReadGBuffer(), GetFreeGBuffer(), Camera, i);
		FlipCurrentBufferIndex();
	}

	//FlipCurrentBufferIndex();

	S_DefferedCompositor->OutputToScreen(GetReadGBuffer());
}