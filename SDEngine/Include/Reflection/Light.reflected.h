//LastModifiedTime:637194022260515958
#pragma once
#include "Core/Reflection/Reflection.h"
#include "./Core\Objects\Entities\Light.h"


//Reflection for class Light
REFLECT_CLASS_BEGIN(Light)
REFLECT_CLASS_BEGIN_PARENTS()
REFLECT_CLASS_PARENT(Actor)
REFLECT_CLASS_PARENT_END()
REFLECT_CLASS_MEMBER(Intensity, "Intensity",  "Light", false)
REFLECT_CLASS_MEMBER(Attenuation, "Attenuation",  "Light", false)
REFLECT_CLASS_MEMBER(Color, "Color",  "Light", false)
REFLECT_CLASS_END()
