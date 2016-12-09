#pragma once
#include "Entity.h"
#include "StaticMesh.h"

enum class ELightType {
	Point,
	Spot,
	Ambient,
	Directional
};
struct FLightInfo {
	float Intensity;
	vec3 Color;
	float Attenuation;
	float Hardness = 0.1;
	ELightType Type = ELightType::Point;
};


class Light : public Entity{
public:
	Light(const World& World, const Transform IntialTransform, float Intensity = 20, vec3 Color = vec3(1,1,1), float Attenuation = 25) :
		Entity(World, IntialTransform) {
		S_LightInfo.Intensity = Intensity;
		S_LightInfo.Color = Color;
		S_LightInfo.Attenuation = Attenuation;
		S_Probe = &StaticMesh(World, GetTransform(), "./res/Sphere.fbx");
	}
	~Light();

	FLightInfo& GetLightInfo() { return S_LightInfo; }
	void SetLightColor(const vec3 Color) { S_LightInfo.Color = Color; }
	void SetLightIntensity(const float Intensity) { S_LightInfo.Intensity = Intensity; }
	void SendShaderInformation(Shader& shader, int index) {
		glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Intensity").c_str()), GetLightInfo().Intensity);
		glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Attenuation").c_str()), GetLightInfo().Attenuation);
		glUniform3fv(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Color").c_str()), 1, &GetLightInfo().Color[0]);
		glUniform3fv(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Position").c_str()), 1, &GetTransform().GetPosition()[0]);
		glUniform3fv(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Direction").c_str()), 1, &GetTransform().GetForwardVector()[0]);

		//switch (GetLightInfo().Type) {
		//	case ELightType::Ambient :glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Type").c_str()), 0);
		//		break;
		//	case ELightType::Spot:glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Type").c_str()), 1);
		//		break;
		//	case ELightType::Point:glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Type").c_str()), 2);
		//		break;
		//	case ELightType::Directional:glUniform1f(glGetUniformLocation(shader.GetProgram(), ("lights[" + std::to_string(index) + "].Type").c_str()), 3);
		//		break;
		//}
	}
	virtual void Draw(Shader& shader) override;
	virtual void Tick(float DeltaTime) override;
private:
	FLightInfo S_LightInfo;
	StaticMesh* S_Probe;
};

