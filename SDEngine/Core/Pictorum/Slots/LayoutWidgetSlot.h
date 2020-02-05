#pragma once
#include "Core/Pictorum/IWidgetSlot.h"
#include "Core/Pictorum/PictorumDataTypes.h"

class LayoutWidgetSlot : public IWidgetSlot {
public:
	LayoutWidgetSlot();

	void SetTopOffset(const float& TopOffset);
	void SetRightOffset(const float& RightOffset);
	void SetBottomOffset(const float& BottomOffset);
	void SetLeftOffset(const float& LeftOffset);
	void SetOffsets(const float& Top, const float& Right, const float& Bottom, const float& Left);

	const FAnchors& GetOffsets();
	void SetOffsetRelative(bool Relative);
	const bool& GetOffsetsRelative() const;

	void SetTopMargin(const float& TopMargin);
	void SetRightMargin(const float& RightMargin);
	void SetBottomMargin(const float& BottomMargin);
	void SetLeftMargin(const float& LeftMargin);
	void SetMargins(const float& Top, const float& Right, const float& Bottom, const float& Left);

	const FMargins& GetMargins();
private:
	FAnchors Offsets;
	FMargins Margins;
	bool bIsRelative;
};

