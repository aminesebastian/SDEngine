#pragma once
#include "TypeDefenitions.h"

using namespace glm;

class SMath {
public:
	SMath();
	~SMath();

	template<typename T>
	static T RandRange(T Min, T Max) {
		return Min + static_cast <T> (rand()) / (static_cast <T> (RAND_MAX / (Max - Min)));
	}
	template<typename T>
	static T Lerp(T Min, T Max, float Alpha) {
		return Min + Alpha * (Max - Min);
	}

	static inline TString LocationToString(vec3 Position) { return "X: " + std::to_string(Position.x) + " Y: " + std::to_string(Position.y) + " Z: " + std::to_string(Position.z); }
	static inline TString RotationToString(vec3 EulerRotation) { return "P: " + std::to_string(degrees(EulerRotation.x)) + " Y: " + std::to_string(degrees(EulerRotation.y)) + " R: " + std::to_string(degrees(EulerRotation.z)); }
	static inline TString ScaleToString(vec3 Scale) { return "X: " + std::to_string(Scale.x) + " Y: " + std::to_string(Scale.y) + " Z: " + std::to_string(Scale.z); }
};

