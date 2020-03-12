#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Reflection/Reflection.h"

class ReflectionHandle;

/**
 * A handle to a property of a reflected object. Useful for getting and setting the value of a
 * property.
 *
 * @author	Amine
 * @date	3/11/2020
 */
class PropertyHandle {
public:
	/** Default constructor */
	PropertyHandle();
	/** Destructor */
	virtual ~PropertyHandle();

	/**
	 * Constructor
	 *
	 * @param 	PropertyIn  	The property this handle encapsulates.
	 * @param 	TargetHandle	The reflected object's handle this property belongs to.
	 */
	PropertyHandle(const FProperty* PropertyIn, const ReflectionHandle& TargetHandle);

	/**
	 * Gets the property name as it appears in the source code.
	 *
	 * @returns	The property name.
	 */
	const TString GetPropertyName() const;

	/**
	 * Gets the pretty property name for display in the UI.
	 *
	 * @returns	The inspector name.
	 */
	const TString GetInspectorName() const;

	/**
	 * Gets the metadata of the property this handle encapsulates.
	 *
	 * @returns	The metadata.
	 */
	const FPropertyMetadata& GetMetadata() const;

	/**
	* Determines if this handle is valid.
	*
	* @returns	True if valid, false otherwise.
	*/
	const bool& IsValid() const;

	/**
	 * Gets the type of the property this handle encapsulates.
	 *
	 * @returns	Null if it fails, else the type.
	 */
	const ITypeDescriptor* GetType() const;

	/**
	 * Gets a pointer to the raw property. This should RARELY be accessed.
	 *
	 * @returns	Null if it fails, else the raw property.
	 */
	const FProperty* GetRawProperty() const;
	template<typename T>
	const T* GetValue() const {
		return Property->GetValue<T>(Target);
	}
	template<typename T>
	void SetValue(T* Value) {
		Property->SetValue<T>(Target, Value);
	}
	template<typename T>
	void SetValue(const T& Value) {
		Property->SetValue<T>(Target, Value);
	}
	bool operator== (const PropertyHandle& Other) const {
		return Property == Other.Property;
	}
	bool operator!= (const PropertyHandle& Other) const {
		return Property == Other.Property;
	}
	bool operator== (PropertyHandle& Other) const {
		return Property == Other.Property;
	}
	bool operator!= (PropertyHandle& Other) const {
		return Property == Other.Property;
	}
private:
	/** The property this represents.*/
	const FProperty* Property;
	const void* Target;
	bool bIsValid;
};