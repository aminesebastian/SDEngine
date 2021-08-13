#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

class IEngineModule{
public:
	virtual const TString GetModuleName() const = 0;
	virtual void Startup() = 0;
	virtual void Teardown() = 0;
};