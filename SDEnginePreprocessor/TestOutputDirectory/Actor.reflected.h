//LastModifiedTime:504911232000000000
#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Actor.h"


//Reflection for class Actor
REFLECT_CLASS_BEGIN(Actor)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Entity)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_FUNCTION_BEGIN()
REFLECT_CLASS_FUNCTION(GetWorldLocation, True, Vector3D, 0, True)
REFLECT_CLASS_FUNCTION(GetWorldRotation, True, Vector3D, 0, True)
REFLECT_CLASS_FUNCTION(GetWorldScale, True, Vector3D, 0, True)
REFLECT_CLASS_FUNCTION(GetWorldTransform, True, FTransform, 0, True)
REFLECT_CLASS_FUNCTION(GetLastFrameWorldTransform, True, FTransform, 0, True)
REFLECT_CLASS_FUNCTION_END()
REFLECT_CLASS_END()
