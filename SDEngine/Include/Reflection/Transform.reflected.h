#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Utilities\Math\Transform.h"


//Reflection for struct Transform
REFLECT_STRUCT_BEGIN(Transform)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
REFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(Location, Location, false)
REFLECT_STRUCT_MEMBER(Rotation, Rotation, false)
REFLECT_STRUCT_MEMBER(Scale, Scale, false)
REFLECT_STRUCT_MEMBERS_END()
REFLECT_STRUCT_END()
