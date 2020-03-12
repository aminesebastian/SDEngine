//LastModifiedTime:637195879499335823
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Utilities\Math\Transform.h"


//Reflection for struct FTransform
REFLECT_STRUCT_BEGIN(FTransform)
REFLECT_STRUCT_MEMBER(Location, "Location",  "Default", false)
REFLECT_STRUCT_MEMBER(Rotation, "Rotation",  "Default", false)
REFLECT_STRUCT_MEMBER(Scale, "Scale",  "Default", false)
REFLECT_STRUCT_END()
