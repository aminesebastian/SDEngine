#include "RenderViewport.h"
#include "Rendering/Shader.h"
#include "Engine/World.h"
#include "Rendering/GBuffer.h"
#include "Entities/Entity.h"
#include "Entities/Light.h"
#include "Entities/TransformGizmo.h"
#include "Display.h"
#include "Rendering/DefferedCompositor.h"
#include <GLEW/glew.h>
#include "Engine/EngineStatics.h"
#include "Rendering/PostProcessing/ToneMapper.h"
#include "Rendering/PostProcessing/BloomPostProcessing.h"
#include "Rendering/PostProcessing/SSAOPostProcessing.h"
#include "Rendering/PostProcessing/MotionBlurPostProcessing.h"
#include "Rendering/PostProcessing/PostProcessingLayer.h"
#include "Rendering/PostProcessing/DOFPostProcessing.h"
#include "Rendering/Utilities/VariableGausianBlur.h"
#include "Rendering/RenderTarget.h"
#include "Utilities/Logger.h"
#include "SAsset.h"
#include "AssetManager.h"

RenderViewport::RenderViewport(vec2 RenderTargetDimensions) : RenderTargetDimensions(RenderTargetDimensions) {
	CurrentBuffer             = 0;
	bDebugMode                = false;
	bInitialized              = false;
	S_DefferedCompositor      = new DefferedCompositor("Res/Shaders/DefferedLighting");
	S_TranslucencyBlendShader = new Shader("./Res/Shaders/TranslucencyCompositor", false);
	SelectionOutlineShader    = new Shader("./Res/Shaders/SelectionOutlineShader", false);
	S_GBuffer                 = nullptr;
	S_TranslucencyBuffer      = nullptr;
	S_OutputBuffer1           = nullptr;
	S_OutputBuffer2           = nullptr;
}
RenderViewport::~RenderViewport() {

}
void RenderViewport::Initialize() {
	if (bInitialized) {
		return;
	}

 	SD_ENGINE_INFO("Initializing Viewport")
	TransformGizmoEntity = new TransformGizmo(this);
	GenerateRenderTargets();
	RegisterPostProcessEffects();
	bInitialized = true;
}
void RenderViewport::GenerateRenderTargets() {
	if (!bInitialized) {
		SD_ENGINE_INFO("Generating Viewport Render Targets")
	} else {
		SD_ENGINE_INFO("Regenerating Viewport Render Targets")
	}

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

	S_GBuffer = new GBuffer(RenderTargetDimensions);
	S_GBuffer->FinalizeRenderTarget();

	S_TranslucencyBuffer = new GBuffer(RenderTargetDimensions);
	S_TranslucencyBuffer->FinalizeRenderTarget();

	S_OutputBuffer1 = new RenderTarget(RenderTargetDimensions);
	S_OutputBuffer1->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_OutputBuffer1->AddDepthStencilBuffer();
	S_OutputBuffer1->FinalizeRenderTarget();

	S_OutputBuffer2 = new RenderTarget(RenderTargetDimensions);
	S_OutputBuffer2->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_LINEAR_MIPMAP_LINEAR, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	S_OutputBuffer2->AddDepthStencilBuffer();
	S_OutputBuffer2->FinalizeRenderTarget();
}
void RenderViewport::RegisterPostProcessEffects() {
	if (!bInitialized) {
		SD_ENGINE_INFO("Creating Post Process Stack")
	} else {
		SD_ENGINE_INFO("Recreating Post Process Stack")
	}

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


void RenderViewport::RecompileShaders(UWorld* World) {
	S_DefferedCompositor->RecompileShaders();
	S_TranslucencyBlendShader->RecompileShader();
	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		StaticMesh* temp = dynamic_cast<StaticMesh*>(World->GetWorldEntities()[i]);
		if (temp && temp->GetMaterial() != nullptr) {
			temp->GetMaterial()->GetShader()->RecompileShader();
		}
	}

	TransformGizmoEntity->RecompileShaders();

	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		layerPair->RecompileShaders();
	}
	EngineStatics::RecompileShaders();
	SelectionOutlineShader->RecompileShader();
}
SArray<PostProcessingLayer*> RenderViewport::GetPostProcessingLayers() {
	return S_PostProcessingLayers;
}
void RenderViewport::EnablePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->Enable();
			return;
		}
	}
}
void RenderViewport::DisablePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->Disable();
			return;
		}
	}
}
void RenderViewport::TogglePostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			layerPair->ToggleEnabled();
			return;
		}
	}
}
bool RenderViewport::IsPostProcessingLayerEnabled(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			return layerPair->IsEnabled();
		}
	}
	return false;
}
PostProcessingLayer* RenderViewport::GetPostProcessingLayer(TString Layer) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		if (layerPair->GetPostProcessingLayerName() == Layer) {
			return layerPair;
		}
	}
	return nullptr;
}

void RenderViewport::FlipOutputBuffers() {
	CurrentBuffer = ((CurrentBuffer + 1) % 2);
}

