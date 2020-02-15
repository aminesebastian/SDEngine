#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

class EngineObject {
public:
	EngineObject(const TString& Name);
	~EngineObject();

	/**
	 * Gets the internal name of this object.
	 */
	const TString& GetName() const;
private:
	const TString Name;
};

