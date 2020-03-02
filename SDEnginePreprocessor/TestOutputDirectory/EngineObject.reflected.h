#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\EngineObject.h"


//Reflection for class EngineObject
REFLECT_CLASS_BEGIN(EngineObject)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(EngineObjectType, Engine Object Type, true, Default)
REFLECT_CLASS_MEMBER(EngineObjectName, Engine Object Name, true, Default)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
