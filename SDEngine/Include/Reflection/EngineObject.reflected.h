#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Objects\EngineObject.h"

//Reflection for struct EngineObject
REFLECT_CLASS_BEGIN(EngineObject)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(EngineObjectType)
REFLECT_STRUCT_MEMBER(EngineObjectName)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
