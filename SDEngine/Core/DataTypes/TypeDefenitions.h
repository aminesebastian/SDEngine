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

using TString = std::string;

using int8   = char;
using int16  = short;
using int32  = int;
using int64  = __int64;
using uint8  = unsigned char;
using uint16 = unsigned short;
using uint32 = unsigned int;
using uint64 = unsigned __int64;

using Vector2D = glm::vec2;
using Vector3D = glm::vec3;
using Vector4D = glm::vec4;

using Matrix4 = glm::mat4;
using Matrix3 = glm::mat3;

static const Vector2D ZERO_VECTOR2D = Vector2D(0.0f, 0.0f);
static const Vector3D ZERO_VECTOR3D = Vector3D(0.0f, 0.0f, 0.0f);
static const Vector4D ZERO_VECTOR4D = Vector4D(0.0f, 0.0f, 0.0f, 0.0f);

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

struct FColor : Vector4D {

public:

	FColor() {
		r = 0.0f;
		g = 0.0f;
		b = 0.0f;
		a = 1.0f;
	}
	FColor(Vector4D Color) {
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
	FColor(Vector3D Color) {
		r = Color.x;
		g = Color.y;
		b = Color.z;
		a = 1.0f;
	}

	uint32 ToHexadecimal() {
		return ((int)(255 * r) << 24) + (((int)(255 * g)) << 16) + (((int)(255 * b)) << 8) + ((int)(255 * a));
	}

	FColor FColor::operator+ (const FColor& c) const {
		FColor result;
		result.r = r + c.r;
		result.g = g + c.g;
		result.b = b + c.b;
		result.a = a + c.a;
		return result;
	}
	FColor FColor::operator+ (const Vector4D& c) const {
		FColor result;
		result.r = r + c.x;
		result.g = g + c.y;
		result.b = b + c.z;
		result.a = a + c.w;
		return result;
	}
	FColor FColor::operator+ (const Vector3D& c) const {
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
	FColor FColor::operator- (const Vector4D& c) const {
		FColor result;
		result.r = r - c.x;
		result.g = g - c.y;
		result.b = b - c.z;
		result.a = a - c.w;
		return result;
	}
	FColor FColor::operator- (const Vector3D& c) const {
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
	static inline TString TrimLeft(const TString& s, const TString& delimiters = " \f\n\r\t\v") {
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

