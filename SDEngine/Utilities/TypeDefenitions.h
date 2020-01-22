#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <set>
#include <regex>
#include <iterator>
#include <stack>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>
#include "UserInterface/UserInterface.h"

typedef std::string TString;

typedef int int32;

typedef char    int8;
typedef short   int16;
typedef int     int32;
typedef __int64	int64;
typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int        uint32;
typedef unsigned __int64	uint64;

template <typename  N>
using SStack = std::stack<N>;

template <typename  N>
using SArray = std::vector<N>;

template <typename  K, typename V>
using SPair = std::pair<K, V>;

template <typename  K, typename  V>
using SPairArray = std::vector<SPair<K, V>>;

template <typename  K, typename V>
using SHashMap = std::unordered_map<K, V>;

template <typename  K, typename V>
using SSortedMap = std::map<K, V>;

template <typename  N>
using SSet = std::unordered_set<N>;

using namespace glm;

struct FColor : vec4 {
	
public:

	FColor() {
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 1.0f;
	}
	FColor(vec4 Color) {
		r = Color.x;
		g = Color.y;
		b = Color.z;
		a = Color.w;
	}
	FColor(float R, float G, float B, float A = 1.0f) {
		r = R;
		g = G;
		b = B;
		a = A;
	}
	FColor(vec3 Color) {
		r = Color.x;
		g = Color.y;
		b = Color.z;
		a = 1.0f;
	}

	uint32 ToHexadecimal() {
		return ((int)(255 * r) << 24) + (((int)(255 * g)) << 16) + (((int)(255 * b)) << 8) + ((int)(255 * a));
	}
	ImColor ToImColor() {
		return ImColor(r, g, b, a);
	}
	

	FColor FColor::operator+ (const FColor& c) const {
		FColor result;
		result.r = r + c.r;
		result.g = g + c.g;
		result.b = b + c.b;
		result.a = a + c.a;
		return result;
	}
	FColor FColor::operator+ (const vec4& c) const {
		FColor result;
		result.r = r + c.x;
		result.g = g + c.y;
		result.b = b + c.z;
		result.a = a + c.w;
		return result;
	}
	FColor FColor::operator+ (const vec3& c) const {
		FColor result;
		result.r = r + c.x;
		result.g = g + c.y;
		result.b = b + c.z;
		return result;
	}
	FColor FColor::operator- (const FColor& c) const {
		FColor result;
		result.r = r - c.r;
		result.g = g - c.g;
		result.b = b - c.b;
		result.a = a - c.a;
		return result;
	}
	FColor FColor::operator- (const vec4& c) const {
		FColor result;
		result.r = r - c.x;
		result.g = g - c.y;
		result.b = b - c.z;
		result.a = a - c.w;
		return result;
	}
	FColor FColor::operator- (const vec3& c) const {
		FColor result;
		result.r = r - c.x;
		result.g = g - c.y;
		result.b = b - c.z;
		return result;
	}
	FColor FColor::operator* (const float& scalar) const {
		FColor result;
		result.r = r * scalar;
		result.g = g * scalar;
		result.b = b * scalar;
		return result;
	}
	FColor FColor::operator/ (const float& scalar) const {
		FColor result;
		result.r = r / scalar;
		result.g = g / scalar;
		result.b = b / scalar;
		return result;
	}
	FColor FColor::operator* (const int32& scalar) const {
		FColor result;
		result.r = r * scalar;
		result.g = g * scalar;
		result.b = b * scalar;
		return result;
	}
	FColor FColor::operator/ (const int32& scalar) const {
		FColor result;
		result.r = r / scalar;
		result.g = g / scalar;
		result.b = b / scalar;
		return result;
	}
};

template<typename T, typename K>
class SFastHashTable {
public:
	SFastHashTable() {
		m_CurrentID = 0;
	}
	~SFastHashTable() {

	}

	void Put(T Key, K Value) {
		m_IDMap.insert(m_CurrentID, Key);
		m_ValueArray.push_back(Value);
	}

	K Get(T Key) {
		int32 id = GetFastID(Key);
		return GetFast(id);
	}
	K GetFast(int32 ID) {
		return m_ValueArray[ID];
	}

	int32 GetFastID(T Key) {
		SHashMap<T, int>::const_iterator it;
		m_IDMap.find(Asset);

		if (it != m_IDMap.end()) {
			return it.first;
		}
		return -1;
	}

	bool Contains(T Key) {
		SHashMap<T, int>::const_iterator it;
		m_IDMap.find(Asset);

		if (it != S_AssetCache.end()) {
			return true;
		}
		return false;
	}
	bool FastContains(int32 ID) {
		return ID < m_ValueArray.size() ? m_ValueArray[ID] != nullptr : false;
	}

	bool Remove(T Key) {
		int32 id = GetFastID(Key);
		RemoveFast(id);
	}
	bool RemoveFast(int32 ID) {
		m_ValueArray[ID] = nullptr;
	}

private:
	int32			 m_CurrentID;
	SHashMap<T, int> m_IDMap;
	SArray<K>		 m_ValueArray;
};

class TStringUtils {
public:
	static inline TString TrimRight(const TString& s, const TString& delimiters = " \f\n\r\t\v") {
		return s.substr(0, s.find_last_not_of(delimiters) + 1);
	}
	static inline TString TrimLeft( const TString& s, const TString& delimiters = " \f\n\r\t\v") {
		return s.substr(s.find_first_not_of(delimiters));
	}
	static inline TString Trim(const TString& s, const TString& delimiters = " \f\n\r\t\v") {
		return TrimLeft(TrimRight(s, delimiters), delimiters);
	}
};

class TypeDefenitions {

public:
	TypeDefenitions();
	~TypeDefenitions();
};

