#include "Light.h"
#include "EngineStatics.h"

Light::Light(const Transform IntialTransform, ELightType Type, float Intensity, vec3 Color, float Attenuation) :
	Entity(IntialTransform) {
	S_LightInfo.Type = Type;
	S_LightInfo.Intensity = Intensity;
	S_LightInfo.Color = Color;
	S_LightInfo.Attenuation = Attenuation;
	S_DebugLight = false;
	S_DebugMaterial = new Material(EngineStatics::GetLightDebugShader());
	S_DebugMaterial->SetShaderModel(EShaderModel::UNLIT);
	S_DebugMaterial->SetVec3Parameter("Color", Color);
	if(S_LightInfo.Type == POINT) {
		S_Probe = new StaticMesh(GetTransform(), S_DebugMaterial, "./res/Sphere.fbx");
	}else{
		S_Probe = new StaticMesh(GetTransform(), S_DebugMaterial, "./res/Arrow.fbx");
	}
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
void Light::SendShaderInformation(Shader* shader, int index) {
	shader->SetShaderInteger("lights[" + std::to_string(index) + "].Type", GetLightInfo().Type);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Intensity", GetLightInfo().Intensity);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Attenuation", GetLightInfo().Attenuation);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Color", GetLightInfo().Color);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Position", GetTransform().GetPosition());
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Direction", GetTransform().GetForwardVector());
}