#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Objects\Entities\Camera.h"


//Reflection for class Camera
REFLECT_CLASS_BEGIN(Camera)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Actor)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(FieldOfView)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
