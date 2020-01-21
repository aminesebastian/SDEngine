#include "RenderingEngine.h"
#include "Shader.h"
#include "World.h"
#include "GBuffer.h"
#include "Entity.h"
#include "Light.h"
#include "Display.h"
#include "DefferedCompositor.h"
#include <GLEW/glew.h>
#include "EngineStatics.h"
#include "ToneMapper.h"
#include "BloomPostProcessing.h"
#include "SSAOPostProcessing.h"
#include "MotionBlurPostProcessing.h"
#include "PostProcessingLayer.h"
#include "DOFPostProcessing.h"
#include "VariableGausianBlur.h"
#include "RenderTarget.h"

URenderingEngine::URenderingEngine(vec2 RenderTargetDimensions) : RenderTargetDimensions(RenderTargetDimensions) {
	CurrentBuffer = 0;

	bDebugMode = false;
	S_DefferedCompositor = new DefferedCompositor("Res/Shaders/DefferedLighting");
	S_TranslucencyBlendShader = new Shader("./Res/Shaders/TranslucencyCompositor", false);

	S_GBuffer = nullptr;
	S_TranslucencyBuffer = nullptr;
	S_OutputBuffer1 = nullptr;
	S_OutputBuffer2 = nullptr;

	GenerateRenderTargets();
	RegisterPostProcessEffects();
}
URenderingEngine::~URenderingEngine() {

}
void URenderingEngine::GenerateRenderTargets() {
	if (S_GBuffer != nullptr) {
		delete S_GBuffer;
	}
	if (S_TranslucencyBuffer != nullptr) {
		delete S_TranslucencyBuffer;
	}
	if (S_OutputBuffer1 != nullptr) {
		delete S_OutputBuffer1;
	}
	if (S_OutputBuffer2 != nullptr) {
		delete S_OutputBuffer2;
	}

	S_GBuffer = new GBuffer();
	S_GBuffer->Init((unsigned int)RenderTargetDimensions.x, (unsigned int)RenderTargetDimensions.y);

	S_TranslucencyBuffer = new GBuffer();
	S_TranslucencyBuffer->Init((unsigned int)RenderTargetDimensions.x, (unsigned int)RenderTargetDimensions.y);

	S_OutputBuffer1 = new RenderTarget();
	S_OutputBuffer1->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_OutputBuffer1->Init((unsigned int)RenderTargetDimensions.x, (unsigned int)RenderTargetDimensions.y);

	S_OutputBuffer2 = new RenderTarget();
	S_OutputBuffer2->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_OutputBuffer2->Init((unsigned int)RenderTargetDimensions.x, (unsigned int)RenderTargetDimensions.y);
}
void URenderingEngine::RegisterPostProcessEffects() {
	for (PostProcessingLayer* layer : S_PostProcessingLayers) {
		if (layer) {
			delete layer;
		}
	}
	S_PostProcessingLayers.clear();
	S_PostProcessingLayers.emplace_back(new SSAOPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.emplace_back(new BloomPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.emplace_back(new MotionBlurPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.emplace_back(new ToneMapper(RenderTargetDimensions));
}

void URenderingEngine::ChangeRenderTargetDimensions(vec2 NewRenderTargetDimensions) {
	RenderTargetDimensions = NewRenderTargetDimensions;
	GenerateRenderTargets();
	RegisterPostProcessEffects();
}
vec2 URenderingEngine::GetRenderTargetDimensions() {
	return RenderTargetDimensions;
}

SArray<PostProcessingLayer*> URenderingEngine::GetPostProcessingLayers() {
	return S_PostProcessingLayers;
}
void URenderingEngine::EnablePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->Enable();
			return;
		}
	}
}
void URenderingEngine::DisablePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->Disable();
			return;
		}
	}
}
void URenderingEngine::TogglePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->ToggleEnabled();
			return;
		}
	}
}
bool URenderingEngine::IsPostProcessingLayerEnabled(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			return layerPair->IsEnabled();
		}
	}
	return false;
}
PostProcessingLayer* URenderingEngine::GetPostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			return layerPair;
		}
	}
	return nullptr;
}

