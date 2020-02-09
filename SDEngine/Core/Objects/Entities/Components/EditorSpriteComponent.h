#pragma once
#include "Core/Objects/Entities/Component.h"

class Texture2D;

class EditorSpriteComponent : public Component {
public:
	EditorSpriteComponent(const TString& Name, TString TexturePath, vec3 Color);
	EditorSpriteComponent(const TString& Name, Texture2D* Sprite, vec3 Color);
	~EditorSpriteComponent();

	virtual void DrawAdvanced(const Camera* RenderCamera, const EDrawType& DrawType) override;
	virtual bool TraceAgainstRay(vec3 Origin, vec3 Direction, vec3& HitPoint, float& Distance, ECollisionChannel Channel = VISIBILITY);

	void SetTint(vec3 NewTint);
	vec3 GetTint();

protected:
	EditorSpriteComponent(const TString& Name);

private:
	Texture2D* Sprite;
	vec3 Tint;
	GLuint SpriteVertexArray;
	GLuint SpriteVertexBuffer;
};

