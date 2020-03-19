#pragma once
#include <vector>
#include <iterator>

using namespace std;

typedef int int32;

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
	void Add(const Type& Data) {
		push_back(Data);
	}	
	template <typename Type, size_t size>
	void AddAll(const Type (&Data)[size]) {
		if (size == 0) {
			return;
		}
		int32 initialCount = Count();
		Resize(initialCount + size);
		memcpy(&(*this)[0] + initialCount, &Data[0], sizeof(Type) * size);
	}

	template <typename Type>
	void AddAll(const SArray<Type>& Other) {
		if (Other.Count() == 0) {
			return;
		}
		for (const Type& oth : Other) {
			Add(oth);
		}
	}
	template <typename Type>
	void AddAll(const Type* Other, int32 Count) {
		if (Count == 0) {
			return;
		}
		int32 initialCount = Count();
		Resize(initialCount + Count);
		memcpy(&(*this)[0] + initialCount, &Other[0], sizeof(Type) * Count);
	}
	template <typename Type>
	bool AddUnique(Type Data) {
		if (Contains(Data)) {
			return false;
		}
		Add(Data);
		return true;
	}
	template <typename Type>
	void AddAfter(int32 Index, Type Value) {
		insert(begin() + Index, Value);
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
	int Find(Type Data) const {
		for (int i = 0; i < Count(); i++) {
			if ((*this)[i] == Data) {
				return i;
			}
		}
		return -1;
	}
	template <typename Type>
	bool Contains(Type Data) {
		return Find(Data) >= 0;
	}
	const int32 Count() const {
		return (int32)size();
	}
	const int32 LastIndex() const {
		return Count() - 1;
	}
	bool IsEmpty() const{
		return empty();
	}
	void Clear() {
		clear();
	}
	void ShrinkToFit() {
		shrink_to_fit();
	}
	void PreAllocate(int32 Size) {
		reserve(Size);
	}
	void Resize(int32 Size) {
		resize(Size);
	}
	template <typename Type>
	void Resize(int32 Size, Type Content = 0) {
		resize(Size, Content);
	}
	template <typename Type>
	void Fill(Type Value, int32 Start = 0) {
		Fill(Value, Start, Count());
	}
	template <typename Type>
	void Fill(Type Value, int32 Start, int32 End) {
		int requiredSize = End - Start;
		if ((int32)capacity() < requiredSize) {
			PreAllocate(requiredSize - (int32)capacity());
		}
		for (int i = Start; i < End; i++) {
			Add(Value);
		}
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

