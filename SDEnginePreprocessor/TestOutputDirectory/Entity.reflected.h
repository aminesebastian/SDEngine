#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Entity.h"


//Reflection for class Entity
REFLECT_CLASS_BEGIN(Entity)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(EngineObject)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(CurrentTransform, Transform, false)
REFLECT_CLASS_MEMBER(LastFrameTransform, LastFrameTransform, true)
REFLECT_CLASS_MEMBER(bVisible, Visible, false)
REFLECT_CLASS_MEMBER(bHiddenInGame, HiddenInGame, false)
REFLECT_CLASS_MEMBER(bNeedsTick, NeedsTick, false)
REFLECT_CLASS_MEMBER(bNeedsBeginPlay, NeedsBeginPlay, false)
REFLECT_CLASS_MEMBER(bCastShadows, CastShadows, false)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