void URenderingEngine::RecompileShaders(UWorld* World) {
	S_DefferedCompositor->RecompileShaders();
	S_TranslucencyBlendShader->RecompileShader();
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
		if (temp && temp->GetMaterial() != NULL) {
			temp->GetMaterial()->GetShader()->RecompileShader();
		}
	}
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		layerPair->RecompileShaders();
	}
	EngineStatics::GetLightDebugShader()->RecompileShader();
	EngineStatics::GetDefaultGeometryPassShader()->RecompileShader();
	EngineStatics::GetShadowShader()->RecompileShader();
	EngineStatics::GetLineShader()->RecompileShader();
	EngineStatics::GetGausBlur7x1Shader()->RecompileShader();
}
int URenderingEngine::GetTranslucentObjectCount(UWorld* World) {
	int count = 0;
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() == EShaderModel::TRANSLUCENT) {
					count++;
				}
			}
		}
	}
	return count;
}
void URenderingEngine::GemoetryPass(UWorld* World, Camera* Camera) {
	S_GBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		if (World->GetWorldEntities()[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
			if (temp) {
				if (temp->GetMaterial()->GetShaderModel() != EShaderModel::TRANSLUCENT) {
					World->GetWorldEntities()[i]->Draw(Camera);
				}
			} else {
				World->GetWorldEntities()[i]->Draw(Camera);
			}
		}
	}

	if (!Engine::GetInstance()->IsInGameMode()) {
		for (int i = 0; i < World->GetWorldEntities().size(); i++) {
			if (World->GetWorldEntities()[i]->HasAxisAlignedBoundingBox()) {
				World->GetWorldEntities()[i]->GetAxisAlignedBoundingBox()->DrawDebug(vec3(1.0f, 0.5f, 1.0f), Camera, World->GetWorldEntities()[i]->GetTransform());
			}
		}
		for (int i = 0; i < World->GetWorldLights().size(); i++) {
			World->GetWorldLights()[i]->Draw(Camera);
			if (World->GetWorldLights()[i]->HasAxisAlignedBoundingBox()) {
				World->GetWorldLights()[i]->GetAxisAlignedBoundingBox()->DrawDebug(World->GetWorldLights()[i]->GetLightInfo().Color, Camera, World->GetWorldLights()[i]->GetTransform());
			}
		}
	}
}
void URenderingEngine::TranslucencyPass(UWorld* World, Camera* Camera) {
	//WriteBuffer->BindForWriting();
	//ReadBuffer->BindForReading();
	//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClearBufferfv(GL_COLOR, WriteBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY, &vec3(0.0f, 0.0f, 0.0f)[0]);
	//for (int i = 0; i < World->GetWorldEntities().size(); i++) {
	//	if (World->GetWorldEntities()[i]->IsVisible()) {
	//		StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
	//		if (temp) {
	//			if (temp->GetMaterial()->GetShaderModel() == EShaderModel::TRANSLUCENT) {
	//				temp->GetMaterial()->GetShader()->Bind();
	//				for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
	//					glEnable(GL_TEXTURE_2D);
	//					glActiveTexture(GL_TEXTURE0 + i);
	//					glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
	//					glUniform1i(glGetUniformLocation(temp->GetMaterial()->GetShader()->GetProgram(), ReadBuffer->GetTextureName(i).c_str()), i);
	//				}
	//				for (int i = 0; i < World->GetWorldLights().size(); i++) {
	//					World->GetWorldLights()[i]->SendShaderInformation(temp->GetMaterial()->GetShader(), i);
	//				}
	//				World->GetWorldEntities()[i]->Draw(Camera);
	//			}
	//		}
	//	}
	//}
}
void URenderingEngine::BlendTransparencyPass(UWorld* World, Camera* Camera) {
	//S_TranslucencyBuffer->BindForReading();
	//WriteBuffer->BindForWriting();
	//S_TranslucencyBlendShader->Bind();

	//for (int i = 0; i < ReadBuffer->GBUFFER_NUM_TEXTURES; i++) {
	//	glEnable(GL_TEXTURE_2D);
	//	glActiveTexture(GL_TEXTURE0 + i);
	//	glBindTexture(GL_TEXTURE_2D, ReadBuffer->GetTexture(i));
	//	glUniform1i(glGetUniformLocation(S_TranslucencyBlendShader->GetProgram(), ReadBuffer->GetTextureName(i).c_str()), i);
	//}
	//glEnable(GL_TEXTURE_2D);
	//glActiveTexture(GL_TEXTURE0 + TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY);
	//glBindTexture(GL_TEXTURE_2D, TranslucencyBuffer->GetTexture(TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY));
	//glUniform1i(glGetUniformLocation(S_TranslucencyBlendShader->GetProgram(), TranslucencyBuffer->GetTextureName(TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY).c_str()), TranslucencyBuffer->GBUFFER_TEXTURE_TYPE_TRANSLUCENCY);
	//S_DefferedCompositor->DrawScreenQuad();
}

