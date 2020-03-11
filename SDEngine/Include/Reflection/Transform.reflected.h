//LastModifiedTime:637195019153897818
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Utilities\Math\Transform.h"


//Reflection for struct FTransform
REFLECT_STRUCT_BEGIN(FTransform)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
REFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(Location, "Location",  "Default", false)
REFLECT_STRUCT_MEMBER(Rotation, "Rotation",  "Default", false)
REFLECT_STRUCT_MEMBER(Scale, "Scale",  "Default", false)
REFLECT_STRUCT_MEMBERS_END()
REFLECT_STRUCT_END()
