#include "Core/Rendering/RenderViewport.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Engine/World.h"
#include "Core/Rendering/GBuffer.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Engine/Display.h"
#include "Core/Rendering/DefferedCompositor.h"
#include <GLEW/glew.h>
#include "Core/Engine/EngineStatics.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "Core/Rendering/PostProcessing/ToneMapper.h"
#include "Core/Rendering/PostProcessing/BloomPostProcessing.h"
#include "Core/Rendering/PostProcessing/SSAOPostProcessing.h"
#include "Core/Rendering/PostProcessing/MotionBlurPostProcessing.h"
#include "Core/Rendering/PostProcessing/PostProcessingLayer.h"
#include "Core/Rendering/PostProcessing/DOFPostProcessing.h"
#include "Core/Rendering/Utilities/VariableGausianBlur.h"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Utilities/Logger.h"
#include "Core/Pictorum/PictorumRenderer.h"

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
	CurrentlyActiveShader     = nullptr;
	SD_ENGINE_INFO("Render Viewport Created");
}
RenderViewport::~RenderViewport() {

}
void RenderViewport::Initialize() {
	if (bInitialized) {
		return;
	}

 	SD_ENGINE_INFO("Initializing Viewport")
	GenerateRenderTargets();
	RegisterPostProcessEffects();
	bInitialized = true;



	SD_ENGINE_INFO("Viewport Initialized")
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
	S_PostProcessingLayers.Clear();
	S_PostProcessingLayers.Add(new SSAOPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.Add(new BloomPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.Add(new MotionBlurPostProcessing(RenderTargetDimensions));
	S_PostProcessingLayers.Add(new ToneMapper(RenderTargetDimensions));
}

void RenderViewport::RecompileShaders() {
	S_DefferedCompositor->RecompileShaders();
	S_TranslucencyBlendShader->RecompileShader();
	//TransformGizmoEntity->RecompileShaders();

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

const bool RenderViewport::BindNewShader(Shader* ShaderIn) {
	if (ShaderIn != CurrentlyActiveShader) {
		CurrentlyActiveShader = ShaderIn;
		CurrentlyActiveShader->Bind();
		return true;
	}
	return false;
}

void RenderViewport::RenderWorld(World* RenderWorld, Camera* RenderCamera) {
	CurrentBuffer = 0;

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		actor->PreFrameRendered();
	}

	S_CurrentStage = ERenderingStage::GEOMETRY;
	GemoetryPass(RenderWorld, RenderCamera);

	S_CurrentStage = ERenderingStage::LIGHTING;
	S_DefferedCompositor->CompositeLighting(S_GBuffer, GetCurrentOutputBuffer(), RenderWorld->GetWorldLights(), RenderCamera);

	S_CurrentStage = ERenderingStage::POST_PROCESSING;
	RenderPostProcessing(RenderWorld, RenderCamera);
	
	S_CurrentStage = EDITOR_ELEMENTS;
	RenderEditorElements(RenderWorld, RenderCamera);

	S_CurrentStage = ERenderingStage::OUTPUT;
	S_DefferedCompositor->OutputToScreen(GetCurrentOutputBuffer());

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		actor->PostFrameRendered();
	}
}
void RenderViewport::GemoetryPass(World* RenderWorld, Camera* RenderCamera) {
	S_GBuffer->BindForWriting();

	glClearStencil(0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		if (actor->ShouldBeDrawn(EDrawType::SCENE_RENDER)) {
			if (Engine::GetInstance()->GetSelectedEntity() == actor) {
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}
			actor->Draw(RenderCamera);
			if (Engine::GetInstance()->GetSelectedEntity() == actor) {
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
}
void RenderViewport::TranslucencyPass(World* RenderWorld, Camera* RenderCamera) { }
void RenderViewport::BlendTransparencyPass(World* RenderWorld, Camera* Camera) { }
void RenderViewport::RenderPostProcessing(World* RenderWorld, Camera* Camera) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		// Skip any layers that are disabled.
		if (!layerPair->IsEnabled()) {
			continue;
		}

		FlipOutputBuffers();
		layerPair->RenderLayer(S_DefferedCompositor, Camera, S_GBuffer, GetPreviousOutputBuffer(), GetCurrentOutputBuffer());
	}
}
void RenderViewport::RenderEditorElements(World* RenderWorld, Camera* RenderCamera) {
	// Draw selection outline if exists.
	Entity* selectedEntity = Engine::GetInstance()->GetSelectedEntity();
	if (selectedEntity) {
		vec2 resolution = GetCurrentOutputBuffer()->GetRenderTargetResolution();

		S_GBuffer->BindForReading();
		GetCurrentOutputBuffer()->BindForWriting();
		glBlitFramebuffer(0, 0, (GLint)resolution.x, (GLint)resolution.y, 0, 0, (GLint)resolution.x, (GLint)resolution.y, GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT, GL_NEAREST);

		SelectionOutlineShader->Bind();
		SelectionOutlineShader->Update(selectedEntity->GetTransform(), selectedEntity->GetLastFrameTransform(), RenderCamera);

		glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, -1);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

		glLineWidth(5);
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (selectedEntity->ShouldBeDrawn(EDrawType::EDITOR_OUTLINE_RENDER)) {
			selectedEntity->DrawAdvanced(RenderCamera, EDrawType::EDITOR_OUTLINE_RENDER);
		}

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glDisable(GL_STENCIL_TEST);

		glClear(GL_DEPTH_BUFFER_BIT);
		//TransformGizmoEntity->SetVisibility(true);
		//TransformGizmoEntity->SetLocation(selectedEntity->GetLocation());
		//TransformGizmoEntity->Draw(RenderCamera);
	} else {
		//TransformGizmoEntity->SetVisibility(false);
	}
}

void RenderViewport::OnMouseMove(vec2 MouseCoords) {
	Entity* selectedEntity = Engine::GetInstance()->GetSelectedEntity();
	if (selectedEntity) {
		//TransformGizmoEntity->OnMouseMove();
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
int RenderViewport::GetTranslucentObjectCount(World* World) {
	return 0;
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