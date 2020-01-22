#pragma once
#include "BaseWidget.h"
#include "Utilities/TypeDefenitions.h"
#include <functional>

struct FDragFloatEntry {
	float* Value;
	TString Format;
	FColor Color;
	float Min;
	float Max;
	float ValuePerPixel;
	float Power;
	std::function<void(FDragFloatEntry*)> OnChangeFunction;
	std::function<float(float)> OnFormatForViewFunction;
	std::function<float(float)> OnFormatFromView;
};

class DragFloat : public BaseWidget{
public:
	DragFloat(TString Label);
	~DragFloat();
	virtual bool Draw() override;

	FDragFloatEntry* AddEntry(float* Value, TString Format = " %.3f", FColor Color = vec3(0.0f, 0.0f, 0.0f), float Min = 0.0f, float Max = 0.0f, float ValuePerPixel = 1.0f, float Power = 1.0f);
private:


	bool BeginDrawingDragEntries();
	bool DrawDragEntry(FDragFloatEntry* Entry, int Index);
	bool EndDrawingDragEntries();

	TString Label;
	SArray<FDragFloatEntry*> Entries;
	SArray<float> InternalValues;
};

