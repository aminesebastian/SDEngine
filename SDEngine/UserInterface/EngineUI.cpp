#include "Core/Pictorum/PictorumRenderer.h"
#include "Core/Pictorum/PictorumWidget.h"
#include "EngineUI.h"
#include "Core/Engine/Engine.h"
#include "Core/Utilities/Logger.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include "Core/Rendering/PostProcessing/PostProcessingLayer.h"

EngineUI::EngineUI() {
	MaxFrameTimeCache = 50;
	SelectedWidget = nullptr;
	SD_ENGINE_INFO("Engine UI Created");
}
EngineUI::~EngineUI() {

}
bool EngineUI::InitalizeUI(SDL_Window& Window, const SDL_GLContext& Context) {
	SD_ENGINE_INFO("Initializing Engine UI")
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	//io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;  
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;     

	// Setup Dear ImGui style
	ImGui::StyleColorsDark();

	// Setup Platform/Renderer bindings
	ImGui_ImplSDL2_InitForOpenGL(&Window, Context);
	ImGui_ImplOpenGL3_Init("#version 130");


	SD_ENGINE_INFO("Engine UI Initialized")
	return true;
}
void EngineUI::UpdateUI(SDL_Window& Window) {
	//float smoothedFrameTime = GetSmoothedFrameTime();
	//Engine* engine = Engine::GetInstance();
	//RenderViewport* renderingEngine = engine->GetFocusedViewport();

	//ImGui_ImplOpenGL3_NewFrame();
	//ImGui_ImplSDL2_NewFrame(Window);
	//ImGui::NewFrame();

	//ImGui::Begin("Stat Unit");                          

	//ImGui::Text("Render Stats");
	//ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", smoothedFrameTime * 1000, 1.0f / smoothedFrameTime);

	//if (ImGui::CollapsingHeader("Post Processing")) {
	//	for (PostProcessingLayer* layer : renderingEngine->GetPostProcessingLayers()) {
	//		if (ImGui::TreeNode(layer->GetDetailsPanelName().c_str())) {
	//			layer->PopulateDetailsPanel();
	//			ImGui::TreePop();
	//			ImGui::Separator();
	//		}
	//	}	
	//}

	//ImGui::End();

	//ImGui::Begin("Details Panel");
	//ImGui::Text("World");
	//ImGui::PushItemWidth(-1);
	//if (ImGui::ListBoxHeader("##empty")) {
	//	for (Entity* entity : engine->GetWorld()->GetWorldActors()) {
	//		TString& item_name = (TString)entity->GetName();
	//		if (ImGui::Selectable(item_name.c_str(), false)) {
	//			engine->SetSelectedEntity(entity);
	//		}
	//	}
	//	ImGui::ListBoxFooter();
	//}

	//ImGui::Separator();

	//ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
	//if (ImGui::BeginTabBar("##empty", tab_bar_flags)) {
	//	if (ImGui::BeginTabItem("Details")) {
	//		if (engine->GetSelectedEntity()) {
	//			if (IsA<Actor>(engine->GetSelectedEntity())) {
	//				ImGui::Text(engine->GetSelectedEntity()->GetName().c_str());
	//				Cast<Actor>(engine->GetSelectedEntity())->PopulateDetailsPanel();
	//			}
	//		} else {
	//			ImGui::Text("Select an Actor to Modify");
	//		}
	//		ImGui::EndTabItem();
	//	}
	//	if (ImGui::BeginTabItem("World Settings")) {
	//		ImGui::EndTabItem();
	//	}
	//	ImGui::EndTabBar();
	//}

	//ImGui::End();


	//// UI
	//ImGui::Begin("User Interface Panel");
	//ImGui::Text("UI Stack");
	//ImGui::PushItemWidth(-1);
	//if (ImGui::ListBoxHeader("##empty")) {
	//	for (PictorumWidget* rootWidget : Engine::GetInstance()->GetEngineUI()->GetWidgets()) {
	//		TString& item_name = (TString)rootWidget->GetName();
	//		if (ImGui::Selectable(item_name.c_str(), false)) {
	//			SelectedWidget = rootWidget;
	//		}
	//	}
	//	ImGui::ListBoxFooter();
	//}

	//ImGui::Separator();

	//if (ImGui::BeginTabBar("##empty", tab_bar_flags)) {
	//	if (ImGui::BeginTabItem("Details")) {
	//		if (SelectedWidget) {
	//			ImGui::Text(SelectedWidget->GetDetailsPanelName().c_str());
	//			
	//			ImGui::PushID("Root");
	//			SelectedWidget->PopulateDetailsPanel();
	//			ImGui::PopID();

	//			SArray<PictorumWidget*> children;
	//			SelectedWidget->GetAllChildren(children);
	//			int index = 0;
	//			for (PictorumWidget* childWidget : children) {
	//				ImGui::PushID("Child" + index);
	//				ImGui::Text(childWidget->GetDetailsPanelName().c_str());
	//				childWidget->PopulateDetailsPanel();
	//				ImGui::PopID();
	//				index++;
	//			}
	//		} else {
	//			ImGui::Text("Select a Widget to Modify");
	//		}
	//		ImGui::EndTabItem();
	//	}
	//	ImGui::EndTabBar();
	//}
	//ImGui::End();
}
void EngineUI::RenderUI(const float& DeltaTime) {
	FrameTimeCache.Add(DeltaTime);
	if (FrameTimeCache.Count() > MaxFrameTimeCache) {
		FrameTimeCache.RemoveAt(0);
	}

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	//ImGuiIO& io = ImGui::GetIO(); 
	//if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
	//	SDL_Window* backup_current_window = SDL_GL_GetCurrentWindow();
	//	SDL_GLContext backup_current_context = SDL_GL_GetCurrentContext();
	//	ImGui::UpdatePlatformWindows();
	//	ImGui::RenderPlatformWindowsDefault();
	//	SDL_GL_MakeCurrent(backup_current_window, backup_current_context);
	//}
}
bool EngineUI::DestroyUI() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();
	return true;
}

float EngineUI::GetSmoothedFrameTime() {
	float total = 0.0f;
	for (float val : FrameTimeCache) {
		total += val;
	}
	return total / FrameTimeCache.Count();
}
