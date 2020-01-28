#pragma once
#include "Core/DataStructures/Array.h"
#include <string>
#include "Include/FMT/core.h"

class TString {
public:
	TString();
	TString(std::string String);
	TString(const char* Characters);
	TString(const SArray<char>& Characters);

	TString Substring(int StartIndex, int EndIndex);
	SArray<TString> Split(char Delimiter);
	SArray<TString> Split(const TString& Delimiter);
	TString Trim();
	TString TrimLeft();
	TString TrimRight();
	TString RemoveAtIndex(int Index);

	TString& Append(const TString& Other);
	TString& Append(const std::string& Other);
	TString& Append(const char* Other);

	TString& Append(const TString& Other, int CountToCopy);
	TString& Append(const char* Other, int CountToCopy);

	const char* ToCString() const;
	int FindFirstNotOf(TString& Delimiters) const;
	int Length() const;
	int FindFirstOf(char Character) const;
	int FindLastOf(char Character) const;

	bool IsEmpty() const;

	char& operator[](int Index);
	const char& operator[](int Index) const;

	TString operator+(const TString& Other);
	const TString operator+(const TString& Other) const;
	TString operator+(const char* Other);
	const TString operator+(const char* Other) const;
	TString operator+(std::string Other);
	const TString operator+(std::string Other) const;

	TString& operator+=(char Other);
	TString& operator+=(char* Other);

	bool operator==(TString& Other) const;
	bool operator==(const TString& Other) const;
	bool operator==(std::string Other) const;
	bool operator==(const char* Other) const;

	friend TString TString::operator+(const char* Other, TString& Other2) {
		SArray<char> newData;

		char* curr = (char*)Other;
		while (*curr != '\0') {
			newData.Add(*curr);
			curr++;
		}

		newData.AddAll(Other2.Data);

		return TString(newData);
	}

	char* begin();
	char* end();
	const char* begin() const;
	const char* end() const;

private:
	SArray<char> Data;
	char* CString;

	void UpdateCString();
};

#define TRIM_CHARACTERS TString("\t\n\v\f\r ")

namespace std {
	template<>
	struct hash<TString> {
		size_t operator()(const TString& Other) const {
			size_t h = 5381;
			const char* curr = &Other[0];
			int c;
			while ((c = *curr++)) {
				h = ((h << 5) + h) + c;
			}
			return std::hash<std::string>()(Other.ToCString());
		}
	};
}
namespace fmt {
	template<>
	struct fmt::formatter<TString> {
		template<typename ParseContext>
		constexpr auto parse(ParseContext& ctx) {
			return ctx.begin();
		}

		template<typename FormatContext>
		auto format(TString const& stringIn, FormatContext& ctx) {
			return fmt::format_to(ctx.out(), "{0}", stringIn.ToCString());
		}
	};

}

