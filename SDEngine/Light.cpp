#include "Light.h"

Light::Light(const Transform IntialTransform, float Intensity, vec3 Color, float Attenuation) :
	Entity(IntialTransform) {
	S_LightInfo.Intensity = Intensity;
	S_LightInfo.Color = Color;
	S_LightInfo.Attenuation = Attenuation;
	S_DebugLight = false;
	S_DebugMaterial = new Material("./res/Shaders/LightDebugShader");
	S_DebugMaterial->SetShaderModel(EShaderModel::UNLIT);
	S_DebugMaterial->SetVec3Parameter("Color", Color);
	S_Probe = new StaticMesh(GetTransform(), S_DebugMaterial, "./res/Sphere.fbx");
}
Light::~Light() {}

void Light::Tick(float DeltaTime) {
	//GetTransform().GetPosition().x += DeltaTime;
}
void Light::Draw(Camera* Camera) {
	if (S_DebugLight) {
		S_Probe->SetTransform(GetTransform());
		S_Probe->Draw(Camera);
	}
}
