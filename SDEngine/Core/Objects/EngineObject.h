#pragma once
#include "Core/DataTypes/TypeDefenitions.h"

class EngineObject {
public:
	EngineObject(const TString& Name, const TString& Type);
	virtual ~EngineObject();

	/**
	 * Gets the internal name of this object.
	 */
	const TString& GetObjectName() const;
	/**
	 * Gets the type of this object.
	 */
	const TString& GetObjectType() const;

protected:
	void SetObjectType(const TString& Type);
private:
	TString EngineObjectType;
	TString EngineObjectName;
};

