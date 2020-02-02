#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

class IDetailsPanelProvider {
	virtual TString GetDetailsPanelName() = 0;
	virtual bool PopulateDetailsPanel() = 0;
};