void RenderViewport::RenderWorld(UWorld* World, Camera* Camera) {
	CurrentBuffer = 0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearBufferfv(GL_COLOR, S_GBuffer->GBUFFER_TEXTURE_TYPE_POSITION, &vec3(0.0f, 0.0f, 0.0f)[0]);

	for (Entity* entity : World->GetWorldEntities()) {
		entity->PreFrameRendered();
	}

	S_CurrentStage = ERenderingStage::GEOMETRY;
	GemoetryPass(World, Camera);

	S_CurrentStage = ERenderingStage::LIGHTING;
	S_DefferedCompositor->CompositeLighting(S_GBuffer, GetCurrentOutputBuffer(), World->GetWorldLights(), Camera);

	S_CurrentStage = ERenderingStage::POST_PROCESSING;
	RenderPostProcessing(World, Camera);
	
	S_CurrentStage = EDITOR_ELEMENTS;
	RenderEditorElements(World, Camera);

	S_CurrentStage = ERenderingStage::OUTPUT;
	S_DefferedCompositor->OutputToScreen(GetCurrentOutputBuffer());

	for (Entity* entity : World->GetWorldEntities()) {
		entity->PostFrameRendered();
	}
}
void RenderViewport::GemoetryPass(UWorld* World, Camera* Camera) {
	S_GBuffer->BindForWriting();

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < World->GetWorldEntities().size(); i++) {
		Entity* entity = World->GetWorldEntities()[i];
		if (World->GetWorldEntities()[i]->IsVisible()) {
			if (Engine::GetInstance()->GetSelectedEntity() == entity) {
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}
			entity->Draw(Camera);
			if (Engine::GetInstance()->GetSelectedEntity() == entity) {
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
}
void RenderViewport::TranslucencyPass(UWorld* World, Camera* Camera) {
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
void RenderViewport::BlendTransparencyPass(UWorld* World, Camera* Camera) {
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
void RenderViewport::RenderPostProcessing(UWorld* World, Camera* Camera) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		// Skip any layers that are disabled.
		if (!layerPair->IsEnabled()) {
			continue;
		}

		FlipOutputBuffers();
		layerPair->RenderLayer(S_DefferedCompositor, Camera, S_GBuffer, GetPreviousOutputBuffer(), GetCurrentOutputBuffer());
	}
}
void RenderViewport::RenderEditorElements(UWorld* World, Camera* Camera) {
	// Draw selection outline if exists.
	Entity* selectedEntity = Engine::GetInstance()->GetSelectedEntity();
	if (selectedEntity) {
		vec2 resolution = GetCurrentOutputBuffer()->GetRenderTargetResolution();

		S_GBuffer->BindForReading();
		GetCurrentOutputBuffer()->BindForWriting();
		glBlitFramebuffer(0, 0, (GLint)resolution.x, (GLint)resolution.y, 0, 0, (GLint)resolution.x, (GLint)resolution.y, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		SelectionOutlineShader->Bind();
		SelectionOutlineShader->Update(selectedEntity->GetTransform(), selectedEntity->GetLastFrameTransform(), Camera);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glLineWidth(5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		selectedEntity->Draw(Camera, true);

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_STENCIL_TEST);

		glClear(GL_DEPTH_BUFFER_BIT);
		TransformGizmoEntity->UpdateVisibility(true);
		TransformGizmoEntity->SetLocation(selectedEntity->GetLocation());
		TransformGizmoEntity->Draw(Camera);
	} else {
		TransformGizmoEntity->UpdateVisibility(false);
	}
}

void RenderViewport::OnMouseMove(vec2 MouseCoords) {
	Entity* selectedEntity = Engine::GetInstance()->GetSelectedEntity();
	if (selectedEntity) {
		TransformGizmoEntity->OnMouseMove();
	}
}

RenderTarget* RenderViewport::GetCurrentOutputBuffer() {
	return CurrentBuffer == 0 ? S_OutputBuffer1 : S_OutputBuffer2;
}
RenderTarget* RenderViewport::GetPreviousOutputBuffer() {
	return CurrentBuffer == 0 ? S_OutputBuffer2 : S_OutputBuffer1;
}
bool RenderViewport::GetDebugEnabled() {
	return bDebugMode;
}
void RenderViewport::SetDebugEnabled(bool bEnabled) {
	bDebugMode = bEnabled;
}
EDebugState RenderViewport::GetDebugState() {
	return S_DebugState;
}
void RenderViewport::SetDebugState(EDebugState NewState) {
	S_DebugState = NewState;
}
int RenderViewport::GetTranslucentObjectCount(UWorld* World) {
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
void RenderViewport::ChangeRenderTargetDimensions(vec2 NewRenderTargetDimensions) {
	SD_ENGINE_INFO("Changing render target dimensions for viewport from {0} X {1} to {2} x {3}", (int)RenderTargetDimensions.x, (int)RenderTargetDimensions.y, (int)NewRenderTargetDimensions.x, (int)NewRenderTargetDimensions.y);
	RenderTargetDimensions = NewRenderTargetDimensions;
	GenerateRenderTargets();
	RegisterPostProcessEffects();
}
vec2 RenderViewport::GetRenderTargetDimensions() {
	return RenderTargetDimensions;
}