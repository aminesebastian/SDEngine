//LastModifiedTime:637193338161159222
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Objects\EngineObject.h"


//Reflection for class EngineObject
REFLECT_CLASS_BEGIN(EngineObject)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBER(EngineObjectName, "Engine Object Name",  "Default", true)
REFLECT_CLASS_END()
