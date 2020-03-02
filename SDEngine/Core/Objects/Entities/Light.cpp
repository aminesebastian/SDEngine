#include "Light.h"
#include <GLM/glm.hpp>
#include <GLM/gtx/transform.hpp>
#include "Core/Engine/EngineStatics.h"
#include "Core/Engine/Engine.h"
#include "Core/Rendering/DefferedCompositor.h"
#include "Core/Assets/AssetManager.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Light.reflected.h"


Light::Light(const TString& Name, const Transform& IntialTransform, const ELightType& TypeIn, const float& IntensityIn, const FColor& ColorIn, const float& AttenuationIn, const bool& CastShadows) : Actor(Name) {
	SetTransform(IntialTransform);
	SetCastShadows(CastShadows);

	Type = TypeIn;
	Intensity = IntensityIn;
	Color = ColorIn;
	Attenuation = AttenuationIn;

	RegisterComponent(new BillboardComponent("PointLightSprite", Engine::Get()->GetAssetManager()->FindAsset<Texture2D>("./Res/Assets/Editor/Textures/PointLightSprite.sasset"), Color));

	//S_AABB = new AxisAlignedBoundingBox(vec3(0.0f, -1.0f, -1.0f), vec3(0.0f, 1.0f, 1.0f));

	if (Type == DIRECTIONAL) {
		OrthographicMatrix = glm::ortho(-40.0f, 40.0f, -40.0f, 40.0f, -40.0f, 40.0f);

		PrimaryShadowBuffer = new RenderTarget(vec2(2048, 2048));
		PrimaryShadowBuffer->AddTextureIndex(new FRenderTargetTextureEntry("shadowDepth", GL_RG32F, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_RGBA, GL_FLOAT));
		PrimaryShadowBuffer->AddDepthStencilBuffer();
		PrimaryShadowBuffer->FinalizeRenderTarget();

		SecondaryShadowBuffer = new RenderTarget(vec2(2048, 2048));
		SecondaryShadowBuffer->AddTextureIndex(new FRenderTargetTextureEntry("shadowDepth", GL_RG32F, GL_LINEAR, GL_CLAMP_TO_BORDER, GL_RGBA, GL_FLOAT));
		PrimaryShadowBuffer->AddDepthStencilBuffer();
		SecondaryShadowBuffer->FinalizeRenderTarget();
	}
}
Light::~Light() {}

void Light::SetLightColor(const Vector3D& ColorIn) {
	Color = ColorIn;
}
void Light::SetLightIntensity(const float& IntensityIn) {
	Intensity = IntensityIn;
}

void Light::SendShaderInformation(Shader* shader, const uint8& index) {
	shader->SetShaderInteger("lights[" + std::to_string(index) + "].Type", Type);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Intensity", Intensity);
	shader->SetShaderFloat("lights[" + std::to_string(index) + "].Attenuation", Attenuation);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Color", Color);
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Position", GetTransform().GetLocation());
	shader->SetShaderVector3("lights[" + std::to_string(index) + "].Direction", GetTransform().GetForwardVector());
	shader->SetShaderInteger("lights[" + std::to_string(index) + "].CastsShadow", CastsShadows());
	shader->SetShaderMatrix4("lights[" + std::to_string(index) + "].VPMatrix", GetLightOrthogonalMatrix() * GetLightViewMatrix());

	if (Type == DIRECTIONAL && CastsShadows()) {
		glActiveTexture(GL_TEXTURE0 + 9);
		glBindTexture(GL_TEXTURE_2D, PrimaryShadowBuffer->GetTexture(0));
		glUniform1i(glGetUniformLocation(shader->GetProgram(), PrimaryShadowBuffer->GetTextureName(0).c_str()), 9);
	}
}
Matrix4 Light::GetLightViewMatrix() {
	return lookAt(vec3(0, 0, 0), CurrentTransform.GetForwardVector(), vec3(0, 0, 1));
}
Matrix4 Light::GetLightOrthogonalMatrix() {
	return OrthographicMatrix;
}
void Light::GenerateShadowTexture(DefferedCompositor* Compositor) {
	if (!bCastShadows) {
		return;
	}
	PrimaryShadowBuffer->BindForWriting();
	glClear(GL_DEPTH_BUFFER_BIT);

	SArray<Actor*> entityList = GetWorld()->GetWorldActors();
	for (Actor* actor : GetWorld()->GetWorldActors()) {
		if (actor->ShouldBeDrawn(SHADOW_MAP_RENDER)) {
			EngineStatics::GetShadowShader()->Bind();
			mat4 mvp;
			mvp = GetLightOrthogonalMatrix() * GetLightViewMatrix();
			mvp = mvp * actor->GetTransform().GetModelMatrix();
			EngineStatics::GetShadowShader()->SetShaderMatrix4("MVP", mvp);
			actor->DrawAdvanced(nullptr, SHADOW_MAP_RENDER);
		}
	}

	SecondaryShadowBuffer->BindForWriting();

	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, PrimaryShadowBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(EngineStatics::GetGausBlur7x1Shader()->GetProgram(), PrimaryShadowBuffer->GetTextureName(0).c_str()), 0);

	Compositor->DrawScreenQuad();

	PrimaryShadowBuffer->BindForWriting();


	glEnable(GL_TEXTURE_2D);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, SecondaryShadowBuffer->GetTexture(0));
	glUniform1i(glGetUniformLocation(EngineStatics::GetGausBlur7x1Shader()->GetProgram(), SecondaryShadowBuffer->GetTextureName(0).c_str()), 0);

	Compositor->DrawScreenQuad();
}
void Light::BlurTexture(RenderTarget* ReadBuffer, RenderTarget* WriteBuffer) {

}
