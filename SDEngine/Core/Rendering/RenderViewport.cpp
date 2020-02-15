#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Window.h"
#include "Core/Engine/World.h"
#include "Core/Objects/CoreTypes/RenderTarget.h"
#include "Core/Objects/CoreTypes/Shader.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/Entities/Light.h"
#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Rendering/GBuffer.h"
#include "Core/Rendering/PostProcessing/BloomPostProcessing.h"
#include "Core/Rendering/PostProcessing/MotionBlurPostProcessing.h"
#include "Core/Rendering/PostProcessing/PostProcessingLayer.h"
#include "Core/Rendering/PostProcessing/SSAOPostProcessing.h"
#include "Core/Rendering/PostProcessing/ToneMapper.h"
#include "Core/Rendering/RenderTypeDefenitions.h"
#include "Core/Rendering/RenderViewport.h"
#include "Core/Rendering/Utilities/VariableGausianBlur.h"
#include "Core/Utilities/Logger.h"
#include <GLEW/glew.h>

RenderViewport::RenderViewport(const World* RenderWorld, const Window* OwningWindow) : RenderWorld(RenderWorld), OwningWindow(OwningWindow) {
	CurrentBuffer                  = 0;
	bDebugMode                     = false;
	bInitialized                   = false;
	Compositor                     = new DefferedCompositor(this);
	SelectionOutlineShader         = new Shader("./Res/Shaders/SelectionOutlineShader", false);
	MainGBuffer                    = nullptr;
	OutputBuffer1                  = nullptr;
	OutputBuffer2                  = nullptr;
	CurrentlyActiveShader          = nullptr;
	CurrentRenderStage             = GEOMETRY;

	LastFrameRenderTime = 0;
	RenderDeltaTime     = 0.0f;
	RenderFrameRate     = 0.0f;

	SD_ENGINE_INFO("Render viewport created for window: {0}.", OwningWindow->GetTitle());
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
	OwningWindow->OnWindowResized.Add<RenderViewport, &RenderViewport::OnWindowResized>(this);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glEnable(GL_CULL_FACE);
	bInitialized = true;
	SD_ENGINE_INFO("Viewport Initialized")
}
void RenderViewport::GenerateRenderTargets() {
	if (!bInitialized) {
		SD_ENGINE_INFO("Generating Viewport Render Targets")
	} else {
		SD_ENGINE_INFO("Regenerating Viewport Render Targets")
	}

	if (MainGBuffer != nullptr) {
		delete MainGBuffer;
	}
	if (OutputBuffer1 != nullptr) {
		delete OutputBuffer1;
	}
	if (OutputBuffer2 != nullptr) {
		delete OutputBuffer2;
	}

	MainGBuffer = new GBuffer(OwningWindow->GetDimensions());
	MainGBuffer->FinalizeRenderTarget();

	OutputBuffer1 = new RenderTarget(OwningWindow->GetDimensions());
	OutputBuffer1->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	OutputBuffer1->AddDepthStencilBuffer();
	OutputBuffer1->FinalizeRenderTarget();

	OutputBuffer2 = new RenderTarget(OwningWindow->GetDimensions());
	OutputBuffer2->AddTextureIndex(new FRenderTargetTextureEntry("Output", GL_RGBA32F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
	OutputBuffer2->AddDepthStencilBuffer();
	OutputBuffer2->FinalizeRenderTarget();
}
void RenderViewport::RegisterPostProcessEffects() {
	SD_ENGINE_INFO("Creating Post Process Stack")

	S_PostProcessingLayers.Clear();
	S_PostProcessingLayers.Add(new SSAOPostProcessing(this));
	S_PostProcessingLayers.Add(new BloomPostProcessing(this));
	//S_PostProcessingLayers.Add(new MotionBlurPostProcessing(this));
	S_PostProcessingLayers.Add(new ToneMapper(this));
}

void RenderViewport::RecompileShaders() {
	Compositor->RecompileShaders();

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

void RenderViewport::Render(const Camera* RenderCamera) {
	CurrentBuffer = 0;

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		actor->PreFrameRendered();
	}

	CurrentRenderStage = ERenderingStage::GEOMETRY;
	GemoetryPass(RenderCamera);

	CurrentRenderStage = ERenderingStage::LIGHTING;
	Compositor->CompositeLighting(MainGBuffer, GetCurrentOutputBuffer(), RenderWorld->GetWorldLights(), RenderCamera);

	CurrentRenderStage = ERenderingStage::POST_PROCESSING;
	RenderPostProcessing(RenderCamera);
	
	CurrentRenderStage = EDITOR_ELEMENTS;
	RenderEditorElements(RenderCamera);

	CurrentRenderStage = ERenderingStage::OUTPUT;
	Compositor->OutputToScreen(GetCurrentOutputBuffer());

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		actor->PostFrameRendered();
	}
}
void RenderViewport::GemoetryPass(const Camera* RenderCamera) {
	MainGBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

	for (Actor* actor : RenderWorld->GetWorldActors()) {
		if (actor->ShouldBeDrawn(EDrawType::SCENE_RENDER)) {
			if (RenderWorld->GetSelectedEntity() == actor) {
				glEnable(GL_STENCIL_TEST);
				glStencilFunc(GL_ALWAYS, 1, -1);
				glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
			}
			actor->Draw(RenderCamera);
			if (RenderWorld->GetSelectedEntity() == actor) {
				glDisable(GL_STENCIL_TEST);
			}
		}
	}
}
void RenderViewport::RenderPostProcessing(const Camera* RenderCamera) {
	for (PostProcessingLayer* layerPair : S_PostProcessingLayers) {
		// Skip any layers that are disabled.
		if (!layerPair->IsEnabled()) {
			continue;
		}

		FlipOutputBuffers();
		layerPair->RenderLayer(Compositor, RenderCamera, MainGBuffer, GetPreviousOutputBuffer(), GetCurrentOutputBuffer());
	}
}
void RenderViewport::RenderEditorElements(const Camera* RenderCamera) {
	// Draw selection outline if exists.
	Entity* selectedEntity = RenderWorld->GetSelectedEntity();
	if (selectedEntity) {
		vec2 resolution = GetCurrentOutputBuffer()->GetRenderTargetResolution();

		MainGBuffer->BindForReading();
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
	}
}
RenderTarget* RenderViewport::GetCurrentOutputBuffer() {
	return CurrentBuffer == 0 ? OutputBuffer1 : OutputBuffer2;
}
RenderTarget* RenderViewport::GetPreviousOutputBuffer() {
	return CurrentBuffer == 0 ? OutputBuffer2 : OutputBuffer1;
}
void RenderViewport::OnWindowResized(const int32& WindowId, const FDisplayState& State) {
	SD_ENGINE_INFO("Changing render target dimensions for viewport from {0} x {1}", (int)State.GetResolution().x, (int)State.GetResolution().y);
	GenerateRenderTargets();

	for (PostProcessingLayer* layer : S_PostProcessingLayers) {
		layer->OnScreenResolutionChanged();
	}
}

const World* RenderViewport::GetWorld() const {
	return RenderWorld;
}
const Window* RenderViewport::GetOwningWindow() const {
	return OwningWindow;
}
const float& RenderViewport::GetRenderFrameTime() const {
	return RenderDeltaTime;
}
const float& RenderViewport::GetRenderFrameRate() const {
	return RenderFrameRate;
}