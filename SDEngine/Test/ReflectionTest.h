#pragma once
#include "Core/Reflection/Reflection.h"
#include "Core/DataTypes/TypeDefenitions.h"
#include "Core/DataStructures/DataStructures.h"
#include "Core/Objects/EngineObject.h"

SD_CLASS()
class FReflectionTest : public EngineObject {
	SD_CLASS_BODY();

	SD_PROPERTY();
	float testFloat;
	SD_PROPERTY();
	TString testString;
	SD_PROPERTY();
	int32 testInt;
	SD_PROPERTY();
	Vector4D testVector4d;
	SD_PROPERTY();
	Vector3D testVector3d;
	SD_PROPERTY();
	Vector2D testVector2d;
	SD_PROPERTY();
	bool testBool;
	SD_PROPERTY();
	SArray<bool> boolArray;

	SD_FUNCTION()
		const		int32& THISisATesat(const  bool& test, bool test2, int32* test3) const {}
};

