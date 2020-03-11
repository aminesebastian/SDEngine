//LastModifiedTime:637195019153877878
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Test\ReflectionTest.h"


//Reflection for class FReflectionTest
REFLECT_CLASS_BEGIN(FReflectionTest)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(EngineObject)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBER(testFloat, "test Float",  "Default", false)
REFLECT_CLASS_MEMBER(testString, "test String",  "Default", false)
REFLECT_CLASS_MEMBER(testInt, "test Int",  "Default", false)
REFLECT_CLASS_MEMBER(testVector4d, "test Vector4d",  "Default", false)
REFLECT_CLASS_MEMBER(testVector3d, "test Vector3d",  "Default", false)
REFLECT_CLASS_MEMBER(testVector2d, "test Vector2d",  "Default", false)
REFLECT_CLASS_MEMBER(testBool, "test Bool",  "Default", false)
REFLECT_CLASS_MEMBER(boolArray, "bool Array",  "Default", false)
REFLECT_CLASS_FUNCTION(THISisATesat, true, int32, 1, true, {test, bool, True, 1}, {test2, bool, False, 0}, {test3, int32, False, 2})
REFLECT_CLASS_END()
