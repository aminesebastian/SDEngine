#include "Light.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include "EngineStatics.h"
#include "Engine.h"
#include "DefferedCompositor.h"
#include "UserInterface.h"

Light::Light(TString Name, const Transform IntialTransform, ELightType Type, float Intensity, vec3 Color, float Attenuation, bool CastShadows) :
	Entity(Name, IntialTransform) {
	S_LightInfo.Type = Type;
	S_LightInfo.Intensity = Intensity;
	S_LightInfo.Color = Color;
	S_LightInfo.Attenuation = Attenuation;
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

		S_ShadowBuffer = new RenderTarget();
		S_ShadowBuffer->AddTextureIndex(new FRenderTargetTextureEntry("shadowDepth", GL_RG32F, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_RGBA, GL_FLOAT));
		S_ShadowBuffer->Init(2048, 2048);

		S_ShadowBufferTemp = new RenderTarget();
		S_ShadowBufferTemp->AddTextureIndex(new FRenderTargetTextureEntry("shadowDepth", GL_RG32F, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_RGBA, GL_FLOAT));
		S_ShadowBufferTemp->Init(2048, 2048);
	}
}
Light::~Light() {}

void Light::Tick(float DeltaTime) {
	if(S_LightInfo.Type == DIRECTIONAL) {
		//GetTransform().GetRotation().y += DeltaTime/1000;
	}
}
void Light::Draw(Camera* Camera) {
	S_Probe->SetTransform(GetTransform());
	S_Probe->Draw(Camera);
}
void Light::SendShaderInformation(Shader* shader, int index) {
	shader->SetShaderInteger("lights[" + std::to_string(index) + "].Type", GetLightInfo().Type);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Intensity", GetLightInfo().Intensity);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Attenuation", GetLightInfo().Attenuation);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Color", GetLightInfo().Color);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Position", GetTransform().GetLocation());
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

bool Light::PopulateDetailsPanel() {
	Entity::PopulateDetailsPanel();
	if (ImGui::CollapsingHeader("Light Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::Text("Intensity");
		ImGui::DragFloat("Intensity", &S_LightInfo.Intensity, 0.01f);
		ImGui::Text("Attenuation");
		ImGui::DragFloat("Attenuation", &S_LightInfo.Attenuation, 0.01f);
		ImGui::Text("Color");
		ImGui::ColorEdit3("Color", &S_LightInfo.Color[0]);
	}
	return true;
}

void Light::GenerateShadowTexture(DefferedCompositor* Compositor) {
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

	S_ShadowBufferTemp->BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	EngineStatics::GetGausBlur7x1Shader()->Bind();
	EngineStatics::GetGausBlur7x1Shader()->SetShaderVector2("SCALE", vec2(1.0f, 0.0f));

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, S_ShadowBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(EngineStatics::GetGausBlur7x1Shader()->GetProgram(), S_ShadowBuffer->GetTextureName(0).c_str()), 0);

	Compositor->DrawScreenQuad();

	S_ShadowBuffer->BindForWriting();

	glClear(GL_DEPTH_BUFFER_BIT);

	EngineStatics::GetGausBlur7x1Shader()->SetShaderVector2("SCALE", vec2(0.0f, 1.0f));

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, S_ShadowBufferTemp->GetTexture(0));
	glUniform1i(glGetUniformLocation(EngineStatics::GetGausBlur7x1Shader()->GetProgram(), S_ShadowBufferTemp->GetTextureName(0).c_str()), 0);

	Compositor->DrawScreenQuad();
}
void Light::BlurTexture(RenderTarget* ReadBuffer, RenderTarget* WriteBuffer) {
	
}

bool Light::TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance) {
	return S_Probe->TraceAgainstRay(Origin, Direction, HitPoint, Distance);
}
