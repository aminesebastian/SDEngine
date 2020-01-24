#pragma once
#include <vector>
#include <iterator>
#include "Utilities/TypeDefenitions.h"

using namespace std;

template<class Type>
class SArray : public vector<Type> {
public:
	SArray() {

	}
	SArray(int32 PreReserveSize) {
		PreAllocate(PreReserveSize);
	}
	~SArray() {

	}
	template <typename Type>
	void Add(Type Data) {
		push_back(Data);
	}
	template <typename Type>
	void AddAll(const SArray<Type>& Other) {
		for (Type oth : Other) {
			Add(oth);
		}
	}
	template <typename Type>
	void AddUnique(Type Data) {
		if (Contains(Data)) {
			return;
		}
		Add(Data);
	}
	template <class... _Valty>
	Type Emplace(_Valty&& ... _Val) {
		return vector::emplace_back(std::forward<_Valty>(_Val)...);
	}
	bool Remove(Type Data) {
		return RemoveAt(Find(Data));
	}
	bool RemoveAt(int32 Index) {
		if (Index < 0 || Index > size() - 1) {
			return false;
		}
		erase(begin() + Index);
		return true;
	}
	template <typename Type>
	int Find(Type Data) {
		for (int i = 0; i < Count(); i++) {
			if ((*this)[i] == Data) {
				return i;
			}
		}
		return -1;
	}
	template <typename Type>
	bool Contains(Type Data) {
		return Find(Type) >= 0;
	}
	int32 Count() {
		return (int32)size();
	}
	bool IsEmpty() {
		return empty();
	}
	void Clear() {
		clear();
	}
	void PreAllocate(int32 Size) {
		reserve(Size);
	}


private:
	using vector::push_back;
	using vector::emplace_back;
	using vector::size;
	using vector::erase;
	using vector::insert;
	using vector::clear;
	using vector::capacity;
	using vector::reserve;
	using vector::resize;
};

