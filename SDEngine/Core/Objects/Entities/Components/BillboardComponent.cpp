#include "BillboardComponent.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/Entities/Camera.h"

BillboardComponent::BillboardComponent(const TString& Name, TString TexturePath, const FColor& Color) : BillboardComponent(Name, new Texture2D(TexturePath), Color) { }
BillboardComponent::BillboardComponent(const TString& Name, Texture2D* Sprite, const FColor& Color) : BillboardComponent(Name) {
	this->Sprite = Sprite;
	Tint = Color;
}
BillboardComponent::BillboardComponent(const TString& Name) : Component(Name) {
	SpriteVertexArray = 0;
	SpriteVertexBuffer = 0;
	//S_AABB = new AxisAlignedBoundingBox(vec3(0.0f, -1.0f, -1.0f), vec3(0.0f, 1.0f, 1.0f));

	Sprite = nullptr;
	Tint = vec4(1.0f, 1.0f, 1.0f, 1.0f);
	GLfloat quadVertices[] = {
		0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.0f, 1.0f, -1.0f, 1.0f, 1.0f,
		0.0f, -1.0f, -1.0f, 0.0f, 1.0f,
		0.0f, 1.0f, 1.0f, 1.0f, 0.0f,
		0.0f, -1.0f, 1.0f, 0.0f, 0.0f,
		0.0f, -1.0f, -1.0f, 0.0f, 1.0f
	};

	glGenVertexArrays(1, &SpriteVertexArray);
	glGenBuffers(1, &SpriteVertexBuffer);
	glBindVertexArray(SpriteVertexArray);
	glBindBuffer(GL_ARRAY_BUFFER, SpriteVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	SetCastShadows(false);
	SetHiddenInGame(true);
}
BillboardComponent::~BillboardComponent() {

}

void BillboardComponent::DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) {
	if (Sprite == nullptr) {
		return;
	}

	if (RenderCamera != nullptr) {
		SetRotation(RenderCamera->GetRotation());
	}

	if (DrawType == SCENE_RENDER) {
		EngineStatics::GetSpriteShader()->Bind();
		EngineStatics::GetSpriteShader()->Update(GetWorldTransform(), GetLastFrameWorldTransform(), RenderCamera);
		EngineStatics::GetSpriteShader()->SetShaderVector3("TINT", Tint);
		Sprite->Bind("Sprite", EngineStatics::GetSpriteShader(), 0);
	}

	glBindVertexArray(SpriteVertexArray);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 8);
	glBindVertexArray(0);
}
bool BillboardComponent::TraceAgainstRay(const Vector3D& Origin, const Vector3D& Direction, Vector3D& HitPoint, float& Distance, ECollisionChannel Channel) {
	//if (GetAxisAlignedBoundingBox()->IntersectWithRay(Origin, Direction, GetTransform(), HitPoint, Distance)) {
	//	mat4 invModelMatrix = glm::inverse(GetTransform().GetModelMatrix());
	//	vec3 objectSpaceOrigin = invModelMatrix * vec4(Origin.x, Origin.y, Origin.z, 1.0f);
	//	vec3 objectSpaceDirection = invModelMatrix * vec4(Direction.x, Direction.y, Direction.z, 0.0f);
	//	return true;
	//}
	return false;
}

void BillboardComponent::SetTint(const FColor& NewTint) {
	Tint = NewTint;
}
const FColor& BillboardComponent::GetTint() const {
	return Tint;
}