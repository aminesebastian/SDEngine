#pragma once
#include "Core/Reflection/Reflection.h"
#include "../../../../SDEngine\Core\Objects\Entities\Light.h"


//Reflection for struct FLightInfo
REFLECT_STRUCT_BEGIN(FLightInfo)
REFLECT_STRUCT_BEGIN_PARENTS()
REFLECT_STRUCT_PARENT_END()
REFLECT_STRUCT_MEMBERS_BEGIN()
REFLECT_STRUCT_MEMBER(Intensity, Intensity, false, Default)
REFLECT_STRUCT_MEMBER(Color, Color, false, Default)
REFLECT_STRUCT_MEMBER(Attenuation, Attenuation, false, Default)
REFLECT_STRUCT_MEMBERS_END()
REFLECT_STRUCT_END()

//Reflection for class Light
REFLECT_CLASS_BEGIN(Light)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Actor)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBERS_BEGIN()
REFLECT_CLASS_MEMBER(testVal, test Val, false, Default)
REFLECT_CLASS_MEMBER(LightInfo, Light Info, false, Default)
REFLECT_CLASS_MEMBERS_END()
REFLECT_CLASS_END()
