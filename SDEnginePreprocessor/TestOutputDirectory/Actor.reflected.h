#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Actor.h"

//Reflection for struct Actor
REFLECT_CLASS_BEGIN(Actor)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Entity)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
