#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Utilities\Math\Transform.h"


//Reflection for struct Transform
REFLECT_STRUCT_BEGIN(Transform)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
RREFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBERS_END(Location)
REFLECT_STRUCT_MEMBERS_END(Rotation)
REFLECT_STRUCT_MEMBERS_END(Scale)
RREFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_END()
