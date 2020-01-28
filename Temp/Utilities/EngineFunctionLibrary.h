#pragma once

#include "Utilities/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"

template <typename To, typename From>
To* Cast(From* p) {
	return dynamic_cast<To*>(p);
}
template <typename To, typename From>
bool IsA(From* p) {
	return Cast<To>(p) != nullptr;
}
class EngineFunctionLibrary {
	static void AddStringToCharBuffer(SArray<char>& Buffer, const TString& StringToAdd);

};

