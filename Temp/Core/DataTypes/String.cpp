#include "String.h"
#include "Utilities/Logger.h"

TString::TString() {
	CString = nullptr;
	UpdateCString();
}
TString::TString(std::string String) : TString() {
	for (char charToAdd : String) {
		Data.Add(charToAdd);
	}
	UpdateCString();
}
TString::TString(const char* Characters) : TString() {
	char* curr = (char*)Characters;
	while (*curr != '\0') {
		Data.Add(*curr);
		curr++;
	}
	UpdateCString();
}
TString::TString(const SArray<char>& Characters) : TString() {
	for (char charToAdd : Characters) {
		Data.Add(charToAdd);
	}
	UpdateCString();
}

TString TString::Substring(int StartIndex, int EndIndex) {
	if (StartIndex < 0) {
		SD_ENGINE_ERROR("Start Index cannot be less than 0.")
	}
	if (StartIndex > Data.LastIndex()) {
		SD_ENGINE_ERROR("Start Index is out of bounds.")
	}
	if (EndIndex < 0) {
		SD_ENGINE_ERROR("End Index cannot be less than 0.")
	}
	if (EndIndex > Data.LastIndex()) {
		SD_ENGINE_ERROR("End Index is out of bounds.")
	}
	if (StartIndex >= EndIndex) {
		SD_ENGINE_ERROR("Start Index must be less than the End Index.")
	}

	SArray<char> newData;

	for (int i = StartIndex; i < EndIndex; i++) {
		newData.Add(Data[i]);
	}
	return TString(newData);
}
SArray<TString> TString::Split(char Delimiter) {
	SArray<TString> output;

	TString curr = "";
	for (char character : Data) {
		if (character != Delimiter) {
			curr += character;
		} else {
			output.Add(curr);
			curr = "";
		}
	}
	return output;
}
SArray<TString> TString::Split(const TString& Delimiter) {
	SArray<TString> output;
	TString temp(*this);
	char* token = strtok(&temp[0], Delimiter.ToCString());

	while (token != NULL) {
		output.Add(token);
		token = strtok(NULL, Delimiter.ToCString());
	}
	return output;
}
TString TString::Trim() {
	TrimLeft();
	TrimRight();
	return *this;
}
TString TString::TrimLeft() {
	for (int i = 0; i < Length(); i++) {
		if (TRIM_CHARACTERS.FindFirstOf(Data[i]) != -1) {
			RemoveAtIndex(i);
		}
	}
	return *this;
}
TString TString::TrimRight() {
	for (int i = Length() - 1; i >= 0; i--) {
		if (TRIM_CHARACTERS.FindFirstOf(Data[i]) != -1) {
			RemoveAtIndex(i);
		}
	}
	return *this;
}
TString TString::RemoveAtIndex(int Index) {
	if (Index < Data.Count() - 1) {
		Data.RemoveAt(Index);
	}
	return *this;
}

TString& TString::Append(const TString& Other) {
	for (char charToAdd : Other) {
		Data.Add(charToAdd);
	}
	UpdateCString();
	return *this;
}
TString& TString::Append(const std::string& Other) {
	for (char charToAdd : Other) {
		Data.Add(charToAdd);
	}
	UpdateCString();
	return *this;
}
TString& TString::Append(const char* Other) {
	char* curr = (char*)Other;
	while (*curr != '\0') {
		Data.Add(*curr);
		curr++;
	}
	UpdateCString();
	return *this;
}

TString& TString::Append(const TString& Other, int CountToCopy) {
	if (CountToCopy <= 0) {
		SD_ENGINE_ERROR("Amount to copy must be greater than 0.")
			return *this;
	}
	if (CountToCopy > Other.Length()) {
		SD_ENGINE_ERROR("Amount to copy greater than provided input length.")
			return *this;
	}
	for (int i = 0; i < CountToCopy; i++) {
		Data.Add(Other[i]);
	}
	UpdateCString();
	return *this;
}
TString& TString::Append(const char* Other, int CountToCopy) {
	if (CountToCopy <= 0) {
		SD_ENGINE_ERROR("Amount to copy must be greater than 0.")
			return *this;
	}
	for (int i = 0; i < CountToCopy; i++) {
		Data.Add(Other[i]);
	}
	UpdateCString();
	return *this;
}

