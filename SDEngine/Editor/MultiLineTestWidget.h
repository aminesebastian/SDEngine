#pragma once
#include "Core/Pictorum/Pictorum.h"

class MultiLineTestWidget  : public PictorumWidget {
public:
	MultiLineTestWidget(const TString& Name);
	virtual void OnCreated() override;
};

