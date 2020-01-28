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

template <typename  K, typename V>
using SPair = std::pair<K, V>;

template <typename  K, typename  V>
using SPairArray = std::vector<SPair<K, V>>;

template <typename  K, typename V>
using SHashMap = std::unordered_map<K, V>;

template <typename  K, typename V>
using SSortedMap = std::map<K, V>;

template <typename  N>
using SSet = std::vector<N>;

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

