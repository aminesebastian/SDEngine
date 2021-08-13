#pragma once
#include "Core/Objects/EngineObject.h"
#include "Core/Reflection/Handles/PropertyHandle.h"
#include "Core/Reflection/Reflection.h"

struct FProperty;

/**
 * A reference handle iterator.
 *
 * @author	Amine
 * @date	3/12/2020
 */
class ReferenceHandleIterator {
public:
	ReferenceHandleIterator(const ReflectionHandle* Handle, int32 StartIndex = 0);

	bool operator!= (const ReferenceHandleIterator& Other) const;
	const PropertyHandle operator* () const;
	const ReferenceHandleIterator& operator++ ();

private:
	int32 Index;
	const ReflectionHandle* Handle;
	SArray<const FProperty*> Properties;
};


/**
 * A handle to a reflectable object.
 *
 * @author	Amine
 * @date	3/11/2020
 */
class ReflectionHandle {
public:
	/** Default constructor */
	ReflectionHandle();

	/**
	 * Constructor
	 *
	 * @param 	Object	The engine object this ReflectionHandle will alias.
	 */
	ReflectionHandle(const EngineObject* Object);

	/**
	 * Constructor
	 *
	 * @param 	Struct	  	The struct this ReflectionHandle will alias.
	 * @param 	StructType	Type of the structure.
	 */
	ReflectionHandle(const void* Struct, const ITypeDescriptor* StructType);

	/**
	 * Gets a handle to a property of the underlying target.
	 *
	 * @param 	PropertyName	Name of the property.
	 *
	 * @returns	The property handle.
	 */
	const PropertyHandle GetPropertyHandle(const TString& PropertyName) const;

	/**
	 * Checks if this ReflectionHAndle reflects an EngineObject or a struct.
	 *
	 * @returns	True if the reflection target is an instance of EngineObject, otherwise false.
	 */
	const bool& IsObject() const;

	/**
	 * Gets the type of the underlying target.
	 *
	 * @returns	Null if it fails, else the type.
	 */
	const ITypeDescriptor* GetType() const;

	/**
	 * Determines if this handle is valid.
	 *
	 * @returns	True if valid, false otherwise.
	 */
	const bool& IsValid() const;

	/**
	 * Gets an array of all the reflected properties exposed by this object.
	 *
	 * @param 	Properties	The properties out.
	 *
	 * @returns	The Properties.
	 */
	void GetProperties(SArray<const FProperty*>& Properties) const;

	/**
	 * Gets the underlying reflection target.
	 *
	 * @tparam	T	The expected type of the underlying target.
	 *
	 * @returns	A const pointer to the target cast as the provided template parameter.
	 */
	template<typename T>
	const T* Get() const {
		return (T*)Target;
	}

	ReferenceHandleIterator begin() const {
		return ReferenceHandleIterator(this);
	}

	ReferenceHandleIterator end() const {
		return ReferenceHandleIterator(this, -1);
	}
private:
	bool bIsValid;
	bool bIsObject;
	const void* Target;
	const ITypeDescriptor* TargetType;
};


