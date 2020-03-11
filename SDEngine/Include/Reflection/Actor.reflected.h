//LastModifiedTime:637195019153887855
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Objects\Entities\Actor.h"


//Reflection for class Actor
REFLECT_CLASS_BEGIN(Actor)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Entity)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_FUNCTION(GetWorldLocation, true, Vector3D, 0, true)
REFLECT_CLASS_FUNCTION(GetWorldRotation, true, Vector3D, 0, true)
REFLECT_CLASS_FUNCTION(GetWorldScale, true, Vector3D, 0, true)
REFLECT_CLASS_FUNCTION(GetWorldTransform, true, FTransform, 0, true)
REFLECT_CLASS_FUNCTION(GetLastFrameWorldTransform, true, FTransform, 0, true)
REFLECT_CLASS_END()
