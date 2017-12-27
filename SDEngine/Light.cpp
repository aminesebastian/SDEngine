#include "Light.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include "EngineStatics.h"
#include "Engine.h"

Light::Light(const Transform IntialTransform, ELightType Type, float Intensity, vec3 Color, float Attenuation, bool CastShadows) :
	Entity(IntialTransform) {
	S_LightInfo.Type = Type;
	S_LightInfo.Intensity = Intensity;
	S_LightInfo.Color = Color;
	S_LightInfo.Attenuation = Attenuation;
	bDebugLight = false;
	S_DebugMaterial = new Material(EngineStatics::GetLightDebugShader());
	S_DebugMaterial->SetShaderModel(EShaderModel::UNLIT);
	S_DebugMaterial->SetVec3Parameter("Color", Color);

	bCastShadows = CastShadows;
	if(S_LightInfo.Type == POINT) {
		S_Probe = new StaticMesh(GetTransform(), S_DebugMaterial, "./res/Sphere.fbx");
	}else{
		S_Probe = new StaticMesh(GetTransform(), S_DebugMaterial, "./res/Arrow.fbx");
	}
	if (S_LightInfo.Type == DIRECTIONAL) {
		S_ShadowOrthoMatrix = glm::ortho(0.0f, 10.0f, 10.0f, 0.0f, 0.0f, 50.0f);
		S_ShadowDepthBuffer = new FrameBufferObject();
		S_ShadowDepthBuffer->AddTextureIndex(new FFBOTextureEntry("shadowDepth", GL_RGBA16F, GL_NEAREST, GL_CLAMP_TO_EDGE, GL_RGBA, GL_FLOAT));
		S_ShadowDepthBuffer->Init(1024, 1024);
	}
}
Light::~Light() {}

void Light::Tick(float DeltaTime) {
	//GetTransform().GetPosition().x += DeltaTime;
}
void Light::Draw(Camera* Camera) {
	if (bDebugLight) {
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
	shader->SetShaderInteger("lights[" + std::to_string(index) + "].CastsShadow", CastsShadows());
	if (S_LightInfo.Type == DIRECTIONAL && CastsShadows()) {
		glEnable(GL_TEXTURE_2D);
		glActiveTexture(GL_TEXTURE0 + 9);
		glBindTexture(GL_TEXTURE_2D, S_ShadowDepthBuffer->GetTexture(0));
		glUniform1i(glGetUniformLocation(shader->GetProgram(), S_ShadowDepthBuffer->GetTextureName(0).c_str()), 9);
	}
}
void Light::GenerateShadowTexture() {
	S_ShadowDepthBuffer->BindForWriting();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	EngineStatics::GetShadoowShader()->Bind();
	SArray<Entity*> entityList = Engine::GetInstance()->GetWorld()->GetWorldEntities();
	for(int i=0; i<entityList.size(); i++) {
		if (entityList[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(entityList[i]);
			if (temp) {
				mat4 mvp;
				mvp = S_ShadowOrthoMatrix * GetViewMatrix() * temp->GetTransform().GetModelMatrix();
				EngineStatics::GetShadoowShader()->SetShaderMatrix4("MVP", mvp);
				temp->Draw(NULL); // CLEAN THIS UP
			}
		}
	}
}
