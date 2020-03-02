#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Test\ReflectionTest.h"


//Reflection for struct FReflectionTest
REFLECT_STRUCT_BEGIN(FReflectionTest)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
REFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(testFloat, "test Float", false, "Default")
REFLECT_STRUCT_MEMBER(testString, "test String", false, "Default")
REFLECT_STRUCT_MEMBER(testInt, "test Int", false, "Default")
REFLECT_STRUCT_MEMBER(testVector4D, "test Vector4 D", false, "Default")
REFLECT_STRUCT_MEMBER(testVector3D, "test Vector3 D", false, "Default")
REFLECT_STRUCT_MEMBER(testVector2D, "test Vector2 D", false, "Default")
REFLECT_STRUCT_MEMBER(testBool, "test Bool", false, "Default")
REFLECT_STRUCT_MEMBER(boolArray, "bool Array", false, "Default")
REFLECT_STRUCT_MEMBERS_END()
REFLECT_STRUCT_END()
