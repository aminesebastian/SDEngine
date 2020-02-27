#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Test\ReflectionTest.h"


//Reflection for struct FReflectionTest
REFLECT_STRUCT_BEGIN(FReflectionTest)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
REFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(testFloat, testFloat, false)
REFLECT_STRUCT_MEMBER(testString, testString, false)
REFLECT_STRUCT_MEMBER(testInt, testInt, false)
REFLECT_STRUCT_MEMBER(testVector4D, testVector4D, false)
REFLECT_STRUCT_MEMBER(testVector3D, testVector3D, false)
REFLECT_STRUCT_MEMBER(testVector2D, testVector2D, false)
REFLECT_STRUCT_MEMBER(testBool, testBool, false)
REFLECT_STRUCT_MEMBER(boolArray, boolArray, false)
REFLECT_STRUCT_MEMBERS_END()
REFLECT_STRUCT_END()
