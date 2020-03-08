//LastModifiedTime:504911232000000000
#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Entity.h"


//Reflection for class Entity
REFLECT_CLASS_BEGIN(Entity)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(EngineObject)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(LastFrameTransform, "Last Frame Transform", true, "Default")
REFLECT_CLASS_MEMBER(bVisible, "Visible", false, "Entity")
REFLECT_CLASS_MEMBER(bHiddenInGame, "Hidden In Game", false, "Entity")
REFLECT_CLASS_MEMBER(bNeedsTick, "Needs Tick", false, "Entity")
REFLECT_CLASS_MEMBER(bNeedsBeginPlay, "Needs Begin Play", false, "Entity")
REFLECT_CLASS_MEMBER(bCastShadows, "Cast Shadows", false, "Entity")
REFLECT_CLASS_MEMBER(Transform, "Transform", false, "Transform")
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
