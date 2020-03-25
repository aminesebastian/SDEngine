#pragma once
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/Utilities/EngineFunctionLibrary.h"
#include <sstream>

class StringBuilder {
public:
	StringBuilder() {

	}
	~StringBuilder() {

	}

	StringBuilder& operator<<(const TString& Text) {
		Stream << Text;
		return *this;
	}
	StringBuilder& operator<<(const char& Character) {
		Stream << Character;
		return *this;
	}
	StringBuilder& operator<<(const int32& Value) {
		Stream << std::to_string(Value);
		return *this;
	}
	StringBuilder& operator<<(const float& Value) {
		Stream << std::to_string(Value);
		return *this;
	}
	StringBuilder& operator<<(const bool& Value) {
		Stream << (Value ? "true" : "false");
		return *this;
	}
	const TString ToString() const {
		return Stream.str();
	}
	const bool IsEmpty() const {
		return ToString().length() == 0;
	}
	void Clear() {
		Stream.clear();
		Stream.str("");
	}
private:
	std::stringstream Stream;
};

