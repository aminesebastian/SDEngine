#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Engine/Delegates/Event.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Utilities/Logger.h"

SD_CLASS()
class EngineObject {
	// These have to be added manually as they cannot be added by the macro.
	friend struct DefaultResolver;
	static TypeDescriptor_Class Reflection;
	static void InitializeReflection(TypeDescriptor_Class*);

public:
	EngineObject(const TString& Name);
	virtual ~EngineObject();

	/**
	 * Gets the internal name of this object.
	 */
	const TString& GetObjectName() const;
	/**
	 * This method is overwritten through the reflection system.
	 */
	virtual const TypeDescriptor_Class* StaticDescriptor() const;
	/**
	 * Executes the property updated action when a property has been updated in the editor.
	 *
	 * @param [in,out]	Property	If non-null, the property.
	 */
	void OnPropertyUpdated(FProperty* Property);

	void Destroy();
	virtual void OnDestroyed();

	void operator delete(void* p) {
		SD_ENGINE_WARN("An illegal call has been made to the delete operator for an EngineObject. To destroy an EngineObject, call the object's Destory() method.");
	}

	Event<void(EngineObject*)> OnDestroyedDelegate;
protected:
	SD_PROPERTY(InspectorHidden)
		const TString EngineObjectName;
};

