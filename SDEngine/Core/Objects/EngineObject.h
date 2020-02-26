#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Reflection.h"

SD_CLASS()
class EngineObject {
	// These have to be added manually as they cannot be added by the macro.
	friend struct DefaultResolver;
	static TypeDescriptor_Class Reflection;
	static void InitializeReflection(TypeDescriptor_Class*);

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
	/**
	 * This method is overwritten through the reflection system.
	 */
	virtual const TypeDescriptor_Class* StaticDescriptor() const;
protected:
	SD_PROPERTY();
	const TString EngineObjectType;
	SD_PROPERTY();
	const TString EngineObjectName;
};

