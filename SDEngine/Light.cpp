#include "Light.h"

Light::Light(const Transform IntialTransform, float Intensity, vec3 Color, float Attenuation) :
	Entity(IntialTransform) {
	S_LightInfo.Intensity = Intensity;
	S_LightInfo.Color = Color;
	S_LightInfo.Attenuation = Attenuation;
	S_Probe = new StaticMesh(GetTransform(), "./res/Sphere.fbx");
	S_Probe->SetCustomColor(Color);
	S_Probe->SetMaterialID(1);
}
Light::~Light() {}

void Light::Tick(float DeltaTime) {
	//GetTransform().GetPosition().x += DeltaTime;
}
void Light::Draw(Shader& shader) {
	if (S_DebugLight) {
		S_Probe->Draw(shader);
	}
}
