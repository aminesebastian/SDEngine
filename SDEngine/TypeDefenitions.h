#pragma once
#include <string>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <GLM\glm.hpp>
#include <GLM\gtx\transform.hpp>

typedef std::string TString;
typedef int int32;

template <typename  N>
using SArray = std::vector<N>;

template <typename  K, typename V>
using SMapPair = std::pair<K, V>;

template <typename  K, typename V>
using SHashMap = std::unordered_map<K, V>;

template <typename  K, typename V>
using SSortedMap = std::map<K, V>;

template <typename  N>
using SSet = std::vector<N>;

using namespace glm;

struct FColor {
	
public:
	float r;
	float g;
	float b;
	float a;

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
	FColor(vec3 Color) {
		r = Color.x;
		g = Color.y;
		b = Color.z;
		a = 1.0f;
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

class TypeDefenitions {

public:
	TypeDefenitions();
	~TypeDefenitions();
};