int TString::FindFirstNotOf(TString& Delimiters) const {
	for (int i = 0; i < Length(); i++) {
		if (Delimiters.FindFirstOf(Data[i]) == -1) {
			return i;
		}
	}
	return -1;
}
int TString::Length() const {
	return Data.Count();
}
const char* TString::ToCString() const {
	return CString;
}
int TString::FindFirstOf(char Character) const {
	for (int i = 0; i < Length(); i++) {
		if (Data[i] == Character) {
			return i;
		}
	}
	return -1;
}
int TString::FindLastOf(char Character) const {
	int lastIndex = -1;
	for (int i = 0; i < Length(); i++) {
		if (Data[i] == Character) {
			lastIndex = i;
		}
	}
	return lastIndex;
}

bool TString::IsEmpty() const {
	return Length() == 0;
}

char& TString::operator[](int Index) {
	return Data[Index];
}
const char& TString::operator[](int Index) const {
	return Data[Index];
}
TString TString::operator+(const TString& Other) {
	SArray<char> newData;
	newData.AddAll(Data);
	newData.AddAll(Other.Data);
	return TString(newData);
}
const TString TString::operator+(const TString& Other) const {
	SArray<char> newData;
	newData.AddAll(Data);
	newData.AddAll(Other.Data);
	return TString(newData);
}
TString TString::operator+(const char* Other) {
	SArray<char> newData;
	newData.AddAll(Data);

	char* curr = (char*)Other;
	while (*curr != '\0') {
		newData.Add(*curr);
		curr++;
	}
	return TString(newData);
}
const TString TString::operator+(const char* Other) const {
	SArray<char> newData;
	newData.AddAll(Data);

	char* curr = (char*)Other;
	while (*curr != '\0') {
		newData.Add(*curr);
		curr++;
	}
	return TString(newData);
}
TString TString::operator+(std::string Other) {
	SArray<char> newData;
	newData.AddAll(Data);

	for (char charToAdd : Other) {
		newData.Add(charToAdd);
	}

	return TString(newData);
}
const TString TString::operator+(std::string Other) const {
	SArray<char> newData;
	newData.AddAll(Data);

	for (char charToAdd : Other) {
		newData.Add(charToAdd);
	}

	return TString(newData);
}
TString& TString::operator+=(char Other) {
	Data.Add(Other);
	UpdateCString();
	return *this;
}
TString& TString::operator+=(char* Other) {
	Data.Add(*Other);
	UpdateCString();
	return *this;
}

bool TString::operator==(TString& Other) const {
	if (Length() != Other.Length()) {
		return false;
	}
	for (int i = 0; i < Length(); i++) {
		if (Data[i] != Other[i]) {
			return false;
		}
	}
	return true;
}
bool TString::operator==(const TString& Other) const {
	if (Length() != Other.Length()) {
		return false;
	}
	for (int i = 0; i < Length(); i++) {
		if (Data[i] != Other[i]) {
			return false;
		}
	}
	return true;
}
bool TString::operator==(std::string Other) const {
	if (Length() != Other.size()) {
		return false;
	}
	for (int i = 0; i < Length(); i++) {
		if (Data[i] != Other[i]) {
			return false;
		}
	}
	return true;
}
bool TString::operator==(const char* Other) const {
	const char* curr = Other;
	int currentIndex = 0;

	while (*curr != '\0') {
		if (currentIndex > Data.LastIndex()) {
			return false;
		}
		if ((char)(*curr) != Data[currentIndex]) {
			return false;
		}
		curr++;
		currentIndex++;
	}
	return true;
}

char* TString::begin() {
	return &CString[0];
}
char* TString::end() {
	return &CString[Data.Count() - 1];
}
const char* TString::begin() const {
	return &CString[0];
}
const char* TString::end() const {
	return &CString[Data.Count() - 1];
}
void TString::UpdateCString() {
	if (CString) {
		delete CString;
	}
	if (Length() > 0) {
		CString = (char*)malloc(sizeof(char) * (Length() + 1));
		memcpy(CString, &Data[0], Length());
		CString[Length()] = 0;
	} else {;
		CString = '\0';
	}
}
