#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"

SD_STRUCT()
struct FReflectionTest {
	SD_STRUCT_BODY();

	SD_PROPERTY();
	float testFloat;
	SD_PROPERTY();
	TString testString;
	SD_PROPERTY();
	int32 testInt;
	SD_PROPERTY();
	Vector4D testVector4D;
	SD_PROPERTY();
	Vector3D testVector3D;
	SD_PROPERTY();
	Vector2D testVector2D;
	SD_PROPERTY();
	bool testBool;
	SD_PROPERTY();
	SArray<bool> boolArray;

};

