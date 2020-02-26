#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\EngineObject.h"


//Reflection for class EngineObject
REFLECT_CLASS_BEGIN(EngineObject)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT_END()
RREFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBERS_END(EngineObjectType)
REFLECT_CLASS_MEMBERS_END(EngineObjectName)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
