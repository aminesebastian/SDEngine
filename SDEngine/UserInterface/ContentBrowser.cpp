#include "ContentBrowser.h"

ContentBrowser::ContentBrowser() {

}
ContentBrowser::~ContentBrowser() {

}

bool ContentBrowser::Draw() {
	//ImGuiDockNodeFlags dockspaceFlags = ImGuiDockNodeFlags_None;
	//ImGuiID bottomId = ImGui::GetID("Bottom");
	//ImGui::DockSpace(bottomId, ImVec2(0.0f, 0.0f), dockspaceFlags);

	ImGui::Begin("Content Browser");



	ImGui::End();
	return true;
}