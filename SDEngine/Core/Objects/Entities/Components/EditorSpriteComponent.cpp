#include "EditorSpriteComponent.h"
#include "Core/Engine/EngineStatics.h"
#include "Core/Objects/CoreTypes/Texture2D.h"
#include "Core/Objects/Entities/Actor.h"
#include "Core/Objects/Entities/Camera.h"

EditorSpriteComponent::EditorSpriteComponent(const TString& Name, TString TexturePath, vec3 Color) : EditorSpriteComponent(Name, new Texture2D(TexturePath), Color) { }
EditorSpriteComponent::EditorSpriteComponent(const TString& Name, Texture2D* Sprite, vec3 Color) : EditorSpriteComponent(Name) {
	this->Sprite = Sprite;
	Tint = Color;
}
EditorSpriteComponent::EditorSpriteComponent(const TString& Name) : Component(Name) {
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
EditorSpriteComponent::~EditorSpriteComponent() {

}

void EditorSpriteComponent::DrawAdvanced(Camera* RenderCamera, EDrawType DrawType) {
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
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}
bool EditorSpriteComponent::TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance, ECollisionChannel Channel) {
	//if (GetAxisAlignedBoundingBox()->IntersectWithRay(Origin, Direction, GetTransform(), HitPoint, Distance)) {
	//	mat4 invModelMatrix = glm::inverse(GetTransform().GetModelMatrix());
	//	vec3 objectSpaceOrigin = invModelMatrix * vec4(Origin.x, Origin.y, Origin.z, 1.0f);
	//	vec3 objectSpaceDirection = invModelMatrix * vec4(Direction.x, Direction.y, Direction.z, 0.0f);
	//	return true;
	//}
	return false;
}

void EditorSpriteComponent::SetTint(vec3 NewTint) {
	Tint = NewTint;
}
vec3 EditorSpriteComponent::GetTint() {
	return Tint;
}