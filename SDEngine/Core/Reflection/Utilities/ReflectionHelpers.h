#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Reflection/Reflection.h"
#include "Core/Utilities/EngineFunctionLibrary.h"

/**
 * A library of useful static functions to use with the engine reflection system.
 *
 * @author	Amine
 * @date	3/11/2020
 */
class ReflectionHelpers {
public:
	/**
	 * Gets all the properties of a class, including child properties.
	 *
	 * @param [in,out]	Properties	[in] An array to be populate with the properties.
	 * @param 		  	Object	  	The EngineObject to retrieve the properties for.
	 *
	 * @returns	The properties of class.
	 */
	static const void GetPropertiesOfClass(SArray<const FProperty*>& Properties, const class EngineObject* Object);

	/**
	 * Gets property handle for the provided property name from an EngineObject.
	 *
	 * @param 	Name  	The name of the property to retrieve a handle for.
	 * @param 	Object	The object to retrieve the property from.
	 *
	 * @returns	Null if it fails, else the property handle.
	 */
	static const FProperty* GetProperty(const TString& Name, const class EngineObject* Object);

	/**
	 * Gets property handle for the provided property name from a struct or unknown type pointer.
	 *
	 * @param 	Name  	The name of the property to retrieve a handle for.
	 * @param 	Struct	The struct pointer to retrieve the property from.
	 * @param 	Type  	The type of the struct pointer.
	 *
	 * @returns	Null if it fails, else the property handle.
	 */
	static const FProperty* GetProperty(const TString& Name, const void* Struct, const TypeDescriptor_Struct* Type);
private:
	static void worker_GetAllMembersOfClass(const TypeDescriptor_Class* CurrentNode, SArray<const FProperty*>& Output);
};

