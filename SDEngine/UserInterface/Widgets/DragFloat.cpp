#include "DragFloat.h"
#include <iostream>

DragFloat::DragFloat(TString Label) : Label(Label) {

}
DragFloat::~DragFloat() {
	for (FDragFloatEntry* entry : Entries) {
		delete entry;
	}
	Entries.clear();
}
bool DragFloat::Draw() {
	bool valueChanged = false;

	// Begin drawing
	if (!BeginDrawingDragEntries()) {
		return false;
	}

	// Draw entries
	for (int i = 0; i < Entries.size(); i++) {
		valueChanged |= DrawDragEntry(Entries[i], i);
	}

	// Complete drawing
	if (!EndDrawingDragEntries()) {
		return false;
	}

	// Return if there were any values changed
	return valueChanged;
}
FDragFloatEntry* DragFloat::AddEntry(float* Value, TString Format, FColor Color, float Min, float Max, float ValuePerPixel, float Power) {
	FDragFloatEntry* entry = new FDragFloatEntry();
	entry->Value = Value;
	entry->Format = Format;
	entry->Color = Color;
	entry->Min = Min;
	entry->Max = Max;
	entry->ValuePerPixel = ValuePerPixel;
	entry->Power = Power;
	Entries.push_back(entry);
	InternalValues.push_back(float(*Value));
	return entry;
}
bool DragFloat::BeginDrawingDragEntries() {
	ImGuiWindow* window = ImGui::GetCurrentWindow();
	if (window->SkipItems) {
		return false;
	}

	ImGuiContext& g = *GImGui;
	ImGui::BeginGroup();
	ImGui::PushID(Label.c_str());
	ImGui::PushMultiItemsWidths(Entries.size(), ImGui::CalcItemWidth());
	ImGui::TextUnformatted(Label.c_str(), ImGui::FindRenderedTextEnd(Label.c_str()));
	return true;
}
bool DragFloat::DrawDragEntry(FDragFloatEntry* Entry, int Index) {
	bool value_changed = false;

	ImGui::PushID(Index);
	if (Entry->OnFormatForViewFunction) {
		InternalValues[Index] = Entry->OnFormatForViewFunction(*Entry->Value);
	}

	value_changed = ImGui::DragFloat("##v", &InternalValues[Index], Entry->ValuePerPixel, Entry->Min, Entry->Max, Entry->Format.c_str(), Entry->Power);

	if (value_changed) {
		if (Entry->OnFormatForViewFunction) {
			*Entry->Value = Entry->OnFormatFromView(InternalValues[Index]);
		} else {
			*Entry->Value = InternalValues[Index];
		}	
		if (Entry->OnChangeFunction) {
			Entry->OnChangeFunction(Entry);
		}
	}

	const ImVec2 min = ImGui::GetItemRectMin();
	const ImVec2 max = ImGui::GetItemRectMax();
	const float spacing = GImGui->Style.FrameRounding;
	const float halfSpacing = spacing / 2;

	ImGui::GetCurrentWindow()->DrawList->AddLine({ min.x + spacing, max.y - halfSpacing }, { max.x - spacing, max.y - halfSpacing }, Entry->Color.ToImColor(), 4);

	ImGui::SameLine(0, GImGui->Style.ItemInnerSpacing.x);
	ImGui::PopID();
	ImGui::PopItemWidth();

	return value_changed;
}
bool DragFloat::EndDrawingDragEntries() {
	ImGui::PopID();

	ImGui::EndGroup();

	return true;
}
