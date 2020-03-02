#pragma once
#include "Core/Objects/IEngineModule.h"

class EditorModule : public IEngineModule {
public:
	const TString GetModuleName() const override;
	void Startup() override;
	void Teardown() override;
};

