#pragma once
#include "Core/Pictorum/IWidgetSlot.h"

class PictorumGridSlot : public IWidgetSlot {
public:
	PictorumGridSlot();
	virtual ~PictorumGridSlot();

	int32 Column;
	int32 Row;
	int32 ColumnSpan;
	int32 RowSpan;
};

