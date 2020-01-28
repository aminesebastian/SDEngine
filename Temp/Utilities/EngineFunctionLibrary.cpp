#include "EngineFunctionLibrary.h"

void EngineFunctionLibrary::AddStringToCharBuffer(SArray<char>& Buffer, const TString& StringToAdd) {
	for (char charToAdd : StringToAdd) {
		Buffer.Add(charToAdd);
	}
}