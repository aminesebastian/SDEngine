#include "Light.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include "EngineStatics.h"
#include "Engine.h"

Light::Light(TString Name, const Transform IntialTransform, ELightType Type, float Intensity, vec3 Color, float Attenuation, bool CastShadows) :
	Entity(Name, IntialTransform) {
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
		S_Probe = new StaticMesh(GetName(), GetTransform(), S_DebugMaterial, "./res/Sphere.fbx");
	}else{
		S_Probe = new StaticMesh(GetName(), GetTransform(), S_DebugMaterial, "./res/Arrow.fbx");
	}

	S_AABB = new FAABB(S_Probe->GetVerticies());
	if (S_LightInfo.Type == DIRECTIONAL) {
		S_ShadowOrthoMatrix = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);

		S_ShadowBuffer = new FrameBufferObject();
		S_ShadowBuffer->AddTextureIndex(new FFBOTextureEntry("shadowDepth", GL_RG32F, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_RGBA, GL_FLOAT));
		S_ShadowBuffer->Init(2048, 2048);

		//FramebufferName = 0;
		//glGenFramebuffers(1, &FramebufferName);

		//glGenTextures(1, &depthTexture);
		//glBindTexture(GL_TEXTURE_2D, depthTexture);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RG32F, 2048, 2048, 0, GL_RGBA, GL_FLOAT, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
		//float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
		//glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
		//glBindFramebuffer(GL_FRAMEBUFFER, FramebufferName);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, depthTexture, 0);

		//glGenTextures(1, &depthBufferTexture);
		//glBindTexture(GL_TEXTURE_2D, depthBufferTexture);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32, 2048, 2048, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_NONE);
		//glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthBufferTexture, 0);

		////glDrawBuffer(GL_NONE);
		////glReadBuffer(GL_NONE);
		//glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		//	cout << "ERROR: Unable to create shadow buffer for light." << endl;
		//}
	}
}
Light::~Light() {}

void Light::Tick(float DeltaTime) {
	//if(S_LightInfo.Type == DIRECTIONAL) {
	//	GetTransform().GetRotation().y += DeltaTime/1000;
	//}
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
	shader->SetShaderMatrix4("lights[" + std::to_string(index) + "].VPMatrix", GetLightOrthogonalMatrix()*GetLightViewMatrix());
	
	if (S_LightInfo.Type == DIRECTIONAL && CastsShadows()) {
		glActiveTexture(GL_TEXTURE0 + 9);
		glBindTexture(GL_TEXTURE_2D, S_ShadowBuffer->GetTexture(0));
		glUniform1i(glGetUniformLocation(shader->GetProgram(), S_ShadowBuffer->GetTextureName(0).c_str()), 9);
	}
}
mat4 Light::GetLightViewMatrix() {
	return lookAt(vec3(0,0,0), S_Transform.GetForwardVector(), vec3(0, 0, 1));
}
mat4 Light::GetLightOrthogonalMatrix() {
	return S_ShadowOrthoMatrix;
}

void Light::GenerateShadowTexture() {
	S_ShadowBuffer->BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);

	SArray<Entity*> entityList = Engine::GetInstance()->GetWorld()->GetWorldEntities();
	//glCullFace(GL_FRONT);
	for(int i=0; i<entityList.size(); i++) {
		if (entityList[i]->IsVisible()) {
			StaticMesh* temp = dynamic_cast<StaticMesh*>(entityList[i]);
			if (temp) {
				EngineStatics::GetShadowShader()->Bind();
				mat4 mvp;
				mvp = GetLightOrthogonalMatrix() * GetLightViewMatrix();
				mvp = mvp * temp->GetTransform().GetModelMatrix();
				EngineStatics::GetShadowShader()->SetShaderMatrix4("MVP", mvp);
				temp->Draw(NULL); // CLEAN THIS UP
			}
		}
	}
	//glCullFace(GL_BACK);
}
