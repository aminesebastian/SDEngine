#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Entity.h"

//Reflection for struct Entity
REFLECT_CLASS_BEGIN(Entity)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(EngineObject)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(bVisible)
REFLECT_STRUCT_MEMBER(bHiddenInGame)
REFLECT_STRUCT_MEMBER(bNeedsTick)
REFLECT_STRUCT_MEMBER(bNeedsBeginPlay)
REFLECT_STRUCT_MEMBER(bCastShadows)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
