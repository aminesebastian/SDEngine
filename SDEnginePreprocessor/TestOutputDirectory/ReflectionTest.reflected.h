//LastModifiedTime:504911232000000000
#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Test\ReflectionTest.h"


//Reflection for class FReflectionTest
REFLECT_CLASS_BEGIN(FReflectionTest)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(testFloat, "test Float", false, "Default")
REFLECT_CLASS_MEMBER(testString, "test String", false, "Default")
REFLECT_CLASS_MEMBER(testInt, "test Int", false, "Default")
REFLECT_CLASS_MEMBER(testVector4d, "test Vector4d", false, "Default")
REFLECT_CLASS_MEMBER(testVector3d, "test Vector3d", false, "Default")
REFLECT_CLASS_MEMBER(testVector2d, "test Vector2d", false, "Default")
REFLECT_CLASS_MEMBER(testBool, "test Bool", false, "Default")
REFLECT_CLASS_MEMBER(boolArray, "bool Array", false, "Default")
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_FUNCTION_BEGIN()
REFLECT_CLASS_FUNCTION(THISisATesat, True, int32, 1, True, {test, bool, True, 1}, {test2, bool, False, 0}, {test3, int32, False, 2})
REFLECT_CLASS_FUNCTION_END()
REFLECT_CLASS_END()
