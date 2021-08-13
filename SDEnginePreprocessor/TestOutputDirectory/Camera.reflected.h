//LastModifiedTime:504911232000000000
#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Camera.h"


//Reflection for class Camera
REFLECT_CLASS_BEGIN(Camera)
REFLECT_CLASS_PARENT(Actor)
REFLECT_CLASS_MEMBER(FieldOfView, "Field Of View",  "Default", false)
REFLECT_CLASS_END()
