//LastModifiedTime:504911232000000000
#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Utilities\Math\Transform.h"


//Reflection for struct FTransform
REFLECT_STRUCT_BEGIN(FTransform)
REFLECT_STRUCT_MEMBER(Location, "Location",  "Default", false)
REFLECT_STRUCT_MEMBER(Rotation, "Rotation",  "Default", false)
REFLECT_STRUCT_MEMBER(Scale, "Scale",  "Default", false)
REFLECT_STRUCT_END()
