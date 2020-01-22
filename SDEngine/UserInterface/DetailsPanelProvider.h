#pragma once
#include "Utilities/TypeDefenitions.h"

class IDetailsPanelProvider {
	virtual TString GetDetailsPanelName() = 0;
	virtual bool PopulateDetailsPanel() = 0;
};