void URenderingEngine::RenderWorld(UWorld* World, Camera* Camera) {
	for (Entity* entity : World->GetWorldEntities()) {
		entity->PreFrameRendered();
	}

	CurrentBuffer = 0;

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, S_GBuffer->GBUFFER_TEXTURE_TYPE_POSITION, &vec3(0.0f, 0.0f, 0.0f)[0]);

	S_CurrentStage = ERenderingStage::GEOMETRY;
	GemoetryPass(World, Camera);

	S_CurrentStage = ERenderingStage::LIGHTING;
	S_DefferedCompositor->CompositeLighting(S_GBuffer, GetCurrentOutputBuffer(), World->GetWorldLights(), Camera);

	//S_CurrentStage = ERenderingStage::TRANSLUCENCY;
	//if (GetTranslucentObjectCount(World) > 0) {
	//	TranslucencyPass(World, Camera, GetReadGBuffer(), S_TranslucencyBuffer);
	//	BlendTransparencyPass(World, Camera, S_TranslucencyBuffer, GetReadGBuffer(), GetFreeGBuffer());
	//	FlipCurrentBufferIndex();
	//}

	if (!bDebugMode) {
		S_CurrentStage = ERenderingStage::POST_PROCESSING;

		for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
			// Skip any layers that are disabled.
			if (!layerPair->IsEnabled()) {
				continue;
			}

			FlipOutputBuffers();
			layerPair->RenderLayer(S_DefferedCompositor, Camera, S_GBuffer, GetPreviousOutputBuffer(), GetCurrentOutputBuffer());
		}
	}

	S_CurrentStage = ERenderingStage::OUTPUT;
	S_DefferedCompositor->OutputToScreen(GetCurrentOutputBuffer());

	for (Entity* entity : World->GetWorldEntities()) {
		entity->PostFrameRendered();
	}
}

void URenderingEngine::FlipOutputBuffers() {
	CurrentBuffer = ((CurrentBuffer + 1) % 2);
}
RenderTarget* URenderingEngine::GetCurrentOutputBuffer() {
	return CurrentBuffer == 0 ? S_OutputBuffer1 : S_OutputBuffer2;
}
RenderTarget* URenderingEngine::GetPreviousOutputBuffer() {
	return CurrentBuffer == 0 ? S_OutputBuffer2 : S_OutputBuffer1;
}

bool URenderingEngine::GetDebugEnabled() {
	return bDebugMode;
}
void URenderingEngine::SetDebugEnabled(bool bEnabled) {
	bDebugMode = bEnabled;
}

EDebugState URenderingEngine::GetDebugState() {
	return S_DebugState;
}
void URenderingEngine::SetDebugState(EDebugState NewState) {
	S_DebugState = NewState;
}